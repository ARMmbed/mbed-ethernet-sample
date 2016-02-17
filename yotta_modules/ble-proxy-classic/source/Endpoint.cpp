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

#include "ble-proxy/Endpoint.h"

#include "ble-proxy/BLEProxy.h"

Endpoint::Endpoint()
    :   handle(0),
        host(),
        port(0)
{}

Endpoint::~Endpoint()
{
    /*  Close handle if one exists for this endpoint.
    */
    if (handle > 0)
    {
        BLEProxy::close(handle);
    }
}

void Endpoint::reset_address(void)
{
    /*  Close handle if endpoint has been set previously
    */
    if (handle > 0)
    {
        BLEProxy::close(handle);
    }

    host = "";
    port = 0;
    handle = 0;
}

uint32_t Endpoint::set_address(const char* _host, const int _port)
{
    host = _host; // convert to string
    port = _port;

    return 0;
}

const char* Endpoint::get_address()
{
    return host.c_str();
}

uint16_t Endpoint::get_port()
{
   return port;
}

void Endpoint::set_handle(uint32_t _handle)
{
    handle = _handle;
}

uint32_t Endpoint::get_handle()
{
    return handle;
}

