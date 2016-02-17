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

#ifndef __UDPSOCKET_H__
#define __UDPSOCKET_H__

#include "ble-proxy/Socket.h"
#include "ble-proxy/Endpoint.h"

/**
UDP Socket
*/
class UDPSocket : public Socket
{
public:
    /** Instantiate an UDP Socket.
    */
    UDPSocket();

    /** Init the UDP Client Socket without binding it to any specific port
    \return 0 on success, -1 on failure.
    */
    int init(void);

    /** Bind a UDP Server Socket to a specific port
    \param port The port to listen for incoming connections on
    \return 0 on success, -1 on failure.
    */
    int bind(int port);

    /** Join the multicast group at the given address
    \param address  The address of the multicast group
    \return 0 on success, -1 on failure.
    */
    int join_multicast_group(const char* address);

    /** Set the socket in broadcasting mode
    \return 0 on success, -1 on failure.
    */
    int set_broadcasting(bool broadcast=true);

    /** Send a packet to a remote endpoint
    \param remote   The remote endpoint
    \param packet   The packet to be sent
    \param length   The length of the packet to be sent
    \return the number of written bytes on success (>=0) or -1 on failure
    */
    int sendTo(Endpoint& remote, const char* packet, int length);

    /** Receive a packet from a remote endpoint
    \param remote   The remote endpoint
    \param buffer   The buffer for storing the incoming packet data. If a packet
           is too long to fit in the supplied buffer, excess bytes are discarded
    \param length   The length of the buffer
    \return the number of received bytes on success (>=0) or -1 on failure
    */
    int receiveFrom(Endpoint& remote, char* buffer, int length);
};

#endif // __UDPSOCKET_H__
