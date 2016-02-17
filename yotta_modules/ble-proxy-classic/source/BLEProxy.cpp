/* mbed
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

#include "ble-proxy/BLEProxy.h"

#include "ble/BLE.h"
#include "ble-blocktransfer/BlockTransferService.h"

#include "cborg/Cbor.h"

#include "mbed.h"

#include <vector>
#include <queue>

const char DEVICE_NAME[] = "Proxy Client";
const UUID uuid(0xFFFE);

#ifndef CFG_BLE_TX_POWER_LEVEL
#define CFG_BLE_TX_POWER_LEVEL 0
#endif

#define VERBOSE_DEBUG_OUTPUT 0
#define BEACON_DEBUG_OUTPUT 0

#if 1
#define BLE_DEBUG(...) { printf(__VA_ARGS__); }
#else
#define BLE_DEBUG(...) /* nothing */
#endif

/*****************************************************************************/

#define MAX_HEADER_SEND_SIZE 9

static BLE ble;
static BlockTransferService bts;

static bool initialized = false;
static bool isConnected = false;
static Gap::Handle_t connectionHandle;
static Gap::ConnectionParams_t custom;
static Gap::DisconnectionReason_t reason = (Gap::DisconnectionReason_t) 0;

typedef enum
{
    STATE_OFF,
    STATE_READY,
    STATE_CONTROL,
    STATE_MESSAGE
} state_t;

static volatile state_t state = STATE_OFF;

// send queue and pointer
static std::queue<SharedPointer<Block> > sendQueue;
static SharedPointer<Block> sendBlock;

// handle counter
static std::vector<bool> handleTable;

//
static void sendDone(void);
static void receiveDone(SharedPointer<Block>&);
static void processQueue(void);
static void whenConnected(const Gap::ConnectionCallbackParams_t* params);
static void whenDisconnected(const Gap::DisconnectionCallbackParams_t* params);

// variables to the current message in the receive buffer
static uint32_t messageType = BLEProxy::MessageTypeInvalid;
static uint32_t messageHandle = 0;
static const uint8_t* messagePayload = NULL;
static uint32_t messageLength = 0;
static SharedPointer<Block> messageBlock;

// receive
static FunctionPointer receiveCallback;

//
#if !defined(YOTTA_MINAR_VERSION_STRING)
static bool processQueueFlag = false;
static bool receiveCallbackFlag = false;
static volatile bool sendDoneFlag = false;
static volatile bool updateConnectionParametersFlag = false;
#endif

/*****************************************************************************/
/* BLE                                                                       */
/*****************************************************************************/

#if BEACON_DEBUG_OUTPUT
void updateBeacon()
{
    ble.gap().clearAdvertisingPayload();

    char name[20];
    int size = snprintf(name, 20, "mbed: %lu %lu", NRF_CLOCK->LFCLKSTAT, NRF_CLOCK->HFCLKSTAT);

    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED|GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME, (const uint8_t *) name, size);
    ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, uuid.getBaseUUID(), uuid.getLen());

    ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
    ble.gap().setAdvertisingInterval(1285);

    // ble setup complete - start advertising
    ble.gap().startAdvertising();
}
#endif

void updateConnectionParameters()
{
    // update connection to the prefered parameters
    ble_error_t error = ble.gap().updateConnectionParams(connectionHandle, &custom);
    (void) error;
    BLE_DEBUG("ble::proxy: update %d\r\n", error);
}

/*
    Functions called when BLE device has been connected.
*/
void whenConnected(const Gap::ConnectionCallbackParams_t* params)
{
    BLE_DEBUG("ble::proxy: Connected: %d %d %d %d\r\n", params->connectionParams->minConnectionInterval,
                                                      params->connectionParams->maxConnectionInterval,
                                                      params->connectionParams->slaveLatency,
                                                      params->connectionParams->connectionSupervisionTimeout);

    connectionHandle = params->handle;
    isConnected = true;
    state = STATE_READY;

#if defined(YOTTA_MINAR_VERSION_STRING)
    minar::Scheduler::postCallback(updateConnectionParameters)
        .delay(minar::milliseconds(1 * 1000));
#else
        updateConnectionParametersFlag = true;
#endif
}

