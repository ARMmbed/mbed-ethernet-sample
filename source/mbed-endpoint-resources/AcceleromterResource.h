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
#include "FXOS8700Q/FXOS8700Q.h"

// Configured for the FRDM-K64F with onboard sensors
I2C _acc_i2c(PTE25, PTE24);
FXOS8700QAccelerometer _acc(_acc_i2c, FXOS8700CQ_SLAVE_ADDR1);

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
    	// start the accelerometer
    	_acc.enable();
    }

    /**
    Get the value of the Accelerometer sensor
    @returns string containing the acclerometer sensor value
    */
    virtual string get() {
    	char buffer[128] = "";
    	
    	// get the latest accel data...
        motion_data_units_t data;
        _acc.getAxis(data);
                    
        // fill the value of the accelerometer
        sprintf(buffer,"{\"accelX\":%1.1f, \"accelY\":%1.1f, \"accelZ\":%1.1f}", data.x, data.y ,data.z);
        return string(buffer);
    }
};

#endif // __ACCELEROMETER_RESOURCE_H__
