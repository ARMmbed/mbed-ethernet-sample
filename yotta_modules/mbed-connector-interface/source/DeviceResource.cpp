/**
 * @file    DeviceResource.h
 * @brief   mbed CoAP Endpoint Device Resource class
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
 
 // configuration
 #include "mbed-connector-interface/mbedConnectorInterface.h"
 
 // mbed-client support
 #include "mbed-client/m2mconstants.h"
 
 // BaseClass 
 #include "mbed-connector-interface/DeviceResource.h"
 
// Constructor
DeviceResource::DeviceResource(const Logger *logger,M2MDevice::DeviceResource name, const char *value) : Resource<string>(logger,string(M2M_DEVICE_RES_ROOT),this->resource_name(name),string(value)), m_data_wrapper()
{
	this->m_object = NULL;
	this->m_dev_name = name;
}

// Constructor
DeviceResource::DeviceResource(const Logger *logger,M2MDevice::DeviceResource name,const string value) : Resource<string>(logger,string(M2M_DEVICE_RES_ROOT),this->resource_name(name),string(value))
{
    this->m_data_wrapper = NULL;
    this->m_object = NULL;
    this->m_dev_name = name;
}

// Copy constructor
DeviceResource::DeviceResource(const DeviceResource &resource) : Resource<string>((const Resource<string> &)resource)
{
    this->m_data_wrapper = resource.m_data_wrapper;
    this->m_object = resource.m_object;
    this->m_dev_name = resource.m_dev_name;
}

// Destructor
DeviceResource::~DeviceResource() {
}

// bind CoAP Resource..
M2MObject *DeviceResource::bind(void *p) {
	if (p != NULL) {
	    M2MDevice *resource = M2MInterfaceFactory::create_device();
    	    if (resource != NULL) {
    		if (this->getDataWrapper() != NULL) {
            	    // wrap the data...
            	    this->getDataWrapper()->wrap((uint8_t *)this->getValue().c_str(),(int)this->getValue().size());
            	    string str_value = string((char *)this->getDataWrapper()->get());
            	    resource->create_resource(this->m_dev_name,str_value.c_str());
            	    this->logger()->log("DeviceResource: [%s] value: [%s] bound",this->getFullName().c_str(),this->getDataWrapper()->get());
        	}
        	else {
        			// do not wrap the data...
            	    resource->create_resource(this->m_dev_name,this->getValue().c_str());
             	    this->logger()->log("DeviceResource: [%s] value: [%s] bound",this->getFullName().c_str(),this->getValue().c_str());
        	}
        	this->m_object = (M2MObject *)resource;
    	    }
    	    else {
    		 this->logger()->log("DeviceResource: M2MInterfaceFactory::create_device() FAILED");
            }
        }
        else {
    	    this->logger()->log("DeviceResource: NULL value parameter in bind() request...");
        }
        return this->m_object;
}

// no reason why this should not be public in M2MDevice... 
string DeviceResource::resource_name(M2MDevice::DeviceResource name)  const {
    m2m::String res_name = "";
    switch(name) {
        case M2MDevice::Manufacturer:
            res_name = DEVICE_MANUFACTURER;
            break;
        case M2MDevice::DeviceType:
            res_name = DEVICE_DEVICE_TYPE;
            break;
        case M2MDevice::ModelNumber:
            res_name = DEVICE_MODEL_NUMBER;
            break;
        case M2MDevice::SerialNumber:
            res_name = DEVICE_SERIAL_NUMBER;
            break;
        case M2MDevice::HardwareVersion:
            res_name = DEVICE_HARDWARE_VERSION;
            break;
        case M2MDevice::FirmwareVersion:
            res_name = DEVICE_FIRMWARE_VERSION;
            break;
        case M2MDevice::SoftwareVersion:
            res_name = DEVICE_SOFTWARE_VERSION;
            break;
        case M2MDevice::Reboot:
            res_name = DEVICE_REBOOT;
            break;
        case M2MDevice::FactoryReset:
            res_name = DEVICE_FACTORY_RESET;
            break;
        case M2MDevice::AvailablePowerSources:
            res_name = DEVICE_AVAILABLE_POWER_SOURCES;
            break;
        case M2MDevice::PowerSourceVoltage:
            res_name = DEVICE_POWER_SOURCE_VOLTAGE;
            break;
        case M2MDevice::PowerSourceCurrent:
            res_name = DEVICE_POWER_SOURCE_CURRENT;
            break;
        case M2MDevice::BatteryLevel:
            res_name = DEVICE_BATTERY_LEVEL;
            break;
        case M2MDevice::BatteryStatus:
            res_name = DEVICE_BATTERY_STATUS;
            break;
        case M2MDevice::MemoryFree:
            res_name = DEVICE_MEMORY_FREE;
            break;
        case M2MDevice::MemoryTotal:
            res_name = DEVICE_MEMORY_TOTAL;
            break;
        case M2MDevice::ErrorCode:
            res_name = DEVICE_ERROR_CODE;
            break;
        case M2MDevice::ResetErrorCode:
            res_name = DEVICE_RESET_ERROR_CODE;
            break;
        case M2MDevice::CurrentTime:
            res_name = DEVICE_CURRENT_TIME;
            break;
        case M2MDevice::UTCOffset:
            res_name = DEVICE_UTC_OFFSET;
            break;
        case M2MDevice::Timezone:
            res_name = DEVICE_TIMEZONE;
            break;
        case M2MDevice::SupportedBindingMode:
            res_name = DEVICE_SUPPORTED_BINDING_MODE;
            break;
    }
    string result = string("") + res_name.c_str();
    return result;
}
