/**
 * @file    MinarResourceObserver.h
 * @brief   mbed CoAP DynamicResource Minar-based observer (header)
 * @author  Doug Anson/Chris Paola
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

#ifndef __MINAR_RESOURCE_OBSERVER_H__
#define __MINAR_RESOURCE_OBSERVER_H__

// mbed support
#include "mbed-drivers/mbed.h"

// switch for proper resource observer selection (from mbedEndpointNetwork)
#include "mbed-connector-interface/configuration.h"

// mbedConnectorInterface configuration
#include "mbed-connector-interface/mbedConnectorInterface.h"

// Base class support
#include "mbed-connector-interface/ResourceObserver.h"

class MinarResourceObserver : public ResourceObserver {
    public:
        /**
        Default Constructor
        @param resource input the resource to observe
        @param sleep_time input the time for the observation thread to sleep (in ms)
        */
        MinarResourceObserver(DynamicResource *resource,int sleep_time = OBS_PERIOD_MS);

        /**
        Destructor
        */
        virtual ~MinarResourceObserver();

        /**
        begin the observation
        */
        virtual void beginObservation();

        /**
        stop the observation
        */
        virtual void stopObservation();

        /**
        perform the observation (iteration)
        */
        void perform_observation();

    private:

};

#endif // __MINAR_RESOURCE_OBSERVER_H__