/*
    Function called when BLE device has been disconnected.
*/
void whenDisconnected(const Gap::DisconnectionCallbackParams_t* params)
{
    BLE_DEBUG("ble::proxy: Disconnected!\r\n");
    BLE_DEBUG("ble::proxy: Restarting the advertising process\r\n");

    (void) params;
    isConnected = false;
    state = STATE_OFF;
    reason = params->reason;

    /* clean up queue */
    while (sendQueue.size() > 0)
    {
        sendQueue.pop();
    }

    /* close all handles */
    for (std::size_t idx = 0; idx < handleTable.size(); idx++)
    {
        handleTable.at(idx) = false;
    }

    /* start advertising */
#if BEACON_DEBUG_OUTPUT
    updateBeacon();
#else
    ble.gap().startAdvertising();
#endif
}


/*****************************************************************************/
/* Block Transfer Server                                                     */
/*****************************************************************************/

/*
    Function called when device receives a read request over BLE.
*/
SharedPointer<Block> blockServerReadHandler(uint32_t offset)
{
    (void) offset;

    BLE_DEBUG("ble::proxy: block read: %4lu %6lu\r\n", sendBlock->getLength(), offset);

#if VERBOSE_DEBUG_OUTPUT
    for (std::size_t idx = 0; idx < sendBlock->getLength(); idx++)
    {
        BLE_DEBUG("%02X", sendBlock->at(idx));
    }
    BLE_DEBUG("\r\n");
#endif

    return sendBlock;
}

/*
    Function called when data has been written over BLE.
*/
void blockServerWriteHandler(SharedPointer<Block> block)
{
    BLE_DEBUG("ble::proxy: block write: length: %4lu offset: %6lu\r\n", block->getLength(), block->getOffset());

#if VERBOSE_DEBUG_OUTPUT
    for (std::size_t idx = 0; idx < block->getLength(); idx++)
    {
        BLE_DEBUG("%02X", block->at(idx));
    }
    BLE_DEBUG("\r\n");
#endif

    // process received data immediately to avoid buffer overflow
    receiveDone(block);
}

static void blockServerReadDoneHandler()
{
    BLE_DEBUG("ble::proxy: read done\r\n");

#if defined(YOTTA_MINAR_VERSION_STRING)
    minar::Scheduler::postCallback(sendDone);
#else
    sendDoneFlag = true;
#endif
}

/*****************************************************************************/
/* Helper functions                                                          */
/*****************************************************************************/
static const char* typeToString(BLEProxy::ProtocolType_t type)
{
    switch(type)
    {
        case BLEProxy::ProtocolTypeTCP:
                                    return "tcp";
        case BLEProxy::ProtocolTypeUDP:
                                    return "udp";
        default:
                                    return "";
    }
}


static void actualBlockTransferSendFunction()
{
    // signal data is ready to be read
    bts.updateCharacteristicValue((uint8_t*) "", 0);
}

static void sendDone()
{
    // avoid reading invalid block
    sendBlock = SharedPointer<Block>();

    if (sendQueue.size() > 0)
    {
        // remove message from queue
        sendQueue.pop();
    }

    BLE_DEBUG("send done: %d\r\n", sendQueue.size());

    state = STATE_READY;

    processQueue();
}

static void receiveDone(SharedPointer<Block>& block)
{
    BLE_DEBUG("receive done\r\n");

    /***************************************************************/
    /* Extract information from CBOR object                        */
    /***************************************************************/

    // get pointer and length from block
    BlockStatic* blockPointer = (BlockStatic*) block.get();
    uint8_t* pointer = blockPointer->getData();
    uint32_t length = blockPointer->getLength();

    // decode cbor
    Cborg object(pointer, length);

    bool result = true;
    result &= object.at(0).getUnsigned(&messageType);
    result &= object.at(1).getUnsigned(&messageHandle);
    result &= object.at(2).getBytes(&messagePayload, &messageLength);

    // keep memory block alive if payload decoded to a message
    if (result)
    {
        messageBlock = block;
    }

    /***************************************************************/
    /* Process message type for given handle                       */
    /***************************************************************/
    switch(messageType)
    {
        case BLEProxy::MessageTypeOnOpen:
        case BLEProxy::MessageTypeOnClose:
                                            BLE_DEBUG("control done\r\n");
                                            break;

        case BLEProxy::MessageTypeOnMessage:
#if defined(YOTTA_MINAR_VERSION_STRING)
                                            minar::Scheduler::postCallback(receiveCallback);
#else
                                            receiveCallbackFlag = true;
#endif
                                            break;
        default:
                                            BLE_DEBUG("unknown message type\r\n");
                                            break;
    }
}

