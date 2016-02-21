/* mbed Microcontroller Library
 * Copyright (c) 2013-2015 ARM Limited
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
#include <math.h>
#include <stdio.h>
#include "mbed-drivers/mbed_assert.h"

#include "mbed-hal/spi_api.h"

#if DEVICE_SPI

#include "cmsis.h"
#include "mbed-hal/pinmap.h"
#include "mbed-drivers/mbed_error.h"
#include "fsl_clock_manager.h"
#include "fsl_dspi_hal.h"
#include "PeripheralPins.h"
#include "uvisor-lib/uvisor-lib.h"

static void spi_enable_vector_interrupt(spi_t *obj, uint32_t handler, uint8_t enable);
static void spi_master_enable_interrupt(spi_t *obj, uint8_t enable);
static uint32_t spi_master_write_asynch(spi_t *obj, uint32_t TxLimit);
static uint32_t spi_master_read_asynch(spi_t *obj);
static uint32_t spi_event_check(spi_t *obj);
static void spi_buffer_tx_write(spi_t *obj);
static void spi_buffer_rx_read(spi_t *obj);
static void spi_enable_event_flags(spi_t *obj, uint32_t event, uint8_t enable);
static void spi_buffer_set(spi_t *obj, void *tx, size_t tx_length, void *rx, size_t rx_length);

#define SPI_TX_FIFO_SIZE (4)
#define SPI_RX_FIFO_SIZE (4)

#define SPI_ASYNCH_INITIAL_TX (min(SPI_TX_FIFO_SIZE+1,SPI_RX_FIFO_SIZE))

#define max(a,b)\
    ((a)>(b)?(a):(b))
#define min(a,b)\
    ((a)<(b)?(a):(b))


uint8_t spi_get_module(spi_t *obj)
{
    return obj->spi.instance;
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk) {
    // determine the SPI to use
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);

    obj->spi.instance = pinmap_merge(spi_data, spi_sclk);
    MBED_ASSERT((int)obj->spi.instance != NC);

    CLOCK_SYS_EnableSpiClock(obj->spi.instance);
    uint32_t spi_address[] = SPI_BASE_ADDRS;
    obj->spi.address = spi_address[obj->spi.instance];
    DSPI_HAL_Init(obj->spi.address);
    DSPI_HAL_Disable(obj->spi.address);
    // set default format and frequency
    spi_format(obj, 8, 0, SPI_MSB);  // 8 bits, mode 0
    DSPI_HAL_SetDelay(obj->spi.address, kDspiCtar0, 0, 0, kDspiPcsToSck);
    spi_frequency(obj, 1000000);

    DSPI_HAL_Enable(obj->spi.address);
    DSPI_HAL_StartTransfer(obj->spi.address);

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
}

void spi_free(spi_t *obj) {
    (void) obj;
    // [TODO]
}
void spi_format(spi_t *obj, int bits, int mode, spi_bitorder_t order) {
    MBED_ASSERT(bits > 3 && bits < 17); // for master max 16
    dspi_data_format_config_t config = {0};
    config.bitsPerFrame = (uint32_t)bits;
    obj->spi.bits = bits;
    config.clkPolarity = (mode & 0x2) ? kDspiClockPolarity_ActiveLow : kDspiClockPolarity_ActiveHigh;
    config.clkPhase = (mode & 0x1) ? kDspiClockPhase_SecondEdge : kDspiClockPhase_FirstEdge;
    if (order == SPI_MSB) {
        config.direction = kDspiMsbFirst;
    } else {
        config.direction = kDspiLsbFirst;
    }
    dspi_status_t result = DSPI_HAL_SetDataFormat(obj->spi.address, kDspiCtar0, &config);
    if (result != kStatus_DSPI_Success) {
        error("Failed to configure SPI data format");
    }

    DSPI_HAL_SetMasterSlaveMode(obj->spi.address, kDspiMaster);
}

void spi_frequency(spi_t *obj, int hz) {
    uint32_t busClock;
    CLOCK_SYS_GetFreq(kBusClock, &busClock);
    DSPI_HAL_SetBaudRate(obj->spi.address, kDspiCtar0, (uint32_t)hz, busClock);
    DSPI_HAL_CalculateDelay(obj->spi.address, kDspiCtar0, kDspiAfterTransfer, busClock, 1000000000/(2*hz));
}

static inline int spi_writeable(spi_t * obj) {
    return DSPI_HAL_GetStatusFlag(obj->spi.address, kDspiTxFifoFillRequest);
}

static inline int spi_readable(spi_t * obj) {
    return DSPI_HAL_GetStatusFlag(obj->spi.address, kDspiRxFifoDrainRequest);
}

int spi_master_write(spi_t *obj, int value) {

    // wait tx buffer empty
    while(!spi_writeable(obj));
    dspi_command_config_t command = {0};
    command.isEndOfQueue = true;
    command.isChipSelectContinuous = 0;
    DSPI_HAL_WriteDataMastermode(obj->spi.address, &command, (uint16_t)value);
    DSPI_HAL_ClearStatusFlag(obj->spi.address, kDspiTxFifoFillRequest);

    // wait rx buffer full
    while (!spi_readable(obj));
    DSPI_HAL_ClearStatusFlag(obj->spi.address, kDspiRxFifoDrainRequest);
    return DSPI_HAL_ReadData(obj->spi.address) & 0xff;
}

static void spi_enable_event_flags(spi_t *obj, uint32_t event, uint8_t enable)
{
    obj->spi.event = event;
    if (event & SPI_EVENT_RX_OVERFLOW) {
        BW_SPI_RSER_RFOF_RE(obj->spi.address, enable);
    }
}

static void spi_enable_vector_interrupt(spi_t *obj, uint32_t handler, uint8_t enable)
{
    IRQn_Type spi_irq[] = SPI_IRQS;
    if (enable) {
        vIRQ_SetVector(spi_irq[obj->spi.instance], handler);
        vIRQ_EnableIRQ(spi_irq[obj->spi.instance]);
    } else {
        vIRQ_SetVector(spi_irq[obj->spi.instance], handler);
        vIRQ_DisableIRQ(spi_irq[obj->spi.instance]);
    }
}

static void spi_master_enable_interrupt(spi_t *obj, uint8_t enable)
{
    DSPI_HAL_SetRxFifoDrainDmaIntMode(obj->spi.address, kDspiGenerateIntReq, enable);
}

void spi_master_transfer(spi_t *obj, void *tx, size_t tx_length, void *rx, size_t rx_length, uint32_t handler, uint32_t event, DMAUsage hint)
{
    (void)hint;
    MBED_ASSERT(hint == DMA_USAGE_NEVER); // only IRQ supported
    spi_enable_event_flags(obj, event, true);

    // corrections in lengths for 16bit transfers
    if (obj->spi.bits > 8) {
        MBED_ASSERT(((tx_length % 2) == 0) && ((rx_length % 2) == 0)); // alignment check for 16bit transfer
        tx_length /= 2;
        rx_length /= 2;
    }
    spi_buffer_set(obj, tx, tx_length, rx, rx_length);
    // Clear the FIFO
    DSPI_HAL_SetFlushFifoCmd(obj->spi.address,true,true);
    // Enable the FIFO
    DSPI_HAL_SetFifoCmd(obj->spi.address, true, true);
    // Start writing to the SPI peripheral
    spi_master_write_asynch(obj,SPI_ASYNCH_INITIAL_TX);
    // use IRQ
    spi_master_enable_interrupt(obj, true);
    spi_enable_vector_interrupt(obj, handler, true);
}

static uint32_t spi_event_check(spi_t *obj)
{
    uint32_t event = 0;
    // Get the TX FIFO underflow flag
    uint8_t U = DSPI_HAL_GetStatusFlag(obj->spi.address,kDspiTxFifoUnderflow);
    // Get the RX FIFO overflow flag
    uint8_t O = DSPI_HAL_GetStatusFlag(obj->spi.address,kDspiRxFifoOverflow);
    // Get the RX FIFO drain request flag
    uint8_t TXCTR = DSPI_HAL_GetFifoCountOrPtr(obj->spi.address, kDspiTxFifoCounter);
    // Get the number of words in the RX FIFO
    uint8_t RXCTR = DSPI_HAL_GetFifoCountOrPtr(obj->spi.address, kDspiRxFifoCounter);

    // The transfer is only complete if the TX buffer has been sent, the RX buffer has been filled, and there are no
    // values in the TX FIFO or RX FIFO
    if ((RXCTR == 0) && (TXCTR == 0) &&
        (obj->rx_buff.pos >= obj->rx_buff.length) &&
        (obj->tx_buff.pos >= obj->tx_buff.length)) {
        if (obj->spi.event & SPI_EVENT_COMPLETE) {
            event |= SPI_EVENT_COMPLETE;
        }
        event |= SPI_EVENT_INTERNAL_TRANSFER_COMPLETE; // process another transactions
    }
    // Signal an RX overflow event
    if ((obj->spi.event & SPI_EVENT_RX_OVERFLOW) && O) {
        event |= SPI_EVENT_RX_OVERFLOW;
    }
    // Signal a TX underflow event
    if ((obj->spi.event & SPI_EVENT_ERROR) && U) {
        event |= SPI_EVENT_ERROR;
    }

    return event;
}

// Write a value from the TX buffer to the TX FIFO
static void spi_buffer_tx_write(spi_t *obj)
{
    int data = SPI_FILL_WORD;
    dspi_command_config_t command = {0};
    command.isEndOfQueue = false;
    // TODO: This may be wrong
    command.isChipSelectContinuous = 0;

    if ((obj->tx_buff.buffer) && (obj->tx_buff.pos < obj->tx_buff.length)) {
        // Load the data as either an 8-bit value or a 16-bit one
        if (obj->spi.bits <= 8) {
            uint8_t *tx = (uint8_t *)(obj->tx_buff.buffer);
            data = tx[obj->tx_buff.pos];
        } else if (obj->spi.bits <= 16) {
            uint16_t *tx = (uint16_t *)(obj->tx_buff.buffer);
            data = tx[obj->tx_buff.pos];
        }
        // Increment the buffer position
    }
    obj->tx_buff.pos++;
    // Send the data
    DSPI_HAL_WriteDataMastermode(obj->spi.address, &command, (uint16_t)data);
    // Clear the FIFO fill request
    DSPI_HAL_ClearStatusFlag(obj->spi.address, kDspiTxFifoFillRequest);
}

static void spi_buffer_rx_read(spi_t *obj)
{
    // Read a word from the RX FIFO
    int data = (int)DSPI_HAL_ReadData(obj->spi.address);

    // Disregard the word if the rx buffer is full or not present
    if (obj->rx_buff.buffer && obj->rx_buff.pos < obj->rx_buff.length) {
        if (obj->spi.bits <= 8) {
            // store the word to the rx buffer
            uint8_t *rx = (uint8_t *)(obj->rx_buff.buffer);
            rx[obj->rx_buff.pos] = data;
        } else if (obj->spi.bits <= 16) {
            uint16_t *rx = (uint16_t *)(obj->rx_buff.buffer);
            rx[obj->rx_buff.pos] = data;
        }
    }
    // advance the buffer position
    obj->rx_buff.pos++;
    // clear the RX FIFO drain request
    DSPI_HAL_ClearStatusFlag(obj->spi.address, kDspiRxFifoDrainRequest);
}

/**
 * Send words from the SPI TX buffer until the send limit is reached or the TX FIFO is full
 * TxLimit is provided to ensure that the number of SPI frames (words) in flight can be managed.
 * @param[in] obj     The SPI object on which to operate
 * @param[in] TxLimit The maximum number of words to send
 * @return The number of SPI frames that have been transfered
 */
