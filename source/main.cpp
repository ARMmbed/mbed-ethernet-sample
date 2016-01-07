/**
 * @file    main.cpp
 * @brief   mbed Connected Home Endpoint main entry point
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

// mbed Connector Interface (configuration)
#include "mbedConnectorInterface.h"

// mbed Endpoint Network
#include "mbedEndpointNetwork.h"

// Static Resources
#include "StaticResource.h"
StaticResource mfg(&logger,"dev/mfg","FRDM-K64F");
StaticResource model(&logger,"dev/mdl","mbed Ethernet node");

//
// Dynamic Resource Note:
//
//  mbedConnectorInterface supports up to IPT_MAX_ENTRIES 
//  (currently 5) independent dynamic resources.
//
//  You can increase this (at the cost of memory) 
//  in mbedConnectorinterface.h
//

// Sample Dynamic Resource
#include "SampleDynamicResource.h"
SampleDynamicResource sample(&logger,"999/999/9999");

// Light Resource
#include "LightResource.h"
LightResource light(&logger,"311/0/5850");

// Temperature Resource
#include "TemperatureResource.h"
TemperatureResource temperature(&logger,"303/0/5700",true);         // "true" --> resource is observable

// My Endpoint Name
#define MY_ENDPOINT_NAME                        "mbed-eth-endpoint"

// My NSP Domain
#define MY_NSP_DOMAIN                           "domain"   

// Customization Example: My custom NSP IPv4 address and NSP CoAP port 
uint8_t my_nsp_address[NSP_IP_ADDRESS_LENGTH] = {129,41,135,57};       // connector (api.connector.mbed.org)
int my_nsp_coap_port                          = 5683;

// called from the Endpoint::start() below to create resources and the endpoint internals...
Connector::Options *configure_endpoint(Connector::OptionsBuilder &config)
{    
    // Build the endpoint configuration parameters
    logger.log("configure_endpoint: building endpoint configuration...");
    return config.setEndpointNodename(MY_ENDPOINT_NAME)                   // custom endpoint name
                 .setNSPAddress(my_nsp_address)                           // custom NSP address
                 .setDomain(MY_NSP_DOMAIN)                                // custom NSP domain
                 .setNSPPortNumber(my_nsp_coap_port)                      // custom NSP CoAP port
                 
                 // enable or disable(default) immediate observationing control
                 .setImmedateObservationEnabled(true)
                 
                 // enable or disable(default) GET-based observation control
                 .setEnableGETObservationControl(false)
                 
                 // add the static resource representing this endpoint
                 .addResource(&mfg)
                 .addResource(&model)
                 
                 // add a Sample Dynamic Resource
                 .addResource(&sample)
                   
                 // Add my specific physical dynamic resources...
                 .addResource(&light)
                 .addResource(&temperature,5000)        // observe every 20 seconds - use threaded observer 
                   
                 // finalize the configuration...
                 .build();
}

// main entry point...
int main()
{
    // Announce
    logger.log("\r\n\r\nmbed mDS Sample Endpoint v1.0 (Ethernet-Observe)");
    
    // we have to plumb our network first
    Connector::Endpoint::plumbNetwork();
             
    // starts the endpoint by finalizing its configuration (configure_endpoint() above called),creating a Thread and reading NSP events...
    logger.log("Start the endpoint to finish setup and enter the main loop...");
    Connector::Endpoint::start();
}