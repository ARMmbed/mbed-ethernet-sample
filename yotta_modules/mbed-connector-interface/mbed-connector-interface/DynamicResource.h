/**
 * @file    DynamicResource.h
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

#ifndef __DYNAMIC_RESOURCE_H__
#define __DYNAMIC_RESOURCE_H__

// Base Class
#include "mbed-connector-interface/Resource.h"

// DataWrapper support
#include "mbed-connector-interface/DataWrapper.h"

/** DynamicResource class
 */
class DynamicResource : public Resource<string>
{
public:
    /**
    Default constructor (char strings)
    @param logger input logger instance for this resource
    @param obj_name input the Object
    @param name input the Resource URI/Name
    @param res_type input type for the Resource
    @param res_mask input the resource enablement mask (GET, PUT, etc...)
    @param observable input the resource is Observable (default: FALSE)
    */
    DynamicResource(const Logger *logger,const char *obj_name,const char *res_name,const char *res_type,uint8_t res_mask,const bool observable = false);

    /**
    Default constructor (char strings)
    @param logger input logger instance for this resource
    @param obj_name input the Object
    @param name input the Resource URI/Name
    @param res_type input type for the Resource
    @param value input initial value for the Resource
    @param res_mask input the resource enablement mask (GET, PUT, etc...)
    @param observable input the resource is Observable (default: FALSE)
    */
    DynamicResource(const Logger *logger,const char *obj_name,const char *res_name,const char *res_type,const string value,uint8_t res_mask,const bool observable = false);

    /**
    constructor with string buffer for name
    @param logger input logger instance for this resource
    @param obj_name input the Object
    @param name input the Resource URI/Name
    @param res_type input type for the Resource
    @param value input initial value for the Resource
    @param res_mask input the resource enablement mask (GET, PUT, etc...)
    @param observable input the resource is Observable (default: FALSE)
    */
    DynamicResource(const Logger *logger,const string obj_name,const string res_name,const string res_type,const string value,uint8_t res_mask,const bool observable = false);

    /**
    Copy constructor
    @param resource input the DynamicResource that is to be deep copied
    */
    DynamicResource(const DynamicResource &resource);

    /**
    Destructor
    */
    virtual ~DynamicResource();

    /**
    Bind resource to endpoint
    @param p input pointer to the endpoint resources necessary for binding
    */
    virtual M2MObject *bind(void *p);

    /**
    Process the CoAP message
    @param op input the CoAP Verb (operation)
    @param type input clarification of the M2MBase instance being passed in (Object vs. ObjectInstance vs. Resource vs. ResourceInstance)
    @return 0 - success, 1 - failure
    */
    uint8_t process(M2MBase::Operation op,M2MBase::BaseType type);

    /**
    Resource value getter (REQUIRED: must be implemented in derived class as all Binders MUST support and implement GET)
    @returns string value of the resource
    */
    virtual string get() = 0;

    /**
    Resource value setter (PUT) (OPTIONAL: defaulted noop if not derived. Binders MAY implement PUT if needed)
    @param string value of the resource
    */
    virtual void put(const string value);

    /**
    Resource value setter (POST) (OPTIONAL: defaulted noop if not derived. Binders MAY implement POST if needed)
    @param string value of the resource
    */
    virtual void post(const string value);

    /**
    Resource value deleter (OPTIONAL: defaulted noop if not derived. Binders MAY implement DELETE if needed)
    @param string value of the resource
    */
    virtual void del(const string value);

    /**
    Send notification of new data
    @param data input the new data to update
    @returns 1 - success, 0 - failure
    */
    int notify(const string data);

    /**
    Determine whether this dynamic resource is observable or not
    @returns true - is observable, false - otherwise
    */
    bool isObservable() { return this->m_observable; }

    /**
    Set the observer pointer
    @param observer input the pointer to the ResourceObserver observing this resource
    */
    void setObserver(void *observer);

    /**
    Set the content format for responses
    @param content_format short integer CoAP content-format ID
    */
    void setContentFormat(uint8_t content_format);

    /**
    Set the max-age for cache control of responses in a proxy cache
    @param maxage short integer CoAP max-age in seconds
    */
    void setMaxAge(uint8_t maxage);

    /**
    Set the data wrapper
    @param data_wrapper input the data wrapper instance
    */
    void setDataWrapper(DataWrapper *data_wrapper) { this->m_data_wrapper = data_wrapper; }

    /**
    observe the resource
    */
    virtual void observe();
    
    /**
    get the base resource representation
    */
    M2MBase *getResource();
    
    /**
    Process a POST message for Resources
    */
    void process_resource_post(void *args);

protected:
    int notify(uint8_t *data,int data_length);
    DataWrapper *getDataWrapper() { return this->m_data_wrapper; }
    bool              m_observable;

private:

    string                 			   m_res_type;
    uint8_t               			   m_res_mask;
    uint8_t               			   m_obs_number;
    DataWrapper   			  *m_data_wrapper;
    void                  		      *m_observer;
    uint8_t               			    m_maxage;
    uint8_t               			    m_content_format;
    M2MObject      			   *m_object; 
    M2MObjectInstance       *m_obj_instance;
    M2MResource				   *m_res;
    void                                *m_op_processor;

    // convenience method to create a string from the NSDL CoAP data buffers...
    string coapDataToString(uint8_t *coap_data_ptr,int coap_data_ptr_length);
};

#endif // __DYNAMIC_RESOURCE_H__