static uint32_t spi_master_write_asynch(spi_t *obj, uint32_t TxLimit)
{
    uint32_t ndata = 0;
    // Determine the number of frames to send
    uint32_t txRemaining = obj->tx_buff.length - min(obj->tx_buff.pos, obj->tx_buff.length);
    uint32_t rxRemaining = obj->rx_buff.length - min(obj->tx_buff.pos, obj->rx_buff.length);
    uint32_t maxTx = max(txRemaining, rxRemaining);
    maxTx = min(maxTx, TxLimit);
    // Send words until the FIFO is full or the send limit is reached
    while ((ndata < maxTx) && DSPI_HAL_GetStatusFlag(obj->spi.address, kDspiTxFifoFillRequest)) {
        spi_buffer_tx_write(obj);
        ndata++;
    }
    //Return the number of frames that have been sent
    return ndata;
}

/**
 * Read SPI frames out of the RX FIFO
 * Continues reading frames out of the RX FIFO until the following condition is met:
 * o There are no more frames in the FIFO
 * OR BOTH OF:
 * o At least as many frames as the TX buffer have been received
 * o At least as many frames as the RX buffer have been received
 * This way, RX overflows are not generated when the TX buffer size exceeds the RX buffer size
 * @param[in] obj The SPI object on which to operate
 * @return Returns the number of frames extracted from the RX FIFO
 */
