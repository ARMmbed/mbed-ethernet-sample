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

#ifndef __BLE_PROXY_H__
#define __BLE_PROXY_H__

#include "mbed.h"


#include <string>
#include <stdint.h>


// socket length is simply an unsigned int.
typedef uint32_t socklen_t;

namespace BLEProxy
{
    typedef enum
    {
        ProtocolTypeTCP   = 0,
        ProtocolTypeUDP   = 1
    } ProtocolType_t;

    typedef enum
    {
        MessageTypeOpen       = 0,
        MessageTypeSend       = 1,
        MessageTypeClose      = 2,
        MessageTypeOnOpen     = 3,
        MessageTypeOnMessage  = 4,
        MessageTypeOnClose    = 5,
        MessageTypeInvalid
    } MessageType_t;

    void init(void);

    uint32_t open(uint32_t handle, const char* host, uint16_t port, ProtocolType_t type);
    void close(uint32_t handle);

    bool handleIsValid(uint32_t handle);

    int32_t send(uint32_t handle, const uint8_t* buffer, uint32_t length);
    int32_t recv(uint32_t handle, uint8_t* buffer, uint32_t length);

    bool isReady(void);

    void onReceive(void (*callback)(void));

#if !defined(YOTTA_MINAR_VERSION_STRING)
    void callFromMainLoop(void);
#endif
}


#endif // __BLE_PROXY_H__
