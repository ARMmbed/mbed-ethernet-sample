/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "serial_api.h"

#if DEVICE_SERIAL

// math.h required for floating point operations for baud rate calculation
#include <math.h>
#include "mbed-drivers/mbed_assert.h"

#include <string.h>

#include "cmsis.h"
#include "pinmap.h"
#include "fsl_uart_hal.h"
#include "fsl_clock_manager.h"
#include "fsl_uart_features.h"
#include "PeripheralPins.h"
#include "uvisor-lib/uvisor-lib.h"
 
void uart0_irq(void);
void uart1_irq(void);
void uart2_irq(void);
void uart3_irq(void);
void uart4_irq(void);

static void serial_tx_enable_interrupt(serial_t *obj, uint32_t address, uint8_t enable);
static void serial_rx_enable_interrupt(serial_t *obj, uint32_t address, uint8_t enable);
static void serial_interrupt_vector_set(serial_t *obj, uint32_t handler);
static int serial_read_match_asynch(serial_t *obj);
static int serial_write_asynch(serial_t *obj);
static int serial_read_asynch(serial_t *obj);
static void serial_buffer_tx_write(serial_t *obj);
static int serial_buffer_rx_read(serial_t *obj);
static uint32_t serial_rx_error_event_check(serial_t *obj);
static uint32_t serial_rx_event_check(serial_t *obj);
static uint8_t serial_is_tx_complete(serial_t *obj);
static void serial_tx_buffer_set(serial_t *obj, void *tx, size_t length, uint8_t width);
static void serial_rx_buffer_set(serial_t *obj, void *rx, size_t length, uint8_t width);
static void serial_rx_set_char_match(serial_t *obj, uint8_t char_match);

/* TODO:
    putchar/getchar 9 and 10 bits support
*/
#ifndef UART3_BASE
#define UART_NUM    3
#else
#define UART_NUM    5
#endif

static uint32_t serial_irq_ids[UART_NUM] = {0};
static uart_irq_handler irq_handler;

int stdio_uart_inited = 0;
serial_t stdio_uart;

void serial_init(serial_t *obj, PinName tx, PinName rx) {
    uint32_t uart_tx = pinmap_peripheral(tx, PinMap_UART_TX);
    uint32_t uart_rx = pinmap_peripheral(rx, PinMap_UART_RX);
    obj->serial.instance = pinmap_merge(uart_tx, uart_rx);
    MBED_ASSERT((int)obj->serial.instance != NC);

    uint32_t uartSourceClock = CLOCK_SYS_GetUartFreq(obj->serial.instance);

    CLOCK_SYS_EnableUartClock(obj->serial.instance);
    uint32_t serial_address[] = UART_BASE_ADDRS;
    obj->serial.address = serial_address[obj->serial.instance];
    UART_HAL_Init(obj->serial.address);
    UART_HAL_SetBaudRate(obj->serial.address, uartSourceClock, 9600);
    UART_HAL_SetParityMode(obj->serial.address, kUartParityDisabled);
    #if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
    UART_HAL_SetStopBitCount(obj->serial.address, kUartOneStopBit);
    #endif
    UART_HAL_SetBitCountPerChar(obj->serial.address, kUart8BitsPerChar);
    obj->serial.databits = 8;

    pinmap_pinout(tx, PinMap_UART_TX);
    pinmap_pinout(rx, PinMap_UART_RX);

    switch (obj->serial.instance) {
        case 0: obj->serial.irq_number = UART0_RX_TX_IRQn; obj->serial.vector_cur = (uint32_t)&uart0_irq; break;
        case 1: obj->serial.irq_number = UART1_RX_TX_IRQn; obj->serial.vector_cur = (uint32_t)&uart1_irq; break;
        case 2: obj->serial.irq_number = UART2_RX_TX_IRQn; obj->serial.vector_cur = (uint32_t)&uart2_irq; break;
#if (UART_NUM > 3)
        case 3: obj->serial.irq_number = UART3_RX_TX_IRQn; obj->serial.vector_cur = (uint32_t)&uart3_irq; break;
        case 4: obj->serial.irq_number = UART4_RX_TX_IRQn; obj->serial.vector_cur = (uint32_t)&uart4_irq; break;
#endif
    }

    if (obj->serial.instance == STDIO_UART) {
        stdio_uart_inited = 1;
        memcpy(&stdio_uart, obj, sizeof(serial_t));
    }
    uint8_t fifo_size = UART_HAL_GetTxFifoSize(obj->serial.address);
    obj->serial.entry_count = (fifo_size == 0 ? 1 : 0x1 << (fifo_size + 1));
    UART_HAL_ClearAllNonAutoclearStatusFlags(obj->serial.address);
    // Flashing fifo must be done with a disabled module, thus enabling TX happens after RX is enabled
    if (tx != NC) {
        pin_mode(tx, PullUp);
        UART_HAL_SetTxFifoCmd(obj->serial.address, true);
        UART_HAL_FlushTxFifo(obj->serial.address);
    }
    if (rx != NC) {
        pin_mode(rx, PullUp);
        UART_HAL_SetRxFifoCmd(obj->serial.address, true);
        UART_HAL_FlushRxFifo(obj->serial.address);
        UART_HAL_EnableReceiver(obj->serial.address);
    }
    if (tx != NC) {
        UART_HAL_EnableTransmitter(obj->serial.address);
    }
    obj->char_match = SERIAL_RESERVED_CHAR_MATCH;

}

