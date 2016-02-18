/**
 * @file    SampleDynamicResource.h
 * @brief   mbed CoAP Endpoint example dynamic resource supporting CoAP GET and PUT
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

#ifndef __SAMPLE_DYNAMIC_RESOURCE_H__
#define __SAMPLE_DYNAMIC_RESOURCE_H__

// Base class
#include "mbed-connector-interface/DynamicResource.h"

// We are a simple counter
static int _sample_counter = 0;

/** SampleDynamicResource class
 */
class SampleDynamicResource : public DynamicResource
{

public:
    /**
    Default constructor
    @param logger input logger instance for this resource
    @param obj_name input the sample object name
    @param res_name input the sample resource name
    @param observable input the resource is Observable (default: FALSE)
    */
    SampleDynamicResource(const Logger *logger,const char *obj_name,const char *res_name,const bool observable = false) : DynamicResource(logger,obj_name,res_name,"Counter",M2MBase::GET_ALLOWED,observable) {
    }

    /**
    Get the value of the Sample Resource
    @returns string representing the current value of this resource
    */
    virtual string get() {
    	char buf[7];
    	memset(buf,0,7);
    	sprintf(buf,"%d",_sample_counter);
    	++_sample_counter;
        return string(buf);
    }
};

#endif // __SAMPLE_DYNAMIC_RESOURCE_H__