static uint32_t spi_master_read_asynch(spi_t *obj)
{
    uint32_t ndata = 0;
    // Calculate the maximum number of frames to receive
    uint32_t txRemaining = obj->tx_buff.length - min(obj->rx_buff.pos, obj->tx_buff.length);
    uint32_t rxRemaining = obj->rx_buff.length - min(obj->rx_buff.pos, obj->rx_buff.length);
    uint32_t maxRx = max(txRemaining, rxRemaining);
    // Receive frames until the maximum is reached or the RX FIFO is empty
    while ((ndata < maxRx) && DSPI_HAL_GetStatusFlag(obj->spi.address, kDspiRxFifoDrainRequest)) {
        spi_buffer_rx_read(obj);
        ndata++;
    }
    // Return the number of frames received
    return ndata;
}

/**
 * Abort an SPI transfer
 * This is a helper function for event handling. When any of the events listed occurs, the HAL will abort any ongoing
 * transfers
 * @param[in] obj The SPI peripheral to stop
 */
void spi_abort_asynch(spi_t *obj)
{
    spi_enable_vector_interrupt(obj, 0, false);
    DSPI_HAL_SetFlushFifoCmd(obj->spi.address,true,true);
    DSPI_HAL_SetFifoCmd(obj->spi.address, false, false);
}

