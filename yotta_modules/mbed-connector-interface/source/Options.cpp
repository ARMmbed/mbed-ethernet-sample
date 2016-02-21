/**
 * @file    Options.cpp
 * @brief   mbed CoAP Options (immutable OptionsBuilder instance) class implementation
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

#include "mbed-connector-interface/Options.h"

namespace Connector {

// default constructor
Options::Options()
{
}

// copy constructor
Options::Options(const Options &opt)
{
}

// destructors
Options::~Options()
{
}

// lifetime pointer
int Options::getLifetime()
{
    return this->m_lifetime;
}

// NSP domain
string Options::getDomain()
{
    return this->m_domain;
}

// Endpoint name
string Options::getEndpointNodename()
{
    return this->m_node_name;
}

// Endpoint Type
string Options::getEndpointType()
{
    return this->m_endpoint_type;
}

// NODE Radio Channel List
uint32_t Options::getRadioChannelList()
{
    return this->m_channel_list;
}

// mbed Registation update period
int Options::getRegUpdatePeriod()
{
    return this->m_reg_update_period;
}

// Connector URL
char  *Options::getConnectorURL()
{
    return (char *)this->m_connector_url.c_str();
}

// NODE MAC Address
uint8_t *Options::getMACAddress()
{
    return this->m_mac_address;
}

// Device Resources
DeviceResourcesList *Options::getDeviceResourceList()
{
    return &this->m_device_resources;
}

// Static Resources
StaticResourcesList *Options::getStaticResourceList()
{
    return &this->m_static_resources;
}

// Dynamic Resources
DynamicResourcesList *Options::getDynamicResourceList()
{
    return &this->m_dynamic_resources;
}

// WiFi SSID
string Options::getWiFiSSID() {
    return this->m_wifi_ssid;
}

// WiFi AuthType
WiFiAuthTypes Options::getWiFiAuthType() {
    return this->m_wifi_auth_type;
}

// WiFi AuthKey
string Options::getWiFiAuthKey() {
    return this->m_wifi_auth_key;
}

// 802.15.4 Network ID
string Options::getNetworkID() {
    return this->m_network_id;
}

// 802.15.4 Network ID
uint8_t Options::getRadioChannel() {
    return this->m_channel;
}

// Immediate Observationing Enabled
bool Options::immedateObservationEnabled() {
    return this->m_enable_immediate_observation;
}

// Enable/Disable Observation control via GET
bool Options::enableGETObservationControl() {
    return this->m_enable_get_obs_control;
}

// Get the Server Certificate
uint8_t *Options::getServerCertificate() {
	return (uint8_t *) this->m_server_cert;
}

// Get the Server Certificate length
int Options::getServerCertificateSize() {
	return this->m_server_cert_length;
}

// Get the Client Certificate
uint8_t *Options::getClientCertificate() {
	return (uint8_t *) this->m_client_cert;
}

// Get the Client Certificate length
int Options::getClientCertificateSize() {
	return this->m_client_cert_length;
}

// Get the Client Key
uint8_t *Options::getClientKey() {
	return (uint8_t *) this->m_client_key;
}

// Get the Client Key length
int Options::getClientKeySize() {
	return this->m_client_key_length;
}

} // namespace Connector