void processQueue()
{
    BLE_DEBUG("process queue: %d %d\r\n", state, sendQueue.size());

    if ((sendQueue.size() > 0) && bts.isReady())
    {
        if (state == STATE_READY)
        {
            state = STATE_MESSAGE;

            // set read block
            sendBlock = sendQueue.front();

            // send notification
            actualBlockTransferSendFunction();
        }
    }
}

/*****************************************************************************/
/* BLEProxy                                                                  */
/*****************************************************************************/
void BLEProxy::init()
{
    if (!initialized)
    {
        initialized = true;

        /* bluetooth le */
        ble.init();

        // connection status handlers
        ble.gap().onConnection(whenConnected);
        ble.gap().onDisconnection(whenDisconnected);

        // set preferred connection parameters
#if 1
        custom.minConnectionInterval = 80;          // 100 ms
        custom.maxConnectionInterval = 100;         // 125 ms
        custom.slaveLatency = 4;                    // 4 events
        custom.connectionSupervisionTimeout = 200;  // 2000 ms
#else
        custom.minConnectionInterval = 304;         // 380 ms
        custom.maxConnectionInterval = 320;         // 400 ms
        custom.slaveLatency = 4;                    // 4 events
        custom.connectionSupervisionTimeout = 600;  // 6000 ms
#endif

        ble_error_t error = ble.gap().setPreferredConnectionParams(&custom);
        (void) error;
        BLE_DEBUG("ble::proxy: set %d\r\n", error);

        /* construct advertising beacon */
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED|GapAdvertisingData::LE_GENERAL_DISCOVERABLE);
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::SHORTENED_LOCAL_NAME, (const uint8_t *) DEVICE_NAME, sizeof(DEVICE_NAME) - 1);
        ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, uuid.getBaseUUID(), uuid.getLen());
        ble.gap().accumulateAdvertisingPayloadTxPower(CFG_BLE_TX_POWER_LEVEL);

        ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED);
        ble.gap().setAdvertisingInterval(3 * 1285);

        // set TX power
        ble.gap().setTxPower(CFG_BLE_TX_POWER_LEVEL);

        // Apple uses device name instead of beacon name
        ble.gap().setDeviceName((const uint8_t*) DEVICE_NAME);

        /*************************************************************************/
        /*************************************************************************/
        bts.init(uuid, SecurityManager::SECURITY_MODE_ENCRYPTION_OPEN_LINK);

        // set callback functions
        bts.setWriteAuthorizationCallback(blockServerWriteHandler);
        bts.setReadAuthorizationCallback(blockServerReadHandler);
        bts.setReadDoneCallback(blockServerReadDoneHandler);

        // ble setup complete - start advertising
#if BEACON_DEBUG_OUTPUT
        updateBeacon();
#else
        ble.gap().startAdvertising();
#endif

        // initialize handle 0 to false
        handleTable.push_back(false);

        BLE_DEBUG("ble::proxy::init()\r\n");
    }
}

/*
    Open connection to host.
    host - IP or URL string
    port - port number
    type - connection enum type
*/
uint32_t BLEProxy::open(uint32_t handle, const char* host, uint16_t port, ProtocolType_t type)
{
    BLE_DEBUG("ble::proxy::open: %lu %s %u %d\r\n", handle, host, port, type);

    /*  Check handle
    */
    if (handle == 0)
    {
        handleTable.push_back(true);
        handle = handleTable.size() - 1;
    }
    else
    {
        handleTable.at(handle) = true;
    }

    /*  Construct temporary string with the host, port and connection type
        formatted into one URL: type://host:port
    */
    const char* typeString = typeToString(type);

    uint32_t payloadLength = strlen(typeString) + strlen(host) + 9;
    char payload[payloadLength];

    uint32_t actualLength = snprintf(payload, payloadLength, "%s://%s:%u", typeString, host, port);

    /*  CBOR encode temporary host string and copy it to the controlBuffer.
    */
    uint8_t* buffer = (uint8_t*) malloc(MAX_HEADER_SEND_SIZE + actualLength);

    Cbore encoder(buffer, MAX_HEADER_SEND_SIZE + actualLength);
    encoder.array(3)
        .item(BLEProxy::MessageTypeOpen)
        .item(handle)
        .item(payload, actualLength);

    /*  Add control message to queue
    */
    SharedPointer<Block> message(new BlockDynamic(buffer, encoder.getLength()));

    sendQueue.push(message);

#if VERBOSE_DEBUG_OUTPUT
    BLE_DEBUG("%p: ", buffer);

    for (std::size_t idx = 0; idx < message->getLength(); idx++)
    {
        BLE_DEBUG("%02X", message->at(idx));
    }
    BLE_DEBUG("\r\n");
#endif

    /*
    */
#if defined(YOTTA_MINAR_VERSION_STRING)
    minar::Scheduler::postCallback(processQueue);
#else
    processQueueFlag = true;
#endif

    return handle;
}

