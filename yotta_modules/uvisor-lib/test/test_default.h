/*
 * Copyright (c) 2013-2015, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __UVISOR_LIB_TEST_DEFAULT_H__
#define __UVISOR_LIB_TEST_DEFAULT_H__

/* common header files */
#include "uvisor-lib/uvisor-lib.h"
#include "mbed-drivers/test_env.h"
#include "utest/utest.h"
#include "unity/unity.h"

/* API version specification for utest */
using namespace utest::v1;

/* secure boxes configurations */
#include "test_box0.h"

/* default test timeout */
#define UVISOR_LIB_TEST_DEFAULT_TIMEOUT 5

/* default test configuration
 *  defines the default greentea setup handler
 *  defines the test specification based on the provided test cases */
#define UVISOR_LIB_TEST_SETUP(name, cases) \
    status_t greentea_setup(const size_t number_of_cases) { \
        /* greentea configuration */ \
        MBED_HOSTTEST_TIMEOUT(UVISOR_LIB_TEST_DEFAULT_TIMEOUT); \
        MBED_HOSTTEST_SELECT(name); \
        MBED_HOSTTEST_DESCRIPTION(name); \
        MBED_HOSTTEST_START(name); \
        \
        /* call the default reporting function */ \
        return verbose_test_setup_handler(number_of_cases); \
    } \
    \
    /* declare test specification
     *  use a greentea-compatible setup handler
     *  use greentea "continue" method for failing test cases */ \
    Specification specification(greentea_setup, cases, greentea_continue_handlers);

#define UVISOR_LIB_TEST_RUN() \
    /* run the test specification */ \
    Harness::run(specification);

#endif/*__UVISOR_LIB_TEST_DEFAULT_H__*/
