/**
 * @file    MinarResourceObserver.cpp
 * @brief   mbed CoAP DynamicResource Minar-based observer (implementation)
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

 #include "mbed-connector-interface/MinarResourceObserver.h"

// DEBUG
#ifndef NDEBUG
#define DEBUG_OUT(...) { printf(__VA_ARGS__); }
#else
#define DEBUG_OUT(...) /* nothing */
#endif

 // constructor
 MinarResourceObserver::MinarResourceObserver(DynamicResource *resource,int sleep_time) : ResourceObserver(resource,sleep_time) {
        this->setObserving(false);
        
        // DEBUG
        DEBUG_OUT("MinarResourceObserver being used for %s (sleep_time=%d ms)\r\n",resource->getName().c_str(),sleep_time);
 }

 // destructor
 MinarResourceObserver::~MinarResourceObserver() {
     this->stopObservation();
 }

 // notifier
 void MinarResourceObserver::perform_observation() {
	 if (this->isObserving() == true && this->getResource() != NULL) {
             this->getResource()->observe();
     }
 }

 // begin observing...
 void MinarResourceObserver::beginObservation() {
     this->setObserving(true);
     minar::Scheduler::postCallback(this,&MinarResourceObserver::perform_observation).period(minar::milliseconds(this->getSleepTime()));
 }

 // stop observing...
 void MinarResourceObserver::stopObservation() {
     this->setObserving(false);
 }