void BLEProxy::close(uint32_t handle)
{
    BLE_DEBUG("ble::proxy::close: %lu\r\n", handle);

    handleTable.at(handle) = false;
}

bool BLEProxy::handleIsValid(uint32_t handle)
{
    BLE_DEBUG("ble::proxy::handle %lu %u\r\n", handle, handleTable.size());

    if (handle < handleTable.size())
    {
        return handleTable.at(handle);
    }

    return false;
}

int32_t BLEProxy::send(uint32_t handle, const uint8_t* payload, uint32_t payloadLength)
{
    BLE_DEBUG("ble::proxy::send: %lu %lu %d\r\n", handle, payloadLength, sendQueue.size());

    /*  Check if handle is valid */
    if (handleIsValid(handle) == false)
    {
        return -1;
    }

    /*  CBOR encode message and copy it to the messageBuffer.
    */
    uint8_t* buffer = (uint8_t*) malloc(MAX_HEADER_SEND_SIZE + payloadLength);

    Cbore encoder(buffer, MAX_HEADER_SEND_SIZE + payloadLength);
    encoder.array(3)
        .item(BLEProxy::MessageTypeSend)
        .item(handle)
        .item(payload, payloadLength);

    /*  Add message to queue
    */
    SharedPointer<Block> message(new BlockDynamic(buffer, encoder.getLength()));

    sendQueue.push(message);

#if VERBOSE_DEBUG_OUTPUT
    for (std::size_t idx = 0; idx < message->getLength(); idx++)
    {
        BLE_DEBUG("%02X", message->at(idx));
    }
    BLE_DEBUG("\r\n");
#endif

    /**/
#if defined(YOTTA_MINAR_VERSION_STRING)
    minar::Scheduler::postCallback(processQueue);
#else
    processQueueFlag = true;
#endif

    return payloadLength;
}

int32_t BLEProxy::recv(uint32_t handle, uint8_t* buffer, uint32_t length)
{
    BLE_DEBUG("ble::proxy::recv: %lu ", handle);

    if (bts.isReady() == false)
    {
        BLE_DEBUG("\r\n");

        return -1;
    }

    if (messageHandle == handle)
    {
        uint32_t actualLength = (messageLength < length) ? messageLength : length;

        memcpy(buffer, messagePayload, actualLength);

        BLE_DEBUG("%p %ld\r\n", messagePayload, actualLength);

        // mark message as handled
        messageHandle = 0;
        messageLength = 0;
        messageBlock = SharedPointer<Block>();

        return actualLength;
    }
    else
    {
        BLE_DEBUG("empty\r\n");

        return 0;
    }
}

bool BLEProxy::isReady()
{
    return bts.isReady();
}

void BLEProxy::onReceive(void (*callback)(void))
{
    receiveCallback.attach(callback);
}

#if !defined(YOTTA_MINAR_VERSION_STRING)
void BLEProxy::callFromMainLoop(void)
{
    // loop until all flags have been handled or at most 10 times
    for (std::size_t count = 0; count < 10; count++)
    {
        if (updateConnectionParametersFlag)
        {
            updateConnectionParametersFlag = false;
            updateConnectionParameters();
        }
        else if (processQueueFlag)
        {
            processQueueFlag = false;
            processQueue();
        }
        else if (sendDoneFlag)
        {
            sendDoneFlag = false;
            sendDone();
        }
        else if (receiveCallbackFlag)
        {
            receiveCallbackFlag = false;
            receiveCallback();
        }
        else
        {
            break;
        }
    }
}
#endif