/**
 * Handle the SPI interrupt
 * Read frames until the RX FIFO is empty.  Write at most as many frames as were read.  This way,
 * it is unlikely that the RX FIFO will overflow.
 * @param[in] obj The SPI peripheral that generated the interrupt
 * @return
 */
uint32_t spi_irq_handler_asynch(spi_t *obj)
{
    uint32_t result = 0;
    // Read frames until the RX FIFO is empty
    uint32_t r = spi_master_read_asynch(obj);
    // Write at most the same number of frames as were received
    spi_master_write_asynch(obj, r);

    // Check for SPI events
    uint32_t event = spi_event_check(obj);
    if (event) {
        result = event;
        if (event & SPI_EVENT_COMPLETE) {
            // adjust buffer positions
            obj->tx_buff.pos = obj->tx_buff.length;
            obj->rx_buff.pos = obj->rx_buff.length;
        }
    }

    if (result) {
        spi_master_enable_interrupt(obj, false);
        DSPI_HAL_SetFlushFifoCmd(obj->spi.address,true,true);
    }
    return result;
}

uint8_t spi_active(spi_t *obj)
{
    /* Check if rx or tx buffers are set and check if any more bytes need to be transferred. */
    if ((obj->rx_buff.buffer && obj->rx_buff.pos < obj->rx_buff.length)
            || (obj->tx_buff.buffer && obj->tx_buff.pos < obj->tx_buff.length) ){
        return 1;
    } else  {
        // interrupts are disabled, all transaction have been completed
        // TODO: checking rx fifo, it reports data eventhough RFDF is not set
        return DSPI_HAL_GetIntMode(obj->spi.address, kDspiRxFifoDrainRequest);
    }

}

static void spi_buffer_set(spi_t *obj, void *tx, size_t tx_length, void *rx, size_t rx_length)
{
    obj->tx_buff.buffer = tx;
    obj->tx_buff.length = tx_length;
    obj->tx_buff.pos = 0;
    obj->tx_buff.width = 0;
    obj->rx_buff.buffer = rx;
    obj->rx_buff.length = rx_length;
    obj->rx_buff.pos = 0;
    obj->rx_buff.width = 0;
}

#endif