void serial_free(serial_t *obj) {
    serial_irq_ids[obj->serial.instance] = 0;
}

void serial_baud(serial_t *obj, int baudrate) {
    UART_HAL_SetBaudRate(obj->serial.address, CLOCK_SYS_GetUartFreq(obj->serial.instance), (uint32_t)baudrate);
}

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits) {
    // TODO assert for not supported parity and data bits
    uart_bit_count_per_char_t bit_count = (data_bits == 8 ? kUart8BitsPerChar : kUart9BitsPerChar);
    UART_HAL_SetBitCountPerChar(obj->serial.address, bit_count);
    obj->serial.databits = data_bits;
    uart_parity_mode_t uart_parity;
    if (parity == ParityOdd) {
        uart_parity = kUartParityOdd;
    } else if (parity == ParityEven) {
        uart_parity = kUartParityEven;
    } else {
        uart_parity = kUartParityDisabled;
    }
    UART_HAL_SetParityMode(obj->serial.address, uart_parity);
#if FSL_FEATURE_UART_HAS_STOP_BIT_CONFIG_SUPPORT
    uart_stop_bit_count_t stop_count = (stop_bits == 1) ? kUartOneStopBit : kUartTwoStopBit;
    UART_HAL_SetStopBitCount(obj->serial.address, stop_count);
#endif
}

/******************************************************************************
 * INTERRUPTS HANDLING
 ******************************************************************************/
static inline void uart_irq(uint32_t transmit_empty, uint32_t receive_full, uint32_t index) {
    if (serial_irq_ids[index] != 0) {
        if (transmit_empty)
            irq_handler(serial_irq_ids[index], TxIrq);

    if (receive_full)
        irq_handler(serial_irq_ids[index], RxIrq);
    }
}

void uart0_irq() {
    uart_irq(UART_HAL_GetTxDataRegEmptyIntCmd(UART0_BASE) && UART_HAL_IsTxDataRegEmpty(UART0_BASE), UART_HAL_GetRxDataRegFullIntCmd(UART0_BASE) && UART_HAL_IsRxDataRegFull(UART0_BASE), 0);
    if (UART_HAL_GetStatusFlag(UART0_BASE, kUartRxOverrun))
        UART_HAL_ClearStatusFlag(UART0_BASE, kUartRxOverrun);
}
void uart1_irq() {
    uart_irq(UART_HAL_GetTxDataRegEmptyIntCmd(UART1_BASE) && UART_HAL_IsTxDataRegEmpty(UART1_BASE), UART_HAL_GetRxDataRegFullIntCmd(UART1_BASE) && UART_HAL_IsRxDataRegFull(UART1_BASE), 1);
}

