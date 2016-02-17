/**
 * @file    network_stubs.h
 * @brief   mbed Endpoint Network Stubs header (BLE)
 * @author  Doug Anson
 * @version 1.0
 * @see
 *
 * Copyright (c) 2014
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
#ifndef __BLE_STUBS_H__
#define __BLE_STUBS_H__

// mbed support
#include "mbed.h"

// configure the endpoint
extern void configure_endpoint();

#include "mbed.h"

#define IP_ADDRESS_LENGTH  64

// timeval
struct timeval {
    uint32_t tv_sec;
    uint32_t tv_usec;
};


// socket length is simply an unsigned int.
typedef uint32_t socklen_t;

#endif // __BLE_STUBS_H__