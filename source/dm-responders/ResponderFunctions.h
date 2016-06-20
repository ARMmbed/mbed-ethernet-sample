/**
 * @file    ResponderFunctions.h
 * @brief   mbed Endpoint Data Management Action Responders
 * @author  Doug Anson
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
 
 #ifndef __DM_RESPONDER_FUNCTIONS_H__
 #define __DM_RESPONDER_FUNCTIONS_H__
 
 // Initialize Callback
 extern "C" void dm_initialize(const void *l);
 
 // Reboot Responder
 extern "C" bool dm_reboot_responder(const void *e,const void *l,const void */* not used */);
 
 // Reset Responder
 extern "C" bool dm_reset_responder(const void *e,const void *l,const void */* not used */);
 
 // FOTA Manifest Set
 extern "C" bool dm_set_manifest(const void *e,const void *l,const void *manifest,uint32_t manifest_length);
 
 // FOTA Image Set
 extern "C" bool dm_set_fota_image(const void *e,const void *l,const void *image,uint32_t image_length);
  
 // FOTA Invocation Handler
 extern "C" bool dm_invoke_fota(const void *e,const void *l,const void *data);
 
 #endif // __DM_RESPONDER_FUNCTIONS_H__