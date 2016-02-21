/**
 * @file    StaticResource.h
 * @brief   mbed CoAP Endpoint Static Resource class
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

#include "mbed-connector-interface/StaticResource.h"

// Constructor
StaticResource::StaticResource(const Logger *logger,const char *obj_name,const char *res_name, const char *value) : Resource<string>(logger,string(obj_name),string(res_name),string(value)), m_data_wrapper()
{
	this->m_object = NULL;
}

// Constructor
StaticResource::StaticResource(const Logger *logger,const char *obj_name,const char *res_name,const string value) : Resource<string>(logger,string(obj_name),string(res_name),string(value))
{
    this->m_data_wrapper = NULL;
    this->m_object = NULL;
}

// Constructor with buffer lengths
StaticResource::StaticResource(const Logger *logger,const string obj_name,const string res_name,const string value) : Resource<string>(logger,string(obj_name),string(res_name),string(value))
{
    this->m_data_wrapper = NULL;
    this->m_object = NULL;
}

// Copy constructor
StaticResource::StaticResource(const StaticResource &resource) : Resource<string>((const Resource<string> &)resource)
{
    this->m_data_wrapper = resource.m_data_wrapper;
    this->m_object = resource.m_object;
}

// Destructor
StaticResource::~StaticResource() {
}

// bind CoAP Resource..
M2MObject *StaticResource::bind(void *p) {
	if (p != NULL) {
		this->m_object = M2MInterfaceFactory::create_object(this->getObjName().c_str());
        if (this->m_object != NULL) {
            M2MObjectInstance* inst = this->m_object->create_object_instance();
            if (inst != NULL) {
            		if (this->getDataWrapper() != NULL) {
            			// wrap the data...
            			this->getDataWrapper()->wrap((uint8_t *)this->getValue().c_str(),(int)this->getValue().size());
            			inst->create_static_resource(this->getResName().c_str(),"StaticResource",M2MResourceInstance::STRING,this->getDataWrapper()->get(),(uint8_t)this->getDataWrapper()->length());
            			this->logger()->log("StaticResource: [%s] value: [%s] bound",this->getFullName().c_str(),this->getDataWrapper()->get());
        			}
        			else {
        				// do not wrap the data...
            			inst->create_static_resource(this->getResName().c_str(),"StaticResource",M2MResourceInstance::STRING,(uint8_t *)this->getValue().c_str(),(uint8_t)this->getValue().size());
             			this->logger()->log("StaticResource: [%s] value: [%s] bound",this->getFullName().c_str(),this->getValue().c_str());
        			}
             }
             else {
             		// create_object_instance() failed...
             		this->logger()->log("StaticResource: Unable to create object instance...");
             		delete this->m_object;
             		this->m_object = NULL;
             }
       }
       else {
       		// create_object() failed
       		this->logger()->log("StaticResource: Unable to create object...");
       }
	}
	else {
      	 this->logger()->log("StaticResource: NULL value parameter in bind() request...");
    }
    return this->m_object;
}
