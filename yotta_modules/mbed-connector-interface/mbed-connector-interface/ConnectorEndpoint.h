/**
 * @file    Endpoint.h
 * @brief   mbed CoAP Endpoint base class
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

#ifndef __CONNECTOR_ENDPOINT_H__
#define __CONNECTOR_ENDPOINT_H__

// mbed-client support
#include "mbed-client/m2minterfacefactory.h"
#include "mbed-client/m2mdevice.h"
#include "mbed-client/m2minterfaceobserver.h"
#include "mbed-client/m2minterface.h"
#include "mbed-client/m2mobjectinstance.h"
#include "mbed-client/m2mresource.h"

// Support for Logging/Debug output
#include "mbed-connector-interface/Logger.h"

// Options support
#include "mbed-connector-interface/Options.h"

// Connector namespace
namespace Connector  {

/** Endpoint class
 */
class Endpoint : public M2MInterfaceObserver  {

public:
    /**
    Default Constructor
    */
    Endpoint(const Logger *logger,const Options *ob);

    /**
    Copy Constructor
    @param ob input endpoint instance to deep copy
    */
    Endpoint(const Endpoint &ep);

    /**
    Destructor
    */
    virtual ~Endpoint();

    /**
    Register the endpoint with mbed Device Server
    */
    void register_endpoint();

    /**
    Plumb the lower RF network stack
    @param canActAsRouterNode input boolean indicating whether this node can act as a router node or not.
    */
    static void plumbNetwork(bool canActAsRouterNode = false);

    /**
    Initialize the endpoint's configuration and begin the endpoint's main even loop
    */
    static void start();
    
    //  mbed-client : bootstrap done
	virtual void bootstrap_done(M2MSecurity *server_object);

	//  mbed-client : object registered
    virtual void object_registered(M2MSecurity *security_object, const M2MServer &server_object);

	//  mbed-client : object unregistered
	virtual void object_unregistered(M2MSecurity *server_object);

	//  mbed-client : registration updated
	virtual void registration_updated(M2MSecurity *security_object, const M2MServer & server_object) ;

	//  mbed-client : resource value updated
	virtual void value_updated(M2MBase *base, M2MBase::BaseType type) ;
	
	//  mbed-client : error handler
	virtual void error(M2MInterface::Error error);
	
	// mbed-client : complete registration of the endpoint
	void complete_endpoint_registration(M2MSecurity *register_object, M2MObjectList object_list);
	
	// mbed-client: re-register endpoint
	void re_register_endpoint();
	
	// mbed-client: de-register endpoint and stop scheduling
	void closedown_endpoint(void);
	
	// get our Options
	void setOptions(Options *options);
	
	// get our Options
	Options *getOptions();
	
	// get our Server
	M2MSecurity *getServer();
	
	// get our Object List
	M2MObjectList  getObjectList();
	
	// configure to act as router node
	void asRouterNode(bool canActAsRouterNode);

private:
    Logger            *m_logger;
    Options           *m_options;
    bool			         m_canActAsRouterNode;
    
    // mbed-client support
    M2MInterface   *m_interface;
    M2MSecurity    *m_server_instance;
    M2MObjectList  m_object_list;
    M2MDevice      *m_device_object;
    
    // mbed-client methods
    void create_interface();
    M2MSecurity *create_server_instance();
	
	// DynamicResource Lookup
	DynamicResource *lookupDynamicResource(M2MBase *base);
	
    // access the logger
    Logger *logger();
};

} // namespace Connector

#endif // __CONNECTOR_ENDPOINT_H__
