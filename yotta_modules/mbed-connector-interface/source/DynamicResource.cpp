/**
 * @file    DynamicResource.cpp
 * @brief   mbed CoAP Endpoint Dynamic Resource class
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

#include "mbed-connector-interface/DynamicResource.h"

// ResourceObserver help
#include "mbed-connector-interface/ResourceObserver.h"

// Options enablement
#include "mbed-connector-interface/Options.h"

// GET option that can be used to Start/Stop Observations...
#define START_OBS 0
#define STOP_OBS  1

// MaxAge support for each DynamicResource
#define DEFAULT_MAXAGE 60

// ContentFormat defaults for each DynamicResource
#define DEFAULT_CONTENT_FORMAT 0

// default constructor
DynamicResource::DynamicResource(const Logger *logger,const char *obj_name,const char *res_name,const char *res_type,uint8_t res_mask,const bool observable) : Resource<string>(logger,string(obj_name),string(res_name),string(""))
{
    this->m_res_type = string(res_type);
    this->m_observable = observable;
    this->m_res_mask = res_mask;
    this->m_obs_number = 0;
    this->m_data_wrapper = NULL;
    this->m_observer = NULL;
    this->m_maxage = DEFAULT_MAXAGE;
    this->m_content_format = DEFAULT_CONTENT_FORMAT;
    this->m_object = NULL;
    this->m_op_processor = NULL;
}

// constructor (input initial value)
DynamicResource::DynamicResource(const Logger *logger,const char *obj_name,const char *res_name,const char *res_type,const string value,uint8_t res_mask,const bool observable) : Resource<string>(logger,string(obj_name),string(res_name),value)
{
    this->m_res_type = string(res_type);
    this->m_observable = observable;
    this->m_res_mask = res_mask;
    this->m_obs_number = 0;
    this->m_data_wrapper = NULL;
    this->m_observer = NULL;
    this->m_maxage = DEFAULT_MAXAGE;
    this->m_content_format = DEFAULT_CONTENT_FORMAT;
    this->m_object = NULL;
    this->m_op_processor = NULL;
}

// constructor (strings)
DynamicResource::DynamicResource(const Logger *logger,const string obj_name,const string res_name,const string res_type,const string value,uint8_t res_mask,const bool observable) : Resource<string>(logger,obj_name,res_name,value)
{
    this->m_res_type = res_type;
    this->m_observable = observable;
    this->m_res_mask = res_mask;
    this->m_obs_number = 0;
    this->m_data_wrapper = NULL;
    this->m_observer = NULL;
    this->m_maxage = DEFAULT_MAXAGE;
    this->m_content_format = DEFAULT_CONTENT_FORMAT;
    this->m_object = NULL;
    this->m_op_processor = NULL;
}

// copy constructor
DynamicResource::DynamicResource(const DynamicResource &resource) : Resource<string>((const Resource<string> &)resource)
{
    this->m_res_type = resource.m_res_type;
    this->m_observable = resource.m_observable;
    this->m_res_mask = resource.m_res_mask;
    this->m_obs_number = resource.m_obs_number;
    this->m_data_wrapper = resource.m_data_wrapper;
    this->m_observer = resource.m_observer;
    this->m_maxage = resource.m_maxage;
    this->m_content_format = resource.m_content_format;
    this->m_object = resource.m_object;
    this->m_op_processor = resource.m_op_processor;
}

// destructor
DynamicResource::~DynamicResource() {
}

// bind CoAP Resource...
M2MObject *DynamicResource::bind(void *p) {
	if (p != NULL) {
		this->m_object = M2MInterfaceFactory::create_object(this->getObjName().c_str());
        if (this->m_object != NULL) {
            this->m_obj_instance = this->m_object->create_object_instance();
            if (this->m_obj_instance != NULL) {
            		this->m_res = this->m_obj_instance->create_dynamic_resource(this->getResName().c_str(),this->m_res_type.c_str(),M2MResourceInstance::STRING,this->m_observable);
            		if (this->m_res != NULL) {   
            			// perform an initial get() to initialize our data value
            			this->setValue(this->get());
            			
            			// now record the data value         			
            			if (this->getDataWrapper() != NULL) {
            				// wrap the data...
							this->getDataWrapper()->wrap((uint8_t *)this->getValue().c_str(),(int)this->getValue().size());
            				this->m_res->set_operation((M2MBase::Operation)this->m_res_mask);
            				this->m_res->set_value( this->getDataWrapper()->get(),(uint8_t)this->getDataWrapper()->length());
            				this->logger()->log("%s: [%s] value: [%s] bound (observable: %d)",this->m_res_type.c_str(),this->getFullName().c_str(),this->getDataWrapper()->get(),this->m_observable);
            				this->m_op_processor = (void *)p;
        				}
        				else {
        					// do not wrap the data...
            				this->m_res->set_operation((M2MBase::Operation)this->m_res_mask);
            				this->m_res->set_value((uint8_t *)this->getValue().c_str(),(uint8_t)this->getValue().size());
             				this->logger()->log("%s: [%s] value: [%s] bound (observable: %d)",this->m_res_type.c_str(),this->getFullName().c_str(),this->getValue().c_str(),this->m_observable);
             				this->m_op_processor = (void *)p;
        				}
        				
        				// For POST-enabled RESOURCES (only...), we must add a callback
        				if ((this->m_res_mask & M2MBase::POST_ALLOWED) != 0) {
        					// add a callback for the execute function...we will just direct through process()...
        					this->m_res->set_execute_function(execute_callback(this, &DynamicResource::process_resource_post));
        				}
        			}
        			else {
        				// create_dynamic_resource() failed
        				this->logger()->log("%s: Unable to create dynamic resource...",this->m_res_type.c_str());
             			delete this->m_object;
             			this->m_object = NULL;
        			}
             }
             else {
             		// create_object_instance() failed...
             		this->logger()->log("%s: Unable to create object instance...",this->m_res_type.c_str());
             		delete this->m_object;
             		this->m_object = NULL;
             }
       }
       else {
       		// create_object() failed
       		this->logger()->log("%s: Unable to create object...",this->m_res_type.c_str());
       }
	}
	else {
      	 this->logger()->log("%s: NULL value parameter in bind() request...",this->m_res_type.c_str());
    }
    return this->m_object;
}

// get our M2MBase representation
M2MBase *DynamicResource::getResource() {
	return (M2MBase *)this->m_res;
}

// process inbound mbed-client POST message for a Resource
void DynamicResource::process_resource_post(void *args) {
		// just call process() for POST and Resources...
		(void)this->process(M2MBase::POST_ALLOWED,M2MBase::Resource);
}

// process inbound mbed-client message
uint8_t DynamicResource::process(M2MBase::Operation op,M2MBase::BaseType type) {
	// DEBUG
	//this->logger()->log("in %s::process()  Operation=0x0%x Type=%x%x",this->m_res_type.c_str(),op,type);
	
	// PUT() check
	if ((op & M2MBase::PUT_ALLOWED) != 0) {
		string value = this->coapDataToString(this->m_res->value(),this->m_res->value_length());
	 	this->logger()->log("%s: Calling resource(PUT) with [%s]=[%s]",this->m_res_type.c_str(),this->getFullName().c_str(),value.c_str());
     	this->put(value.c_str());
     	return 0;
     }
     
     // POST() check
	if ((op & M2MBase::POST_ALLOWED) != 0) {
		string value = this->coapDataToString(this->m_res->value(),this->m_res->value_length());
	 	this->logger()->log("%s: Calling resource(POST) with [%s]=[%s]",this->m_res_type.c_str(),this->getFullName().c_str(),value.c_str());
     	this->post(value.c_str());
     	return 0;
     }
     
     // DELETE() check
	if ((op & M2MBase::DELETE_ALLOWED) != 0) {
		string value = this->coapDataToString(this->m_res->value(),this->m_res->value_length());
	 	this->logger()->log("%s: Calling resource(DELETE) with [%s]=[%s]",this->m_res_type.c_str(),this->getFullName().c_str(),value.c_str());
     	this->del(value.c_str());
     	return 0;
     }
     
     // unknown type...
     this->logger()->log("%s: Unknown Operation (0x%x) for [%s]=[%s]... FAILED.",op,this->m_res_type.c_str(),this->getFullName().c_str(),this->m_res->value());
     return 1;
}

// send the notification
int DynamicResource::notify(const string data) {
    return this->notify((uint8_t *)data.c_str(),(int)data.length());
}

// send the notification
int DynamicResource::notify(uint8_t *data,int data_length) {
    uint8_t *notify_data = NULL;
    int notify_data_length = 0;
    int status = 0;

    // convert the string from the GET to something suitable for CoAP payloads
    if (this->getDataWrapper() != NULL) {
        // wrap the data...
        this->getDataWrapper()->wrap((uint8_t *)data,data_length);

        // announce (after wrap)
        //this->logger()->log("Notify payload [%s]...",this->getDataWrapper()->get());

        // fill notify
        notify_data_length = this->getDataWrapper()->length();
        notify_data = this->getDataWrapper()->get();
    }
    else {
        // announce (no wrap)
        //this->logger()->log("Notify payload [%s]...",data);

        // do not wrap the data...
        notify_data_length = data_length;
        notify_data = data;
    }
    
    // update the resource
    this->m_res->set_value((uint8_t *)notify_data,(uint8_t)notify_data_length);

    // return our status
    return status;
}

// default PUT (does nothing)
void DynamicResource::put(const string value)
{
    // not used by default
    ;
}

// default POST (does nothing)
void DynamicResource::post(const string value)
{
    // not used by default
    ;
}

// default DELETE (does nothing)
void DynamicResource::del(const string value)
{
    // not used by default
    ;
}

// default observe behavior
void DynamicResource::observe() {
    if (this->m_observable == true) {
        this->notify(this->get());
    }
}

// set the observer pointer
void DynamicResource::setObserver(void *observer) {
    this->m_observer = observer;
}

// set the content-format in responses
void DynamicResource::setContentFormat(uint8_t content_format) {
    this->m_content_format = content_format;
}

// set the max-age of responses
void DynamicResource::setMaxAge(uint8_t maxage) {
    this->m_maxage = maxage;
}

// convenience method to get the URI from its buffer field...
string DynamicResource::coapDataToString(uint8_t *coap_data_ptr,int coap_data_ptr_length)
{
    if (coap_data_ptr != NULL && coap_data_ptr_length > 0) {
        if (this->getDataWrapper() != NULL) {
            // unwrap the data...
            this->getDataWrapper()->unwrap(coap_data_ptr,coap_data_ptr_length);
            char *buf = (char *)this->getDataWrapper()->get();                  // assumes data is null terminated in DataWrapper...
            return string(buf);
        }
        else {
            // no unwrap of the data...
            char buf[MAX_VALUE_BUFFER_LENGTH+1];
            memset(buf,0,MAX_VALUE_BUFFER_LENGTH+1);
            memcpy(buf,(char *)coap_data_ptr,coap_data_ptr_length);
            return string(buf);
        }
    }
    return string("");
}
