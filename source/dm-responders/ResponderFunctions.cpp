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

// DeviceIntegerResource Support (State and Result)
#include "mbed-connector-interface/DeviceIntegerResource.h"

// NVIC System Reset
extern "C" void NVIC_SystemReset(void);

// Reboot Responder
extern "C" bool dm_reboot_responder(const void *e,const void *l,const void */* not used */) {
    // Logger
    Logger *logger = (Logger *)l;
    
    // Endpoint
    Connector::Endpoint *ep = (Connector::Endpoint *)e;
    
    // Device Manager
    DeviceManager *dm = (DeviceManager *)ep->getDeviceManager();
    
    // DeviceManagementResponder
    DeviceManagementResponder *dmr = (DeviceManagementResponder *)dm->getResponder();
    
    // State and Result Resources
    DeviceIntegerResource *state = (DeviceIntegerResource *)dmr->getStateResource();
    DeviceIntegerResource *result = (DeviceIntegerResource *)dmr->getResultResource();
    
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
    
    // Endpoint
    Connector::Endpoint *ep = (Connector::Endpoint *)e;
    
    // Device Manager
    DeviceManager *dm = (DeviceManager *)ep->getDeviceManager();
    
    // DeviceManagementResponder
    DeviceManagementResponder *dmr = (DeviceManagementResponder *)dm->getResponder();
    
    // State and Result Resources
    DeviceIntegerResource *state = (DeviceIntegerResource *)dmr->getStateResource();
    DeviceIntegerResource *result = (DeviceIntegerResource *)dmr->getResultResource();
    
    // DEBUG
    logger->log("dm_reboot_responder: Resetting endpoint...");
    
    // Reset (FUTURE)
    NVIC_SystemReset();
    
    // return OK
    return true;
}

// FOTA Invocation Handler
extern "C" bool dm_invoke_fota(const void *e,const void *l,const void *data) {
    // Logger
    Logger *logger = (Logger *)l;
    
    // Endpoint
    Connector::Endpoint *ep = (Connector::Endpoint *)e;
    
    // Device Manager
    DeviceManager *dm = (DeviceManager *)ep->getDeviceManager();
    
    // DeviceManagementResponder
    DeviceManagementResponder *dmr = (DeviceManagementResponder *)dm->getResponder();
    
    // State and Result Resources
    DeviceIntegerResource *state = (DeviceIntegerResource *)dmr->getStateResource();
    DeviceIntegerResource *result = (DeviceIntegerResource *)dmr->getResultResource();
    
    // Download
    result->put("0");   // FOTA beginning
    state->put("2");    // we are downloading... 
    logger->log("fota_update: FOTA beginning... Downloading...");
    
    // DO THE FOTA update (FUTURE)
    state->put("3"); // download completed... FOTA...
    logger->log("fota_update: Download completed. Invoking FOTA on device...");
    
    // Clean up and reboot
    state->put("1");    // idle
    result->put("1");   // FOTA completed successfully 
    logger->log("fota_update: FOTA completed. Rebooting endpoint...");
    
    // Reboot
    NVIC_SystemReset();
    
    // return OK
    return true;
}