void uart2_irq() {
    uart_irq(UART_HAL_GetTxDataRegEmptyIntCmd(UART2_BASE) && UART_HAL_IsTxDataRegEmpty(UART2_BASE), UART_HAL_GetRxDataRegFullIntCmd(UART2_BASE) && UART_HAL_IsRxDataRegFull(UART2_BASE), 2);
}

#if (UART_NUM > 3)

void uart3_irq() {
    uart_irq(UART_HAL_GetTxDataRegEmptyIntCmd(UART3_BASE) && UART_HAL_IsTxDataRegEmpty(UART3_BASE), UART_HAL_GetRxDataRegFullIntCmd(UART3_BASE) && UART_HAL_IsRxDataRegFull(UART3_BASE), 3);
}

void uart4_irq() {
    uart_irq(UART_HAL_GetTxDataRegEmptyIntCmd(UART4_BASE) && UART_HAL_IsTxDataRegEmpty(UART4_BASE), UART_HAL_GetRxDataRegFullIntCmd(UART4_BASE) && UART_HAL_IsRxDataRegFull(UART4_BASE), 4);
}
#endif

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id) {
    irq_handler = handler;
    serial_irq_ids[obj->serial.instance] = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable) {
    if (enable) {
        switch (irq) {
            case RxIrq: UART_HAL_SetRxDataRegFullIntCmd(obj->serial.address, true); break;
            case TxIrq: UART_HAL_SetTxDataRegEmptyIntCmd(obj->serial.address, true); break;
        }
        vIRQ_SetVector(obj->serial.irq_number, obj->serial.vector_cur);
        vIRQ_EnableIRQ(obj->serial.irq_number);

    } else { // disable
        int all_disabled = 0;
        SerialIrq other_irq = (irq == RxIrq) ? (TxIrq) : (RxIrq);
        switch (irq) {
            case RxIrq: UART_HAL_SetRxDataRegFullIntCmd(obj->serial.address, false); break;
            case TxIrq: UART_HAL_SetTxDataRegEmptyIntCmd(obj->serial.address, false); break;
        }
        switch (other_irq) {
            case RxIrq: all_disabled = UART_HAL_GetRxDataRegFullIntCmd(obj->serial.address) == 0; break;
            case TxIrq: all_disabled = UART_HAL_GetTxDataRegEmptyIntCmd(obj->serial.address) == 0; break;
        }
        if (all_disabled)
            vIRQ_DisableIRQ(obj->serial.irq_number);
    }
}

int serial_getc(serial_t *obj) {
    while (!serial_readable(obj));
    uint8_t data;
    UART_HAL_Getchar(obj->serial.address, &data);

    return data;
}

void serial_putc(serial_t *obj, int c) {
    while (!serial_writable(obj));
    UART_HAL_Putchar(obj->serial.address, (uint8_t)c);
}

int serial_readable(serial_t *obj) {
    if (UART_HAL_GetStatusFlag(obj->serial.address, kUartRxOverrun))
        UART_HAL_ClearStatusFlag(obj->serial.address, kUartRxOverrun);
    return UART_HAL_IsRxDataRegFull(obj->serial.address);
}

int serial_writable(serial_t *obj) {
    if (UART_HAL_GetStatusFlag(obj->serial.address, kUartRxOverrun))
        UART_HAL_ClearStatusFlag(obj->serial.address, kUartRxOverrun);

    return UART_HAL_IsTxDataRegEmpty(obj->serial.address);
}

void serial_pinout_tx(PinName tx) {
    pinmap_pinout(tx, PinMap_UART_TX);
}

void serial_break_set(serial_t *obj) {
    UART_HAL_SetBreakCharCmd(obj->serial.address, true);
}

void serial_break_clear(serial_t *obj) {
    UART_HAL_SetBreakCharCmd(obj->serial.address, false);
}

// Asynch

