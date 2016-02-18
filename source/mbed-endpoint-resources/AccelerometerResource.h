/**
 * @file    AccelerometerResource.h
 * @brief   mbed CoAP Endpoint Accelerometer sensor resource supporting CoAP GET
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

#ifndef __ACCELEROMETER_RESOURCE_H__
#define __ACCELEROMETER_RESOURCE_H__

// Base class
#include "mbed-connector-interface/DynamicResource.h"

// our Accelerometer sensor
#include "mbed-drivers/mbed.h"
#include "minar/minar.h"
#include "fxos8700cq/fxos8700cq.h"

// Configured for the FRDM-K64F with onboard sensors
InterruptIn __accel_int_pin(PTC13);
FXOS8700CQ __accel(PTE25, PTE24, FXOS8700CQ_SLAVE_ADDR1);

// forward declarations
static void *_instance = NULL;
extern "C" void _motion_detected(void);
extern "C" void _accel_interrupt(void);

/** 
 * AccelerometerResource class
 */
class AccelerometerResource : public DynamicResource
{
public:
    /**
    Default constructor
    @param logger input logger instance for this resource
    @param obj_name input the Accelerometer Object name
    @param res_name input the Accelerometer Resource name
    @param observable input the resource is Observable (default: FALSE)
    */
    AccelerometerResource(const Logger *logger,const char *obj_name,const char *res_name,const bool observable = false) : DynamicResource(logger,obj_name,res_name,"Accelerometer",M2MBase::GET_ALLOWED,observable) {
    	// init
    	_instance = (void *)this;
    	
    	// Configure the interrupt handler...
    	__accel_int_pin.fall(&_accel_interrupt);
    	__accel_int_pin.mode(PullUp);
    
    	// Configure the Accelerometer
    	__accel.config_int();      		// enabled interrupts from accelerometer
    	__accel.config_feature();  	// turn on motion detection
    	__accel.enable();          		// enable accelerometer
    }

    /**
    Get the value of the Accelerometer sensor
    @returns string containing the acclerometer sensor value
    */
    virtual string get() {
    	char buffer[128] = "";
    	
    	// get the latest accel & mag data...
        SRAWDATA accel;
        SRAWDATA mag;
        __accel.get_data(&accel,&mag);
                    
        // fill the value of the accelerometer
        sprintf(buffer,"{\"accelX\":%d,\"accelY\":%d,\"accelZ\":%d,\"magX\":%d,\"magY\":%d,\"magZ\":%d}", accel.x, accel.y,accel.z,mag.x,mag.y,mag.z);
        return string(buffer);
    }
};

// process the detected motion as a CoAP observation...
void _motion_detected(void) {
	if (_instance != NULL) {
		// observe()...
		((AccelerometerResource *)_instance)->observe();
	}
}

// Interrupt handler for the accelerometer/mag
void _accel_interrupt(void) {
      __accel.clear_int();
      minar::Scheduler::postCallback(mbed::util::FunctionPointer(_motion_detected).bind());
}
#endif // __ACCELEROMETER_RESOURCE_H__
