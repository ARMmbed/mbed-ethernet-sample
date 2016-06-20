/**
 * @file    ResponderFunctions.cpp
 * @brief   mbed Endpoint Data Management Action Responders
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

// Connector::Endpoint Support
#include "mbed-connector-interface/ConnectorEndpoint.h"

// DeviceManager Support
#include "mbed-connector-interface/DeviceManager.h"

// DeviceManagementResponder Support
#include "mbed-connector-interface/DeviceManagementResponder.h"

// Logger instance
static Logger *_logger = NULL;

// NVIC System Reset
extern "C" void NVIC_SystemReset(void);

// Initialize
extern "C" void dm_initialize(const void *l) {
    // Logger
    Logger *logger = (Logger *)l;
    _logger = logger;
    
    // Initialize
    logger->log("dm_initialize: initialized");
}


// Reboot Responder
extern "C" bool dm_reboot_responder(const void *e,const void *l,const void */* not used */) {
    // Logger
    Logger *logger = (Logger *)l;
     _logger = logger;
    
    // Endpoint
    Connector::Endpoint *ep = (Connector::Endpoint *)e;
    
    // Device Manager
    //DeviceManager *dm = (DeviceManager *)ep->getDeviceManager();
    
    // DeviceManagementResponder
    //DeviceManagementResponder *dmr = (DeviceManagementResponder *)dm->getResponder();
        
    // DEBUG
    logger->log("dm_reboot_responder: de-registering endpoint...");

    // de-register the endpoint
    ep->de_register_endpoint();
    
    // DEBUG
    logger->log("dm_reboot_responder: Rebooting endpoint...");
    
    // Reboot
    NVIC_SystemReset();
    
    // return OK
    return true;
}
 
// Reset Responder
extern "C" bool dm_reset_responder(const void *e,const void *l,const void */* not used */) {
    // Logger
    Logger *logger = (Logger *)l;
     _logger = logger;
    
    // Endpoint
    Connector::Endpoint *ep = (Connector::Endpoint *)e;
    
    // Device Manager
    //DeviceManager *dm = (DeviceManager *)ep->getDeviceManager();
    
    // DeviceManagementResponder
    //DeviceManagementResponder *dmr = (DeviceManagementResponder *)dm->getResponder();
        
    // DEBUG
    logger->log("dm_reset_responder: de-registering endpoint...");

    // de-register the endpoint
    ep->de_register_endpoint();
    
    // DEBUG
    logger->log("dm_reset_responder: Resetting endpoint...");
    
    // Reset (FUTURE)
    NVIC_SystemReset();
    
    // return OK
    return true;
}

// FOTA Manifest is set
extern "C" bool dm_set_manifest(const void *e,const void *l,const void *manifest,uint32_t manifest_length) {
    // Logger
    Logger *logger = (Logger *)l;
     _logger = logger;
    
    // Endpoint
    Connector::Endpoint *ep = (Connector::Endpoint *)e;
    if (ep->isRegistered() && manifest != NULL && manifest_length > 0) {
        // set the manifest
        logger->log("dm_set_manifest: Manifest: %s (%d)",(char *)manifest,(int)manifest_length);
        
        // return our processed status
        return true;
    }
    return false;
}

// FOTA Image is Set
extern "C" bool dm_set_fota_image(const void *e,const void *l,const void *image,uint32_t image_length) {
    // Logger
    Logger *logger = (Logger *)l;
     _logger = logger;
    
    // Endpoint
    Connector::Endpoint *ep = (Connector::Endpoint *)e;
    if (ep->isRegistered() && image != NULL && image_length > 0) {
        // set the manifest
        logger->log("dm_set_fota_image: Image length: %d",(int)image_length);
        
        // return our processed status
        return true;
    }
    return false;
}

// FOTA Invocation Handler
extern "C" bool dm_invoke_fota(const void *e,const void *l) {
    // Logger
    Logger *logger = (Logger *)l;
     _logger = logger;
    
    // Endpoint
    Connector::Endpoint *ep = (Connector::Endpoint *)e;
    
    // Device Manager
    //DeviceManager *dm = (DeviceManager *)ep->getDeviceManager();
    
    // DEBUG
    logger->log("fota_update: de-registering endpoint...");
    
    // de-register the endpoint
    ep->de_register_endpoint();
    
    // DEBUG
    logger->log("fota_update: invoking FOTA on the endpoint...");
    
    // Download
    logger->log("fota_update: FOTA beginning... Downloading...");
    
    // DO THE FOTA update (FUTURE)
    logger->log("fota_update: Download completed. Invoking FOTA on device...");
    
    // Clean up and reboot
    logger->log("fota_update: FOTA completed. Rebooting endpoint...");
    
    // return OK
    return true;
}