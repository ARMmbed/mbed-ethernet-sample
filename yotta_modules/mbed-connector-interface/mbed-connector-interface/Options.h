/**
 * @file    Options.h
 * @brief   mbed CoAP Options (immutable OptionsBuilder instance) class header
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

#ifndef __OPTIONS_H__
#define __OPTIONS_H__

// Device Resources
#include "mbed-connector-interface/DeviceResource.h"

// Static Resources
#include "mbed-connector-interface/StaticResource.h"

// Dynamic Resources
#include "mbed-connector-interface/DynamicResource.h"

// include the mbed connector resource list
#include "mbed-connector-interface/mbedConnectorInterface.h"

// include the resource observer includes here so that they are not required in main.cpp
#include "mbed-connector-interface/MinarResourceObserver.h"

// Vector support
#include <vector>

// Resources list
typedef vector<DeviceResource *> DeviceResourcesList;
typedef vector<StaticResource *> StaticResourcesList;
typedef vector<DynamicResource *> DynamicResourcesList;
typedef vector<ResourceObserver *> ResourceObserversList;

// WiFi Security types
typedef enum {
    WIFI_WPA_PERSONAL,
    WIFI_WPA2_PERSONAL,
    WIFI_WEP,
    WIFI_NUM_TYPES
} WiFiAuthTypes;

namespace Connector {

/** Options class
 */
class Options
{
public:
    /**
    Default constructor
    */
    Options();

    /**
    Copy constructor
    */
    Options(const Options &opt);

    /**
    Destructor
    */
    virtual ~Options();

    /**
    Get the node lifetime
    */
   int getLifetime();

    /**
    Get the NSP domain
    */
    string getDomain();

    /**
    Get the node name
    */
    string getEndpointNodename();

    /**
    Get the node type
    */
    string getEndpointType();

    /**
    Get the node radio channel list
    */
    uint32_t getRadioChannelList();

    /**
    Get the mbed registration update period
    */
    int getRegUpdatePeriod();
    
    /**
    Get the endpoint Connector URL
    */
    char *getConnectorURL();

    /**
    Get the node MAC address
    */
    uint8_t *getMACAddress();

	/**
    Get the list of device resources
    */
    DeviceResourcesList *getDeviceResourceList();

    /**
    Get the list of static resources
    */
    StaticResourcesList *getStaticResourceList();

    /**
    Get the list of dynamic resources
    */
    DynamicResourcesList *getDynamicResourceList();

    /**
    Get the WiFi SSID
    */
    string getWiFiSSID();

    /**
    Get the WiFi Auth Type
    */
    WiFiAuthTypes getWiFiAuthType();

    /**
    Get the WiFi Auth Key
    */
    string getWiFiAuthKey();

    /**
    Get the 802.15.4 Network ID
    */
    string getNetworkID();

    /**
    Get the 802.15.4 Radio channel
    */
    uint8_t getRadioChannel();

    /**
    Enable/Disable Immediate Observationing
    */
    bool immedateObservationEnabled();

    /**
    Enable/Disable Observation control via GET
    */
    bool enableGETObservationControl();

	 /**
    Get the Server Certificate
    */
    uint8_t *getServerCertificate();
    
    /**
    Get the Server Certificate Size (bytes)
    */
    int getServerCertificateSize();
    
     /**
    Get the Client Certificate
    */
    uint8_t *getClientCertificate();
    
    /**
    Get the Client Certificate Size (bytes)
    */
    int getClientCertificateSize();
    
     /**
    Get the Client Key
    */
    uint8_t *getClientKey();
    
    /**
    Get the Client Key Size (bytes)
    */
    int getClientKeySize();
    
protected:
    // mDS Resources
    int                     	    m_lifetime;
    string               			m_domain;
    string               			m_node_name;
    string               			m_endpoint_type;
    int                    			m_reg_update_period;
    string						m_connector_url;

    // WiFi Resources
    string               			m_wifi_ssid;
    string               			m_wifi_auth_key;
    WiFiAuthTypes        m_wifi_auth_type;

    // 802.15.4 Resources
    string               			m_network_id;
    uint32_t             		m_channel_list;
    uint8_t              		m_mac_address[NODE_MAC_ADDRESS_LENGTH];
    uint8_t              		m_channel;
    
    // DTLS/TLS Resources
    uint8_t 						m_server_cert[MAX_SERVER_CERT_LENGTH];
    int 				    		m_server_cert_length;
    uint8_t 						m_client_cert[MAX_CLIENT_CERT_LENGTH];
    int 				    		m_client_cert_length;
    uint8_t 						m_client_key[MAX_CLIENT_KEY_LENGTH];
    int 				    		m_client_key_length;

    // CoAP behavior adjustments
    bool                		 	m_enable_immediate_observation;
    bool                 		 	m_enable_get_obs_control;

    // Endpoint Resources
    DeviceResourcesList   	m_device_resources;
    StaticResourcesList   		m_static_resources;
    DynamicResourcesList  	m_dynamic_resources;
    ResourceObserversList 	m_resource_observers;
};

} // namespace Connector

#endif // __OPTIONS_H__
