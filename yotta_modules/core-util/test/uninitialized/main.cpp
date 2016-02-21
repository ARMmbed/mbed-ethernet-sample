/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2016 ARM Limited
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
#include <stdint.h>
#include "cmsis-core/core_generic.h"
#include "mbed-drivers/test_env.h"
#include "core-util/uninitialized.h"

#define TEST_C_INIT      0xDEADBEEFUL
#define TEST_MANUAL_INIT 0xACCE55EDUL

__uninitialized uint32_t g_state = TEST_C_INIT;

void app_start(int, char*[])
{
    if (g_state == TEST_C_INIT) {
        /* First run: Check that the g_state does *not* hold the initialized
         * data. */
        MBED_HOSTTEST_TIMEOUT(5);
        MBED_HOSTTEST_SELECT(default);
        MBED_HOSTTEST_DESCRIPTION(uninitialized section);
        MBED_HOSTTEST_START("UNINITIALIZED_SECTION_TEST");
        MBED_HOSTTEST_RESULT(false);
    } else if (g_state != TEST_MANUAL_INIT) {
        MBED_HOSTTEST_TIMEOUT(5);
        MBED_HOSTTEST_SELECT(default);
        MBED_HOSTTEST_DESCRIPTION(uninitialized section);
        MBED_HOSTTEST_START("UNINITIALIZED_SECTION_TEST");

        /* First or subsequent runs: If this is the first run, initialize the
         * state and reset. If this code is run again after the first run it
         * means the state is not kept across reboots. Eventually, the test will
         * timeout, failing. */
        g_state = TEST_MANUAL_INIT;
        NVIC_SystemReset();
    } else {
        /* Second run: The data was correctly initialized and kept across a
         * system reset. */
        g_state = 0;
        MBED_HOSTTEST_RESULT(true);
    }

    return;
}
