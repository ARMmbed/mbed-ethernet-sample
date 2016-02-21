/**
 * @file    network_stubs.cpp
 * @brief   mbed Endpoint network stubs implementation (Ethernet)
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

// Includes...
#include "mbed-endpoint-network-eth/mbedEndpointNetworkStubs.h"

// Logger
#include "mbed-connector-interface/Logger.h"
extern Logger logger;

// Ethernet Interface
EthernetInterface ethernet;

// endpoint pointer
static void *_my_endpoint = NULL;

// MAC Address customization
extern "C" void mbed_mac_address(char *s) 
{
    char mac[6];
    mac[0] = 0x46;
    mac[1] = 0x32;
    mac[2] = 0x67;
    mac[3] = 0xf4;
    mac[4] = 0xa6;
    mac[5] = 0xdb;
 
    if (s != NULL) memcpy(s, mac, 6);
}

// MAC Address customization
static char mmac[6];

extern "C" {

// plumb out the network
void net_stubs_plumb_network(void *p)   {
     // MAC address handling
     mbed_mac_address(mmac);
     
     // Ethernet Init...
     ethernet.init();
     ethernet.connect();
    
     // LWIP initialize
     lwipv4_socket_init(); 
    
     // Debug
     logger.log("mbedEthernetNetwork(Ethernet): IP Address: %s\r\n",ethernet.getIPAddress());
     
    // save 
    _my_endpoint = p;
}

// create a suitable main event loop for this specific network
void net_stubs_create_main_loop(Connector::Endpoint *endpoint)
{
   	logger.log("Completing Endpoint Registration");
   	endpoint->complete_endpoint_registration(endpoint->getServer(),endpoint->getObjectList());
   	mbed::util::FunctionPointer2<void, M2MSecurity*, M2MObjectList> fp(endpoint, &Connector::Endpoint::complete_endpoint_registration);
    minar::Scheduler::postCallback(fp.bind(endpoint->getServer(),endpoint->getObjectList()));
}

// begin the main loop for processing network events
void net_stubs_begin_main_loop(Connector::Endpoint *endpoint) 
{
	Connector::Options *options = endpoint->getOptions();
	logger.log("Endpoint::initialize(): registration init complete. Starting main loop...");
	minar::Scheduler::postCallback(endpoint,&Connector::Endpoint::re_register_endpoint).period(minar::milliseconds(options->getRegUpdatePeriod()));
}
}

