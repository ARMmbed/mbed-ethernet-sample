/**
 * @file    OptionsBuilder.cpp
 * @brief   mbed CoAP OptionsBuilder class implementation
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

#include "mbed-connector-interface/OptionsBuilder.h"

// external included configuration file for the endpoint...
#include "mbed-connector-interface/configuration.h"

// Minar ResourceObserver support
#include "mbed-connector-interface/MinarResourceObserver.h"

// DEBUG
#ifndef NDEBUG
#define DEBUG_OUT(...) { printf(__VA_ARGS__); }
#else
#define DEBUG_OUT(...) /* nothing */
#endif

// Connector namespace
namespace Connector {

// Constructor
OptionsBuilder::OptionsBuilder()
{
    this->m_domain           		 = NSP_DOMAIN;
    this->m_endpoint_type    	 = NSP_ENDPOINT_TYPE;
    this->m_node_name        	 = NODE_NAME;
    this->m_channel_list    		 = NODE_CHANNEL_LIST;
    this->m_reg_update_period = REG_UPDATE_PERIOD_MS;
    this->m_lifetime  					 = REG_LIFETIME_SEC;
    this->m_connector_url  		 = string(CONNECTOR_URL);
    memset(this->m_mac_address,0,NODE_MAC_ADDRESS_LENGTH);
    this->m_device_resources.clear();
    this->m_static_resources.clear();
    this->m_dynamic_resources.clear();
    this->m_resource_observers.clear();
}

// Copy Constructor
OptionsBuilder::OptionsBuilder(const OptionsBuilder &ob)  : Options(ob)
{
    this->m_lifetime = ob.m_lifetime;
    this->m_domain = ob.m_domain;
    this->m_endpoint_type = ob.m_endpoint_type;
    this->m_device_resources = ob.m_device_resources;
    this->m_static_resources = ob.m_static_resources;
    this->m_dynamic_resources = ob.m_dynamic_resources;
    this->m_resource_observers = ob.m_resource_observers;
}

// Destructor
OptionsBuilder::~OptionsBuilder()
{
	this->m_device_resources.clear();
    this->m_static_resources.clear();
    this->m_dynamic_resources.clear();
    this->m_resource_observers.clear();
}

// set lifetime
OptionsBuilder &OptionsBuilder::setLifetime(int lifetime)
{
    this->m_lifetime = lifetime;
    return *this;
}

// set domain
OptionsBuilder &OptionsBuilder::setDomain(const char *domain)
{
    this->m_domain = string(domain);
    return *this;
}

// set endpoint nodename
OptionsBuilder &OptionsBuilder::setEndpointNodename(const char *node_name)
{
    this->m_node_name = string(node_name);
    return *this;
}

// set lifetime
OptionsBuilder &OptionsBuilder::setEndpointType(const char *endpoint_type)
{
    this->m_endpoint_type = string(endpoint_type);
    return *this;
}

// set low level radio channel list
OptionsBuilder &OptionsBuilder::setRadioChannelList(const uint32_t channel_list)
{
    this->m_channel_list = channel_list;
    return *this;
}

// set mbed registration update period
OptionsBuilder &OptionsBuilder::setRegUpdatePeriod(const int reg_update_period)
{
    this->m_reg_update_period = reg_update_period;
    return *this;
}

// set Connector URL
OptionsBuilder &OptionsBuilder::setConnectorURL(const char *connector_url)
{
	if (connector_url != NULL) {
		this->m_connector_url  = string(connector_url);
	}
    return *this;
}

// set MAC address
OptionsBuilder &OptionsBuilder::setMACAddress(const uint8_t *mac_address,const int mac_address_length)
{
    if (mac_address != NULL && mac_address_length > 0) {
        int length = mac_address_length;
        if (length > NODE_MAC_ADDRESS_LENGTH) length = NODE_MAC_ADDRESS_LENGTH;
        for(int i=0; i<length; ++i) this->m_mac_address[i] = mac_address[i];
    }
    return *this;
}

// add a device resource
OptionsBuilder &OptionsBuilder::addResource(const DeviceResource *resource) {
	if (resource != NULL) {
        ((DeviceResource *)resource)->setOptions(this);
        this->m_device_resources.push_back((DeviceResource *)resource);
    }
    return *this;
}

// add static resource
OptionsBuilder &OptionsBuilder::addResource(const StaticResource *resource)
{
    if (resource != NULL) {
        ((StaticResource *)resource)->setOptions(this);
        this->m_static_resources.push_back((StaticResource *)resource);
    }
    return *this;
}

// add dynamic resource
OptionsBuilder &OptionsBuilder::addResource(const DynamicResource *resource)
{
    // ensure that the boolean isn't mistaken by the compiler for the obs period...
    return this->addResource(resource,OBS_PERIOD_MS,!(((DynamicResource *)resource)->implementsObservation()));
}

// add dynamic resource
OptionsBuilder &OptionsBuilder::addResource(const DynamicResource *resource,const int sleep_time)
{
    // ensure that the boolean isn't mistaken by the compiler for the obs period...
    return this->addResource(resource,sleep_time,!(((DynamicResource *)resource)->implementsObservation()));
}

// add dynamic resource
OptionsBuilder &OptionsBuilder::addResource(const DynamicResource *resource,const bool use_observer)
{
    // ensure that the boolean isn't mistaken by the compiler for the obs period...
    return this->addResource(resource,OBS_PERIOD_MS,use_observer);
}

// add dynamic resource
OptionsBuilder &OptionsBuilder::addResource(const DynamicResource *resource,const int sleep_time,const bool use_observer)
{
    if (resource != NULL) {
        this->m_dynamic_resources.push_back((DynamicResource *)resource);
        ((DynamicResource *)resource)->setOptions(this);
        if (((DynamicResource *)resource)->isObservable() == true && use_observer == true) {
            MinarResourceObserver *observer = new MinarResourceObserver((DynamicResource *)resource,(int)sleep_time);
            this->m_resource_observers.push_back(observer);

            // immedate observation enablement option
            if (this->immedateObservationEnabled()) {
                observer->beginObservation();
            }
        }
    }
    return *this;
}

// set WiFi SSID
OptionsBuilder &OptionsBuilder::setWiFiSSID(char *ssid)
{
    this->m_wifi_ssid = string(ssid);
    return *this;
}

// set WiFi AuthType
OptionsBuilder &OptionsBuilder::setWiFiAuthType(WiFiAuthTypes auth_type)
{
    this->m_wifi_auth_type = auth_type;
    return *this;
}

// set WiFi AuthKey
OptionsBuilder &OptionsBuilder::setWiFiAuthKey(char *auth_key)
{
    this->m_wifi_auth_key = string(auth_key);
    return *this;
}

// set 802.15.4 Network ID
OptionsBuilder &OptionsBuilder::setNetworkID(char *network_id)
{
    this->m_network_id = string(network_id);
    return *this;
}

// set 802.15.4 Radio Channel
OptionsBuilder &OptionsBuilder::setRadioChannel(int channel)
{
    this->m_channel = (uint8_t)channel;
    return *this;
}

// build out our immutable self
Options *OptionsBuilder::build()
{
    return (Options *)this;
}

// Enable/Disable immediate observationing
OptionsBuilder &OptionsBuilder::setImmedateObservationEnabled(bool enable) {
    this->m_enable_immediate_observation = enable;
    return *this;
}

// Enable/Disable GET-based control of observations
OptionsBuilder &OptionsBuilder::setEnableGETObservationControl(bool enable) {
    this->m_enable_get_obs_control = enable;
    return *this;
}

// set the server certificate
OptionsBuilder &OptionsBuilder::setServerCertificate(uint8_t cert[],int cert_size) {
    memset(this->m_server_cert,0,MAX_SERVER_CERT_LENGTH);
    int length = cert_size;
    if (length > MAX_SERVER_CERT_LENGTH) {
    	length = MAX_SERVER_CERT_LENGTH;
    	DEBUG_OUT("WARNING: Truncated Server Certificate: orig: %d bytes (trunc: %d bytes)\r\n",cert_size,length);
    }
    memcpy(this->m_server_cert,cert,length);
    this->m_server_cert_length = length;
	return *this;
}

// set the client certificate
OptionsBuilder &OptionsBuilder::setClientCertificate(uint8_t cert[],int cert_size) {
	memset(this->m_client_cert,0,MAX_CLIENT_CERT_LENGTH);
    int length = cert_size;
    if (length > MAX_CLIENT_CERT_LENGTH) {
    	length = MAX_CLIENT_CERT_LENGTH;
    	DEBUG_OUT("WARNING: Truncated Client Certificate: orig: %d bytes (trunc: %d bytes)\r\n",cert_size,length);
    }
    memcpy(this->m_client_cert,cert,length);
    this->m_client_cert_length = length;
	return *this;
}

// set the client key
OptionsBuilder &OptionsBuilder::setClientKey(uint8_t key[],int key_size) {
	memset(this->m_client_key,0,MAX_CLIENT_KEY_LENGTH);
    int length = key_size;
    if (length > MAX_CLIENT_KEY_LENGTH) {
    	length = MAX_CLIENT_KEY_LENGTH;
    	DEBUG_OUT("WARNING: Truncated Client Key: orig: %d bytes (trunc: %d bytes)\r\n",key_size,length);
    }
    memcpy(this->m_client_key,key,length);
    this->m_client_key_length = length;
	return *this;
}

} // namespace Connector