static void serial_rx_enable_event(serial_t *obj, int event, uint8_t enable)
{
    // serial_enable_event(obj, event, enable);
    if (event & SERIAL_EVENT_RX_OVERRUN_ERROR) {
        UART_HAL_SetIntMode(obj->serial.address, kUartIntRxOverrun, enable);
    }
    if (event & SERIAL_EVENT_RX_FRAMING_ERROR) {
        UART_HAL_SetIntMode(obj->serial.address, kUartIntFrameErrFlag, enable);
    }
    if (event & SERIAL_EVENT_RX_PARITY_ERROR) {
        UART_HAL_SetIntMode(obj->serial.address, kUartIntParityErrFlag, enable);
    }
    if (event & SERIAL_EVENT_RX_OVERFLOW) {
        if (enable) {
            HW_UART_CFIFO_SET(obj->serial.address, BM_UART_SFIFO_RXOF);
        } else {
            HW_UART_CFIFO_CLR(obj->serial.address, BM_UART_SFIFO_RXOF);
        }
    }
}

static uint8_t serial_is_tx_complete(serial_t *obj)
{
    uint8_t complete = 0;
    if ((obj->tx_buff.pos == obj->tx_buff.length) && (UART_HAL_GetTxDatawordCountInFifo(obj->serial.address) == 0)) {
        complete = 1;
    }
    return complete;
}

static uint32_t serial_rx_error_event_check(serial_t *obj)
{
    uint32_t event = obj->serial.event;
    uint32_t result = 0;
    uint8_t overrun = UART_HAL_GetStatusFlag(obj->serial.address, kUartRxOverrun);
    uint8_t framing = UART_HAL_GetStatusFlag(obj->serial.address, kUartFrameErr);
    uint8_t parity = UART_HAL_GetStatusFlag(obj->serial.address, kUartParityErr);
    uint8_t overflow = (HW_UART_SFIFO_RD(obj->serial.address) >> (uint8_t)(BP_UART_SFIFO_RXOF)) & 1U;

    if ((event & SERIAL_EVENT_RX_OVERRUN_ERROR) && overrun) {
        result |= SERIAL_EVENT_RX_OVERRUN_ERROR;
        UART_HAL_ClearStatusFlag(obj->serial.address, kUartRxOverrun);
    }
    if ((event & SERIAL_EVENT_RX_FRAMING_ERROR) && framing) {
        result |= SERIAL_EVENT_RX_FRAMING_ERROR;
        UART_HAL_ClearStatusFlag(obj->serial.address, kUartFrameErr);
    }
    if ((event & SERIAL_EVENT_RX_PARITY_ERROR) && parity) {
        result |= SERIAL_EVENT_RX_PARITY_ERROR;
        UART_HAL_ClearStatusFlag(obj->serial.address, kUartParityErr);
    }
    if ((event & SERIAL_EVENT_RX_OVERFLOW) && overflow) {
        result |= SERIAL_EVENT_RX_OVERFLOW;
        HW_UART_SFIFO_SET(obj->serial.address, BM_UART_SFIFO_RXOF);
    }

    return result;
}

static uint32_t serial_rx_event_check(serial_t *obj)
{
    uint32_t event = obj->serial.event;
    uint32_t result = 0;
    uint8_t in_fifo = UART_HAL_GetRxDatawordCountInFifo(obj->serial.address);
    uint8_t buffer_full = (obj->rx_buff.pos == obj->rx_buff.length);

    if ((event & SERIAL_EVENT_RX_COMPLETE) && buffer_full && (in_fifo == 0)) {
        result |= SERIAL_EVENT_RX_COMPLETE;
    }
    if ((event & SERIAL_EVENT_RX_CHARACTER_MATCH) && (obj->char_match != SERIAL_RESERVED_CHAR_MATCH) &&
        (obj->char_found)) {
        result |= SERIAL_EVENT_RX_CHARACTER_MATCH;
        obj->char_found = 0;
    }

    return result;
}

