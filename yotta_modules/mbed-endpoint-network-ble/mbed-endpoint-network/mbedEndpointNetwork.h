/**
 * @file    mbedEndpointNetworkStubs.h
 * @brief   mbed Endpoint Network header - declares underling network instance
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

 #ifndef __MBED_ENDPOINT_NETWORK_H__
 #define __MBED_ENDPOINT_NETWORK_H__

 // mbed
 #include "mbed.h"

 // USB Serial port access for debugging/logging
// RawSerial pc(USBTX,USBRX);

 // Logging facility
 #include "mbed-connector-interface/Logger.h"
// Logger logger(&pc);
Logger logger(NULL);

 // mbed Connector Endpoint includes
 #include "mbed-connector-interface/ConnectorEndpoint.h"
 #include "mbed-connector-interface/OptionsBuilder.h"

 #endif // __MBED_ENDPOINT_NETWORK_H__
