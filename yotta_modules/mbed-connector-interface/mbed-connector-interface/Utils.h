/**
 * @file    Utils.h
 * @brief   mbed CoAP Endpoint misc utils collection (header)
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
#ifndef __UTILS_H__
#define __UTILS_H__

extern void *utils_init_endpoint(bool canActAsRouterNode);
extern void utils_configure_endpoint(void *p);
extern void utils_register_endpoint(void *p);

#endif // __UTILS_H__
