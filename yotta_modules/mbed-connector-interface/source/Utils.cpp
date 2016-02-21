/**
 * @file    Utils.cpp
 * @brief   mbed CoAP Endpoint misc utils collection
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

// mbed Endpoint includes
#include "mbed-connector-interface/ConnectorEndpoint.h"
#include "mbed-connector-interface/OptionsBuilder.h"

// External references (defined in main.cpp)
Connector::Options *configure_endpoint(Connector::OptionsBuilder &builder);
extern Logger logger;

// Our Endpoint
Connector::Endpoint *__endpoint = NULL;

// Our Endpoint configured Options
Connector::OptionsBuilder config;
Connector::Options *options = NULL;

// Shutdown button (K64F only...)
#if defined(TARGET_K64F)
InterruptIn shutdown_button(SW3);
#endif

// ************************* NSDL Linkage - MDS CONFIGURATION (defaulted) *********************************

uint8_t app_MAC_address[NODE_MAC_ADDRESS_LENGTH] = NODE_MAC_ADDRESS;   // Node MAC address
uint32_t channel_list = NODE_CHANNEL_LIST;                             											 // Node RF Channel list

uint8_t mesh_network_id[MESH_NETWORK_ID_LENGTH] = MESH_DEF_NETWORK_ID; // 802.15.4 Network ID (6LowPAN)
uint8_t rf_channel = MESH_DEF_RF_CHANNEL;                              // 802.15.4 RF Channel (6LowPAN)

// ************************* NSDL Linkage - MDS CONFIGURATION (defaulted)  *********************************

// further simplifies the endpoint main() configuration by removing the final initialization details of the endpoint...
void utils_configure_endpoint(void *p)
{   
	// our Endpoint 
	Connector::Endpoint *ep = (Connector::Endpoint *)p;
	
    // NSP/NSDL default configuration - see mbedConnectorInterface.h for definitions... 
    logger.log("Endpoint: setting defaults...");
    config.setEndpointNodename(NODE_NAME);
    config.setEndpointType(NSP_ENDPOINT_TYPE);
    config.setRegUpdatePeriod(REG_UPDATE_PERIOD_MS);
    config.setLifetime(REG_LIFETIME_SEC);

    // WiFi defaults
    config.setWiFiSSID((char *)WIFI_DEFAULT_SSID);          			// default: changeme
    config.setWiFiAuthType(WIFI_WPA_PERSONAL);      					// default: WPA Personal
    config.setWiFiAuthKey((char *)WIFI_DEFAULT_AUTH_KEY);   	// default: changeme

    // 802.15.4 defaults (6LowPAN)
    config.setNetworkID((char *)mesh_network_id);
    config.setRadioChannel((int)mesh_network_id);
    config.setRadioChannelList(NODE_CHANNEL_LIST);
    config.setMACAddress(app_MAC_address);   // TODO: arm_ns_tasklet_create() should call Endpoint::plumbNetwork()... currently its called before this MAC address can be (re)set
                 
    // Establish default CoAP observation behavior
    config.setImmedateObservationEnabled(true);    

    // Establish default CoAP GET-based observation control behavior
    config.setEnableGETObservationControl(false);    

    // main.cpp can override or change any of the above defaults...
    logger.log("Endpoint: gathering configuration overrides...");
    options = configure_endpoint(config);
	ep->setOptions(options);
	
    // DONE
    logger.log("Endpoint: endpoint configuration completed.");
}

// setup shutdown button (K64F only...)
#if defined(TARGET_K64F)
void utils_setup_deregistration_button(void *p) {
	if (p != NULL) {
		Connector::Endpoint *ep = (Connector::Endpoint *)p;
		logger.log("Endpoint: setting up shutdown button (SW2) for K64F...");
		shutdown_button.rise(ep,&Connector::Endpoint::closedown_endpoint);
	}
}	
#endif

// initialize the Connector::Endpoint instance
void *utils_init_endpoint(bool canActAsRouterNode) {
	// alloc Endpoint
    logger.log("Endpoint: allocating endpoint instance...");
	Connector::Endpoint *ep = new Connector::Endpoint(&logger,options);
	if (ep != NULL) {
		ep->asRouterNode(canActAsRouterNode);
    }
    return (void *)ep;
}

// register the endpoint and its resources
void utils_register_endpoint(void *p)
{
    if (p != NULL) {
		Connector::Endpoint *ep = (Connector::Endpoint *)p;
	    // initialize Endpoint resources
	    logger.log("Endpoint: registering endpoint and its resources...");
	    ep->register_endpoint();
	    
	    // setup the shutdown button (K64F only...)
#if defined(TARGET_K64F)
    	utils_setup_deregistration_button(p);
#endif
	}
}
