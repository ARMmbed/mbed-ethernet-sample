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

#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__

#include <string>
#include <stdint.h>

class UDPSocket;

/**
IP Endpoint (address, port)
*/
class Endpoint {
    friend class UDPSocket;

public:
    /** IP Endpoint (address, port)
     */
    Endpoint(void);

    ~Endpoint(void);

    /** Reset the address of this endpoint
     */
    void reset_address(void);

    /** Set the address of this endpoint
    \param host The endpoint address (it can either be an IP Address or a hostname that will be resolved with DNS).
    \param port The endpoint port
    \return 0 on success, -1 on failure (when an hostname cannot be resolved by DNS).
     */
    uint32_t set_address(const char* host, const int port);

    /** Get the IP address of this endpoint
    \return The IP address of this endpoint.
     */
    const char* get_address(void);

    /** Get the port of this endpoint
    \return The port of this endpoint
     */
    uint16_t get_port(void);

    /** Handle used by proxy
     */
    void set_handle(uint32_t);
    uint32_t get_handle(void);

protected:
    uint32_t handle;
    std::string host;
    uint16_t port;
};

#endif // __ENDPOINT_H__
