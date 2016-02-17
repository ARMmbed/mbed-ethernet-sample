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

#include "ble-proxy/TCPSocketConnection.h"

#include "ble-proxy/BLEProxy.h"

#define VERBOSE_DEBUG_OUTPUT 0

#if 0
#define BLE_DEBUG(...) { printf(__VA_ARGS__); }
#else
#define BLE_DEBUG(...) /* nothing */
#endif

TCPSocketConnection::TCPSocketConnection()
    :   Socket(),
        Endpoint(),
        isConnected(false)
{}

int TCPSocketConnection::connect(const char* _host, const int _port)
{
    handle = BLEProxy::open(0, _host, _port, BLEProxy::ProtocolTypeTCP);

    if (handle != 0)
    {
        host = _host;
        port = _port;
        isConnected = true;
    }

    return 0;
}

bool TCPSocketConnection::is_connected(void)
{
    return isConnected && BLEProxy::isReady();
}

int TCPSocketConnection::send(const char* packet, int length)
{
    BLE_DEBUG("TCPSocketConnection::send\r\n");

#if VERBOSE_DEBUG_OUTPUT
    for (std::size_t idx = 0; idx < length; idx++)
    {
        BLE_DEBUG("%02X", packet[idx]);
    }
    BLE_DEBUG("\r\n");
#endif

    if ((handle == 0) || (BLEProxy::isReady() == false))
    {
        BLE_DEBUG("BLEProxy is busy\r\n");
        return -1;
    }

    /*  renew handle if it is not valid
    */
    if (BLEProxy::handleIsValid(handle) == false)
    {
        handle = BLEProxy::open(handle, host.c_str(), port, BLEProxy::ProtocolTypeTCP);
    }

    /*  BLEProxy is ready, proceed.
    */

    return BLEProxy::send(handle, (const uint8_t*) packet, length);
}

int TCPSocketConnection::send_all(const char* packet, int length)
{
    return send(packet, length);
}

int TCPSocketConnection::receive(char* buffer, int length)
{
    BLE_DEBUG("TCPSocketConnection::receive\r\n");

    if ((handle == 0) || (BLEProxy::isReady() == false))
    {
        return -1;
    }

    /*  BLEProxy is ready, proceed.
    */
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

int TCPSocketConnection::receive_all(char* buffer, int length)
{
    return receive(buffer, length);
}
