/**
 * @file    Location.h
 * @brief   mbed CoAP Endpoint location base class
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

#ifndef __LOCATION_H__
#define __LOCATION_H__

// mbed API
#include "mbed-drivers/mbed.h"

// Max length of a coordinate:  -XXX.YYYYYY
#define LOCATION_COORDINATE_LENGTH 12

// Max length of the MSL altitude (m): ZZZZZZ.Z
#define LOCATION_MSL_ALT_LENGTH    9

// Max length of the speed (km/h): ZZZZZZ.Z
#define LOCATION_SPEED_LENGTH      9

namespace Connector {
    
/** Location class
 */
class Location
{
protected:
    RawSerial *m_pc;
    char      m_latitude[LOCATION_COORDINATE_LENGTH+1];
    char      m_longitude[LOCATION_COORDINATE_LENGTH+1];
    char      m_msl_altitude_m[LOCATION_MSL_ALT_LENGTH+1];
    char      m_speed[LOCATION_SPEED_LENGTH+1];
    
public:
    /**
    Default constructor
    @param pc input BufferedSerial instance for debugging (if NULL, no debugging output will occur in the library)
    */
    Location(const RawSerial *pc);

    /**
    Copy constructor
    @param logger input Location instance to deep copy
    */
    Location(const Location &location);

    /**
    Destructor
    */
    virtual ~Location();

    /**
    Update the current location (pure virtual)
    */
    virtual void updateLocation() = 0;
    
    /**
    Get latest Latitude
    */
    virtual char *getLatitude();
    
    /**
    Get latest Longitude
    */
    virtual char *getLongitude();
    
    /**
    Get latest MSL Altitude (m)
    */
    char *getMSLAltitude();
    
    /**
    Get latest Speed (km/h) (pure virtual)
    */
    char *getSpeed();
    
protected:
    /**
    Init buffers
    */
    void initBuffers();
};

};

#endif // __LOCATION_H__
