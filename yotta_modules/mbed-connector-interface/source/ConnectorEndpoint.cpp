/**
 * @file    ConnectorEndpoint.cpp
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
 
 // Lower level Network
#include "mbed-connector-interface/mbedEndpointNetwork.h"

// ConnectorEndpoint
#include "mbed-connector-interface/ConnectorEndpoint.h"

// Utils support
#include "mbed-connector-interface/Utils.h"

// DEBUG
#ifndef NDEBUG
#define DEBUG_OUT(...) { printf(__VA_ARGS__); }
#else
#define DEBUG_OUT(...) /* nothing */
#endif

// our endpoint instance
static Connector::Endpoint *__endpoint = NULL;

// Connector namespace
namespace Connector {

// STATIC: Plumb the network
void Endpoint::plumbNetwork(bool canActAsRouterNode) {
	if (__endpoint == NULL) {
		// initialize our endpoint instance
		DEBUG_OUT("Endpoint::plumbNetwork: initializing endpoint instance...\r\n");
		__endpoint = (Connector::Endpoint *)utils_init_endpoint(canActAsRouterNode);
	}

    // configure the endpoint now...
    DEBUG_OUT("Endpoint::plumbNetwork: configuring endpoint...\r\n");
    utils_configure_endpoint((void *)__endpoint);
    
    // plumb network
	DEBUG_OUT("Endpoint::plumbNetwork: plumbing network...\r\n");
	net_stubs_plumb_network((void *)__endpoint);
}

// STATIC: Finalize the endpoint's configuration and begin the endpoint's main even loop (static, not tied into Logger)
void Endpoint::start()
{
    // complete our registration setup...
    DEBUG_OUT("Endpoint::start: registering endpoint with Connector...\r\n");
    utils_register_endpoint((void *)__endpoint);
    
    // Initialize our main loop...
    DEBUG_OUT("Endpoint::start: initializing main loop for endpoint...\r\n"); 
    net_stubs_create_main_loop(__endpoint);
    
    // Begin the main loop
    DEBUG_OUT("Endpoint::start: entering main loop in endpoint...\r\n");
    net_stubs_begin_main_loop(__endpoint);
}

// Constructor
Endpoint::Endpoint(const Logger *logger, const Options *options)  : M2MInterfaceObserver() 
{
    this->m_logger = (Logger *)logger;
    this->m_options = (Options *)options;
}

// Copy Constructor
Endpoint::Endpoint(const Endpoint &ep)
{
    this->m_logger = ep.m_logger;
    this->m_options = ep.m_options;
    this->m_interface = ep.m_interface;
    this->m_server_instance = ep.m_server_instance;
    this->m_object_list = ep.m_object_list;
    this->m_device_object = ep.m_device_object;
}

// Destructor
Endpoint::~Endpoint() {
	if (this->m_interface != NULL)
		delete this->m_interface;
	if (this->m_server_instance != NULL)
		delete this->m_server_instance;
}

// router node behavior setting
void Endpoint::asRouterNode(bool canActAsRouterNode) {
	this->m_canActAsRouterNode = canActAsRouterNode;
}

// set our Options
void Endpoint::setOptions(Options *options) {
	this->m_options = options;
}

// get our Options
Options *Endpoint::getOptions() {
	return this->m_options;
}

// get our Server
M2MSecurity *Endpoint::getServer() {
	return this->m_server_instance;
}

// get our ObjectList
M2MObjectList Endpoint::getObjectList() {
	return this->m_object_list;
}

// mbed-client: create our interface
void Endpoint::create_interface() {
		// Randomizing listening port for Certificate mode connectivity
		srand(time(NULL));
		uint16_t port = rand() % 65535 + 12345;
        this->m_interface = M2MInterfaceFactory::create_interface(*this,
                                                  (char *)this->m_options->getEndpointNodename().c_str(),
                                                  (char *)this->m_options->getEndpointType().c_str(),
                                                  (int32_t)this->m_options->getLifetime(),
                                                  port,
                                                  (char *)this->m_options->getDomain().c_str(),
                                                  M2MInterface::UDP,
                                                  M2MInterface::LwIP_IPv4);
}

// mbed-client: create_server_instance()
M2MSecurity *Endpoint::create_server_instance()  {
        // Creates register server object with mbed device server address and other parameters
        M2MSecurity *server = M2MInterfaceFactory::create_security(M2MSecurity::M2MServer);
        if (server != NULL)  {
        	const String url = this->m_options->getConnectorURL();
            server->set_resource_value(M2MSecurity::M2MServerUri, url);
            server->set_resource_value(M2MSecurity::SecurityMode, M2MSecurity::Certificate);
            server->set_resource_value(M2MSecurity::ServerPublicKey,this->m_options->getServerCertificate(),this->m_options->getServerCertificateSize());
            server->set_resource_value(M2MSecurity::PublicKey,this->m_options->getClientCertificate(),this->m_options->getClientCertificateSize());
            server->set_resource_value(M2MSecurity::Secretkey,this->m_options->getClientKey(),this->m_options->getClientKeySize());
        }
        return server;
}

// mbed-client: Callback from mbed client stack if any error is encountered
void Endpoint::error(M2MInterface::Error error) {
        switch(error){
            case M2MInterface::AlreadyExists:
                DEBUG_OUT("[ERROR:] M2MInterface::AlreadyExists");
                break;
            case M2MInterface::BootstrapFailed:
               DEBUG_OUT("[ERROR:] M2MInterface::BootstrapFailed");
                break;
            case M2MInterface::InvalidParameters:
               DEBUG_OUT("[ERROR:] M2MInterface::InvalidParameters");
                break;
            case M2MInterface::NotRegistered:
                DEBUG_OUT("[ERROR:] M2MInterface::NotRegistered");
                break;
            case M2MInterface::Timeout:
                DEBUG_OUT("[ERROR:] M2MInterface::Timeout");
                break;
            case M2MInterface::NetworkError:
                DEBUG_OUT("[ERROR:] M2MInterface::NetworkError");
                break;
            case M2MInterface::ResponseParseFailed:
                DEBUG_OUT("[ERROR:] M2MInterface::ResponseParseFailed");
                break;
            case M2MInterface::UnknownError:
               DEBUG_OUT("[ERROR:] M2MInterface::UnknownError");
                break;
            case M2MInterface::MemoryFail:
                DEBUG_OUT("[ERROR:] M2MInterface::MemoryFail");
                break;
            case M2MInterface::NotAllowed:
               DEBUG_OUT("[ERROR:] M2MInterface::NotAllowed");
                break;
            default:
                break;
        }
}

// complete registration of the endpoint
void Endpoint::complete_endpoint_registration(M2MSecurity *register_object, M2MObjectList object_list) {
        if (this->m_interface != NULL)  {
            this->m_interface->register_object(register_object, object_list);
        }
}

// deregister endpoint and stop scheduling
void Endpoint::closedown_endpoint(void) {
	if (this->m_interface != NULL) {
		// de-register endpoint
		this->logger()->log("De-registering endpoint...");
		this->m_interface->unregister_object(NULL);		
	}
}

// re-register the endpoint
void Endpoint::re_register_endpoint() {
	 if (this->m_interface != NULL)  {
		  this->m_interface->update_registration(this->m_server_instance, this->m_options->getLifetime());
	  }  
}

// bootstrap done
void Endpoint::bootstrap_done(M2MSecurity * /*server_object */) {
	this->logger()->log("Bootstrapped");
}

// object registered
void Endpoint::object_registered(M2MSecurity */*security_object */, const M2MServer &/*server_object*/)  {
	this->logger()->log("Registered");
}

// object unregistered
void Endpoint::object_unregistered(M2MSecurity */*server_object*/)  {
	// ready to exit
	this->logger()->log("Endpoint has been deregistered.. Ready to exit...");

	// stop
	minar::Scheduler::stop();
}

// registration updated
void Endpoint::registration_updated(M2MSecurity */*security_object*/, const M2MServer &/*server_object*/)  {
	this->logger()->log("Registration Updated");
}

// resource value updated
void Endpoint::value_updated(M2MBase *base, M2MBase::BaseType type) {
	this->logger()->log("Value Updated");
	DynamicResource *target_res = this->lookupDynamicResource(base);
	target_res->process(base->operation(),type);
}

// lookup which DynamicResource cooresponds to a given M2MBase instance...
DynamicResource *Endpoint::lookupDynamicResource(M2MBase *base) {
	DynamicResource *res = NULL;
	bool found = false;
	const DynamicResourcesList *dynamic_resources = this->m_options->getDynamicResourceList();
    for(int i=0; i<(int)dynamic_resources->size() && found == false; ++i) {
    	M2MBase *t = dynamic_resources->at(i)->getResource();
        if (t == base) {
        	res = dynamic_resources->at(i);
        	found = true;
        }
    }	
	
	return res;
}

// register the endpoint
void Endpoint::register_endpoint()
{	
   // initialize as an mbed-client
   this->create_interface();
   
   // Create our server instance
   this->m_server_instance = this->create_server_instance();
   
   // Loop through Static Resources and bind each of them...
    this->logger()->log("Endpoint::initialize(): adding device resources...");
    const DeviceResourcesList *device_resources = this->m_options->getDeviceResourceList();
    for(int i=0; i<(int)device_resources->size(); ++i) {
        this->logger()->log("Endpoint::initialize(): binding device resource: [%s]...",device_resources->at(i)->getFullName().c_str());
        this->m_object_list.push_back(device_resources->at(i)->bind(this));
    }
        
    // Loop through Static Resources and bind each of them...
    this->logger()->log("Endpoint::initialize(): adding static resources...");
    const StaticResourcesList *static_resources = this->m_options->getStaticResourceList();
    for(int i=0; i<(int)static_resources->size(); ++i) {
        this->logger()->log("Endpoint::initialize(): binding static resource: [%s]...",static_resources->at(i)->getFullName().c_str());
        this->m_object_list.push_back(static_resources->at(i)->bind(this));
    }

    // Loop through Dynamic Resources and bind each of them...
    this->logger()->log("Endpoint::initialize(): adding dynamic resources...");
    const DynamicResourcesList *dynamic_resources = this->m_options->getDynamicResourceList();
    for(int i=0; i<(int)dynamic_resources->size(); ++i) {
        this->logger()->log("Endpoint::initialize(): binding dynamic resource: [%s]...",dynamic_resources->at(i)->getFullName().c_str());
        this->m_object_list.push_back(dynamic_resources->at(i)->bind(this));
    }
}

// our logger
Logger *Endpoint::logger()
{
    return this->m_logger;
}

} // namespace Connector
