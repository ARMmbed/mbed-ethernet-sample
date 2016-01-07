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
#include "DynamicResource.h"

// Put any physical abstractions here (i.e. DigitalOut, etc...). For this sample, its simply a string...
string my_physical_resource("hello mbed");

/** SampleDynamicResource class
 */
class SampleDynamicResource : public DynamicResource
{

public:
    /**
    Default constructor
    @param logger input logger instance for this resource
    @param name input the sample resource name
    @param observable input the resource is Observable (default: FALSE)
    */
    SampleDynamicResource(const Logger *logger,const char *name,const bool observable = false) : DynamicResource(logger,name,"SampleResource",SN_GRS_GET_ALLOWED|SN_GRS_PUT_ALLOWED,observable) {
    }

    /**
    Get the value of the Sample Resource
    @returns string representing the current value of this resource
    */
    virtual string get() {
        return my_physical_resource;
    }

    /**
    Set the value of the Sample Resource
    @param string input the new string value for this resource
    */
    virtual void put(const string value) {
        my_physical_resource = value;
    }
};

#endif // __SAMPLE_DYNAMIC_RESOURCE_H__
