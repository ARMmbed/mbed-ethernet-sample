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

// Include security.h
#include "security.h"

// mbed Endpoint Network
#include "mbed-connector-interface/mbedEndpointNetwork.h"

// Logger
#include "mbed-connector-interface/Logger.h"
Serial pc(USBTX,USBRX);
Logger logger(&pc);

// Device Resources
#include "mbed-connector-interface/DeviceResource.h"
DeviceResource mfg(&logger,M2MDevice::Manufacturer,"Freescale");
DeviceResource dev_type(&logger,M2MDevice::DeviceType,"FRDM");
DeviceResource model(&logger,M2MDevice::ModelNumber,"K64F");
DeviceResource serial(&logger,M2MDevice::SerialNumber,"0123456789");

// Sample Static Resource
#include "mbed-connector-interface/StaticResource.h"
StaticResource static_sample(&logger,"101","1010","hello mbed");

// Sample Dynamic Resource (a counter)
#include "mbed-endpoint-resources/SampleDynamicResource.h"
SampleDynamicResource sample_counter(&logger,"123","4567",true);				// "true" -> resource is observable

// Light Resource
#include "mbed-endpoint-resources/LightResource.h"
LightResource light(&logger,"311","5850");

// Temperature Resource
#include "mbed-endpoint-resources/TemperatureResource.h"
TemperatureResource temperature(&logger,"303","5700",true);         					// "true" --> resource is observable

// Accelerometer Resource
#include "mbed-endpoint-resources/AccelerometerResource.h"
AccelerometerResource accel(&logger,"888","7700",true);         						    // "true" --> resource is observable

// Custom Connector URL and Port number for CoAP...
char *connector_url = (char *)"coap://api.connector.mbed.com:5684";           		// connector (api.connector.mbed.org)

// called from the Endpoint::start() below to create resources and the endpoint internals...
Connector::Options *configure_endpoint(Connector::OptionsBuilder &config)
{    
    // Build the endpoint configuration parameters
    logger.log("configure_endpoint: building endpoint configuration...");
    return config.setEndpointNodename(MBED_ENDPOINT_NAME)                    // custom endpoint name (security.h)
                 .setDomain(MBED_DOMAIN)                                							   // custom NSP domain (security.h)
                 .setConnectorURL(connector_url)                          							   // custom Connector URL
                 
                 // set the Security Credentials (from security.h)
                 .setServerCertificate((uint8_t *)SERVER_CERT,(int)sizeof(SERVER_CERT))
                 .setClientCertificate((uint8_t *)CERT,(int)sizeof(CERT))
                 .setClientKey((uint8_t *)KEY,(int)sizeof(KEY))
                 
                 // enable or disable(default) immediate observationing control
                 .setImmedateObservationEnabled(true)
                 
                 // enable or disable(default) GET-based observation control
                 .setEnableGETObservationControl(false)
                 
                 // add Device Resources
                 .addResource(&mfg)
                 .addResource(&dev_type)
                 .addResource(&model)
                 .addResource(&serial)
                 
                 // add a Sample Dynamic Resource
                 .addResource(&static_sample)
                 
                 // add a Sample Counter
                 .addResource(&sample_counter,10000)			// observe every 10 seconds
                   
                 // Add my specific physical dynamic resources...
                 .addResource(&light)
                 .addResource(&temperature,7500) 					// observe every 7.5 seconds
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
     
    // we have to plumb our network first
    Connector::Endpoint::plumbNetwork();
             
    // starts the endpoint by finalizing its configuration (configure_endpoint() above called),creating a Thread and reading NSP events...
    Connector::Endpoint::start();
}
