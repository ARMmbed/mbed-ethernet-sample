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

#include "ble-proxy/UDPSocket.h"

#include "ble-proxy/BLEProxy.h"

#define VERBOSE_DEBUG_OUTPUT 0

#if 0
#define BLE_DEBUG(...) { printf(__VA_ARGS__); }
#else
#define BLE_DEBUG(...) /* nothing */
#endif

UDPSocket::UDPSocket()
    :   Socket()
{}

int UDPSocket::init(void)
{
    BLE_DEBUG("UDPSocket::init\r\n");
    BLEProxy::init();
    return 0;
}

// Server initialization
int UDPSocket::bind(int port)
{
    BLE_DEBUG("UDPSocket::bind\r\n");
    (void) port;
    // UDP server not supported
    return -1;
}

int UDPSocket::sendTo(Endpoint& remote, const char* packet, int length)
{
    BLE_DEBUG("UDPSocket::sendTo\r\n");

#if VERBOSE_DEBUG_OUTPUT
    for (std::size_t idx = 0; idx < length; idx++)
    {
        BLE_DEBUG("%02X", packet[idx]);
    }
    BLE_DEBUG("\r\n");
#endif

    if (BLEProxy::isReady() == false)
    {
        BLE_DEBUG("BLEProxy is busy\r\n");
        return -1;
    }

    /*  Register handle if it is not valid
    */
    uint32_t handle = remote.get_handle();

    if (BLEProxy::handleIsValid(handle) == false)
    {
        /* handle is invalid, open connection to renew handle
        */
        handle = BLEProxy::open(handle, remote.get_address(), remote.get_port(), BLEProxy::ProtocolTypeUDP);

        remote.set_handle(handle);
    }

    return BLEProxy::send(handle, (const uint8_t*) packet, length);
}

int UDPSocket::receiveFrom(Endpoint& remote, char* buffer, int length)
{
    BLE_DEBUG("UDPSocket::receiveFrom\r\n");

    if (BLEProxy::isReady() == false)
    {
        return -1;
    }

    /*  Register handle if it is not valid
    */
    uint32_t handle = remote.get_handle();

    if (BLEProxy::handleIsValid(handle) == false)
    {
        /* handle is invalid, open connection to renew handle
        */
        handle = BLEProxy::open(handle, remote.get_address(), remote.get_port(), BLEProxy::ProtocolTypeUDP);

        remote.set_handle(handle);
    }

    int32_t result = BLEProxy::recv(handle, (uint8_t*) buffer, length);

#if VERBOSE_DEBUG_OUTPUT
    for (int32_t idx = 0; idx < result; idx++)
    {
        BLE_DEBUG("%02X", buffer[idx]);
    }
    BLE_DEBUG("\r\n");
#endif

    return result;
}
