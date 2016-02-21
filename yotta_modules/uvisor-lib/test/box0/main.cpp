/*
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
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
#include "test_default.h"
#include "test_cases.h"

/* setup uVisor for this test:
 *  uVisor enabled
 *  box 0 configured */
UVISOR_LIB_TEST_SETUP_BOX0();

/* only one dummy test case */
Case cases[] = {
    Case("Deprivileging to box 0", box0_dummy),
};

/* setup test */
UVISOR_LIB_TEST_SETUP("Deprivileging to box 0", cases);

void app_start(int, char *[])
{
    /* setup serial port */
    static Serial pc(USBTX, USBRX);
    pc.baud(115200);

    /* run the test specification */
    UVISOR_LIB_TEST_RUN();
}
