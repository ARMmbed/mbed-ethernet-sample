/**
 * @file    Location.cpp
 * @brief   mbed CoAP Endpoint Location base class
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

#include "mbed-connector-interface/Location.h"

namespace Connector {

// Constructor
Location::Location(const RawSerial *pc) {
    this->m_pc = (RawSerial *)pc;
    this->initBuffers();
}

// Copy Constructor
Location::Location(const Location &location) {
    this->m_pc = location.m_pc;
    memcpy(this->m_latitude,location.m_latitude,LOCATION_COORDINATE_LENGTH+1);
    memcpy(this->m_longitude,location.m_longitude,LOCATION_COORDINATE_LENGTH+1);
    memcpy(this->m_msl_altitude_m,location.m_msl_altitude_m,LOCATION_MSL_ALT_LENGTH+1);
    memcpy(this->m_speed,location.m_speed,LOCATION_SPEED_LENGTH+1);
}

// Destructor
Location::~Location() {
}

// init the buffers
void Location::initBuffers() {
    memset(this->m_latitude,0,LOCATION_COORDINATE_LENGTH+1);
    memset(this->m_longitude,0,LOCATION_COORDINATE_LENGTH+1);
    memset(this->m_msl_altitude_m,0,LOCATION_MSL_ALT_LENGTH+1);
    memset(this->m_speed,0,LOCATION_SPEED_LENGTH+1);
}

// get the latitude
char *Location::getLatitude() {
    return this->m_latitude;
}

// get the longitude
char *Location::getLongitude() {
    return this->m_longitude;
}

// get the MSL Altitude
char *Location::getMSLAltitude() {
    return this->m_msl_altitude_m;
}

// get the Speed
char *Location::getSpeed() {
    return this->m_speed;
}

};
