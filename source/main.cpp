/**
 * @file    main.cpp
 * @brief   mbed Endpoint Sample main
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

// CUSTOMIZE ME: Define the core Device Types, Firmware, Hardware, Software information
#define MY_DEVICE_MFG			"NXP"
#define MY_DEVICE_TYPE			"mbed-endpoint"
#define MY_DEVICE_MODEL			"K64F"
#define MY_DEVICE_SERIAL 		"0123456789"
#define MY_FIRMWARE_VERSION		"1.0.0"
#define MY_HARDWARE_VERSION		"1.0.0"
#define MY_SOFTWARE_VERSION		"1.0.0"

// Passphrase to supply for data management authentication
#define MY_DM_PASSPHRASE		"arm1234"

// Include security.h
#include "security.h"

// mbed Endpoint Network
#include "mbed-connector-interface/mbedEndpointNetwork.h"

// Logger
#include "mbed-connector-interface/Logger.h"
Serial pc(USBTX,USBRX);
Logger logger(&pc);

// Include the default Device Management Responders
#include "dm-responders/ResponderFunctions.h"

// Our Device Management Authenticator (trivial passphrase authenticator used)
#include "mbed-connector-interface/PassphraseAuthenticator.h"
PassphraseAuthenticator authenticator(&logger,MY_DM_PASSPHRASE);

// Our Device Management Responder
#include "mbed-connector-interface/DeviceManagementResponder.h"
DeviceManagementResponder dm_processor(&logger,&authenticator);

// Our Device Manager
#include "mbed-connector-interface/DeviceManager.h"
DeviceManager device_manager(&logger,&dm_processor,MY_DEVICE_MFG,MY_DEVICE_TYPE,MY_DEVICE_MODEL,MY_DEVICE_SERIAL,MY_FIRMWARE_VERSION,MY_HARDWARE_VERSION,MY_SOFTWARE_VERSION);

// Sample Static Resource
#include "mbed-connector-interface/StaticResource.h"
StaticResource static_sample(&logger,"101","1010","hello mbed");

// Sample Dynamic Resource (a counter)
#include "mbed-endpoint-resources/SampleDynamicResource.h"
SampleDynamicResource sample_counter(&logger,"123","4567",true);		// "true" -> resource is observable

// Light Resource
#include "mbed-endpoint-resources/LightResource.h"
LightResource light(&logger,"311","5850");

// Temperature Resource
#include "mbed-endpoint-resources/TemperatureResource.h"
TemperatureResource temperature(&logger,"303","5700",true);         		// "true" --> resource is observable

// Accelerometer Resource
#include "mbed-endpoint-resources/AccelerometerResource.h"
AccelerometerResource accel(&logger,"888","7700",true);         	   	// "true" --> resource is observable

// Custom Connector URL and Port number for CoAP...
char *connector_url = (char *)"coap://api.connector.mbed.com:5684";        	// mbed Device Connector URL and DTLS port number for CoAP

// called from the Endpoint::start() below to create resources and the endpoint internals...
Connector::Options *configure_endpoint(Connector::OptionsBuilder &config)
{    
    // Build the endpoint configuration parameters
    logger.log("Endpoint::Main: customizing endpoint configuration...");
    return config
    	// our URL to mbed Device Connector
    	.setConnectorURL(connector_url)                          							
                 
        // For Ethernet, we will set the default CoAP Connection type to TCP
        //.setCoAPConnectionType(COAP_TCP)								
                
        // PROVISIONING: set the Provisioning Credentials (all from security.h)
        .setEndpointNodename(MBED_ENDPOINT_NAME)                  			
        .setDomain(MBED_DOMAIN)                               							  
        .setServerCertificate((uint8_t *)SERVER_CERT,(int)sizeof(SERVER_CERT))
        .setClientCertificate((uint8_t *)CERT,(int)sizeof(CERT))
        .setClientKey((uint8_t *)KEY,(int)sizeof(KEY))
                 
        // add a Sample Static Resource
        .addResource(&static_sample)
                 
        // add a Sample Counter (Dynamic Resource)
        .addResource(&sample_counter,10000)			// observe every 10 seconds
                 
        // Add my specific physical dynamic resources...
        .addResource(&light)
        .addResource(&temperature,8000) 			// observe every 8 seconds
        .addResource(&accel)   							
                   
        // finalize the configuration...
        .build();
}

// main entry point...
void app_start(int, char *[])
{
    // set Serial
    pc.baud(115200);
	
    // Announce
    logger.log("\r\n\r\nmbed mDS Sample Endpoint v3.0 (Ethernet)");
    
    // Register the default Device Management Responders
    dm_processor.setRebootResponderHandler(dm_reboot_responder);
    dm_processor.setResetResponderHandler(dm_reset_responder);
    dm_processor.setFOTAInvocationHandler(dm_invoke_fota);
	 
    // we have to plumb our network first
    Connector::Endpoint::plumbNetwork((void*)&device_manager);
             
    // starts the endpoint by finalizing its configuration (configure_endpoint() above called),creating a Thread and reading NSP events...
    Connector::Endpoint::start();
}