static void serial_buffer_tx_write(serial_t *obj)
{
    uint16_t data;
    if (obj->serial.databits == 8) {
        uint8_t *tx = (uint8_t *)(obj->tx_buff.buffer);
        data = tx[obj->tx_buff.pos];
    } else {
        uint16_t *tx = (uint16_t *)(obj->tx_buff.buffer);
        data = tx[obj->tx_buff.pos];
    }

    if (obj->serial.databits < 9) {
        UART_HAL_Putchar(obj->serial.address, (uint8_t)data);
    } else {
        UART_HAL_Putchar9(obj->serial.address, data);
    }
    obj->tx_buff.pos++;
}

static int serial_buffer_rx_read(serial_t *obj)
{
    uint16_t data = 0;
    if (obj->serial.databits < 9) {
        UART_HAL_Getchar(obj->serial.address, (uint8_t *)&data);
    } else {
        UART_HAL_Getchar9(obj->serial.address, &data);
    }

    int character;
    if (obj->serial.databits == 8) {
        uint8_t *rx = (uint8_t *)(obj->rx_buff.buffer);
        rx[obj->rx_buff.pos] = data;
        character = data;
    } else {
        uint16_t *rx = (uint16_t *)(obj->rx_buff.buffer);
        rx[obj->rx_buff.pos] = data;
        character = data;
    }
    obj->rx_buff.pos++;
    return character;
}

static int serial_write_asynch(serial_t *obj)
{
    int ndata = 0;
    uint8_t empty_count = obj->serial.entry_count - UART_HAL_GetTxDatawordCountInFifo(obj->serial.address);
    while ((obj->tx_buff.pos < obj->tx_buff.length) && (empty_count--)) {
        serial_buffer_tx_write(obj);
        ndata++;
    }
    return ndata;
}

static int serial_read_asynch(serial_t *obj)
{
    int ndata = 0;
    while ((obj->rx_buff.pos < obj->rx_buff.length) && !(UART_HAL_IsRxFifoEmpty(obj->serial.address))) {
        serial_buffer_rx_read(obj);
        ndata++;
    }
    return ndata;
}

static int serial_read_match_asynch(serial_t *obj)
{
    int ndata = 0;
    while ((obj->rx_buff.pos < obj->rx_buff.length) && !(UART_HAL_IsRxFifoEmpty(obj->serial.address))) {
        int character = serial_buffer_rx_read(obj);
        ndata++;
        if ((character == obj->char_match) && (obj->serial.event & SERIAL_EVENT_RX_CHARACTER_MATCH)) {
            obj->char_found = 1;
            break;
        }
    }
    return ndata;

}

static void serial_tx_buffer_set(serial_t *obj, void *tx, size_t length, uint8_t width)
{
    obj->tx_buff.buffer = tx;
    obj->tx_buff.length = length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = width;
}

static void serial_rx_buffer_set(serial_t *obj, void *rx, size_t length, uint8_t width)
{
    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = length;
    obj->rx_buff.pos = 0;
    obj->rx_buff.width = width;
}

uint8_t serial_tx_active(serial_t *obj)
{
    uint8_t result;
    if (obj->tx_buff.buffer && (obj->tx_buff.pos < obj->tx_buff.length)) {
        result = 1;
    } else {
        result = (UART_HAL_GetIntMode(obj->serial.address, kUartIntTxDataRegEmpty) ||
                 (UART_HAL_GetTxDatawordCountInFifo(obj->serial.address) != 0));
    }
    return result;
}

uint8_t serial_rx_active(serial_t *obj)
{
    uint8_t result;
    if (obj->rx_buff.buffer && (obj->rx_buff.pos < obj->rx_buff.length)) {
        result = 1;
    } else {
        result = (UART_HAL_GetIntMode(obj->serial.address, kUartIntRxDataRegFull) ||
                 (UART_HAL_GetRxDatawordCountInFifo(obj->serial.address) != 0));
    }
    return result;
}

