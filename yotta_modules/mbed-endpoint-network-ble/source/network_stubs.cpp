/**
 * @file    network_stubs.cpp
 * @brief   mbed Endpoint network stubs implementation (BLE)
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

#include "mbed-endpoint-network/network_stubs.h"

extern "C" {

// plumb out the network
void net_stubs_pre_plumb_network(bool canActAsRouterNode)
{
}

// called after the endpoint is configured...
void net_stubs_post_plumb_network(void)
{

}

// create a suitable main event loop for this specific network
void net_stubs_create_main_loop(void)
{
   // nothing to do for BLE endpoints - we are using the Ticker-based loop in nsdl_support.cpp
   ;
}

// register the endpoint
void net_stubs_register_endpoint(void)
{
    // not used.. must wait until we get a BLE connect callback.. then register...
    ;
}

// begin the main loop for processing network events
void net_stubs_begin_main_loop(void)
{
    // NDSL main loop
}

}
