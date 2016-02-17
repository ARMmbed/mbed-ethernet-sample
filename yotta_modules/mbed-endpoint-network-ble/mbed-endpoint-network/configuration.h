/**
 * @file    configuration.h
 * @brief   mbed Endpoint Network configuration header
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

#ifndef __CONFIGURATION_H_
#define __CONFIGURATION_H_

/************** CONFIGURATION PARAMETERS  ************************/

/************** CONFIGURATION PARAMETERS  ************************/

/************* NSP CONFIGURATION DEFAULTS  ***********************/

// NSP node name
 #define NODE_NAME_LENGTH         64
 #define NODE_NAME                "mbed-endpoint"

 // NSP Address (4 bytes for IPV4, 16 bytes for IPV6)
 #define NSP_IP_ADDRESS_LENGTH    4
 #define NSP_IP_ADDRESS           {23,99,29,171}

 // NSP Endpoint Type specification
 #define NSP_ENDPOINT_TYPE_LENGTH 32
 #define NSP_ENDPOINT_TYPE        "mbed-device"

 // NSP Domain used
 #define NSP_DOMAIN_LENGTH        48
 #define NSP_DOMAIN               "domain"

 /************* NSP CONFIGURATION DEFAULTS  ***********************/

#endif // __CONFIGURATION_H_