int serial_irq_handler_asynch(serial_t *obj)
{
    int event = 0;
    if (UART_HAL_IsReceiverEnabled(obj->serial.address) && UART_HAL_GetRxDataRegFullIntCmd(obj->serial.address)) {
        event = serial_rx_error_event_check(obj);
        if (obj->char_match == SERIAL_RESERVED_CHAR_MATCH) {
            serial_read_asynch(obj);
        } else {
            serial_read_match_asynch(obj);
        }

        event |= serial_rx_event_check(obj);
        if (event) {
            serial_rx_abort_asynch(obj);
        }
    }

    if (UART_HAL_IsTransmitterEnabled(obj->serial.address) && UART_HAL_GetTxDataRegEmptyIntCmd(obj->serial.address) &&
        UART_HAL_IsTxDataRegEmpty(obj->serial.address)) {
        serial_write_asynch(obj);
        if (serial_is_tx_complete(obj)) {
            if (obj->serial.event & SERIAL_EVENT_TX_COMPLETE) {
                event |= SERIAL_EVENT_TX_COMPLETE;
            }
            serial_tx_abort_asynch(obj);
        }
    }
    return event;
}

static void serial_interrupt_vector_set(serial_t *obj, uint32_t handler)
{
    // RX and TX share the same vector - only one handler is allowed
    if (obj->serial.vector_cur != handler) {
        obj->serial.vector_prev = obj->serial.vector_cur;
        obj->serial.vector_cur = handler;
    }
}

static void serial_tx_enable_interrupt(serial_t *obj, uint32_t handler, uint8_t enable)
{
    serial_interrupt_vector_set(obj, handler);
    serial_irq_set(obj, (SerialIrq)1, enable);
}

static void serial_rx_enable_interrupt(serial_t *obj, uint32_t handler, uint8_t enable)
{
    serial_interrupt_vector_set(obj, handler);
    serial_irq_set(obj, (SerialIrq)0, enable);
}

int serial_tx_asynch(serial_t *obj, void *tx, size_t tx_length, uint8_t tx_width, uint32_t handler, uint32_t event, DMAUsage hint)
{
    (void)hint; //Only IRQ is supported
    obj->serial.event &= ~SERIAL_EVENT_TX_MASK;
    obj->serial.event |= event;
    serial_tx_buffer_set(obj, tx, tx_length, tx_width);
    UART_HAL_DisableTransmitter(obj->serial.address);
    UART_HAL_FlushTxFifo(obj->serial.address);
    UART_HAL_EnableTransmitter(obj->serial.address);
    int ndata = serial_write_asynch(obj);
    serial_tx_enable_interrupt(obj, handler, true);
    return ndata;
}

void serial_rx_asynch(serial_t *obj, void *rx, size_t rx_length, uint8_t rx_width, uint32_t handler, uint32_t event, uint8_t char_match, DMAUsage hint)
{
    (void)hint;
    obj->serial.event &= ~SERIAL_EVENT_RX_MASK;
    obj->serial.event |= event;
    serial_rx_enable_event(obj, event, true);
    serial_rx_buffer_set(obj, rx, rx_length, rx_width);
    serial_rx_set_char_match(obj, char_match);
    UART_HAL_DisableReceiver(obj->serial.address);
    UART_HAL_FlushRxFifo(obj->serial.address);
    UART_HAL_EnableReceiver(obj->serial.address);
    serial_rx_enable_interrupt(obj, handler, true);
}

void serial_tx_abort_asynch(serial_t *obj)
{
    serial_irq_set(obj, (SerialIrq)1, false);
    UART_HAL_DisableTransmitter(obj->serial.address);
    UART_HAL_FlushTxFifo(obj->serial.address);
}

void serial_rx_abort_asynch(serial_t *obj)
{
    serial_irq_set(obj, (SerialIrq)0, false);
    UART_HAL_DisableReceiver(obj->serial.address);
    UART_HAL_FlushRxFifo(obj->serial.address);
}

static void serial_rx_set_char_match(serial_t *obj, uint8_t char_match)
{
    if (char_match != SERIAL_RESERVED_CHAR_MATCH) {
        obj->char_match = char_match;
    }
}

#endif
