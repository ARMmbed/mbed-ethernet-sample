/**
 * @file    mbedEndpointNetwork.h
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
 
 // mbed-client support
 #include "mbed-client/m2minterfacefactory.h"
 #include "mbed-client/m2mdevice.h"
 #include "mbed-client/m2minterfaceobserver.h"
 #include "mbed-client/m2minterface.h"
 #include "mbed-client/m2mobjectinstance.h"
 #include "mbed-client/m2mresource.h"

 // mbed Connector Interface (configuration)
 #include "mbed-connector-interface/mbedConnectorInterface.h"

 // Include the Connector::Endpoint
 #include "mbed-connector-interface/ConnectorEndpoint.h"

 // Include the Connector::OptionsBuilder
 #include "mbed-connector-interface/OptionsBuilder.h"
 
 // Linkage to the lower network APIs and interfaces
 extern "C" void net_stubs_plumb_network(void *p);                        					
 extern "C" void net_stubs_create_main_loop(Connector::Endpoint *endpoint);
 extern "C" void net_stubs_begin_main_loop(Connector::Endpoint *endpoint);

 #endif // __MBED_ENDPOINT_NETWORK_H__
