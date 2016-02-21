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

#define TEST_VAL1 0x1U
#define TEST_VAL2 0x2U
#define TEST_VAL3 0x3U
#define TEST_VAL4 0x4U

void svc_custom_table_args0(void)
{
    asm volatile(
        "mov r0, 0\n"
        "mov r1, 0\n"
        "mov r2, 0\n"
        "mov r3, 0\n"
    );
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_CUSTOM, "");
    TEST_ASSERT_EQUAL(0, retval);
}

void svc_custom_table_args1(void)
{
    asm volatile(
        "mov r1, 0\n"
        "mov r2, 0\n"
        "mov r3, 0\n"
    );
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_CUSTOM, "", TEST_VAL1);
    TEST_ASSERT_EQUAL(TEST_VAL1, retval);
}

void svc_custom_table_args2(void)
{
    asm volatile(
        "mov r2, 0\n"
        "mov r3, 0\n"
    );
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_CUSTOM, "", TEST_VAL1, TEST_VAL2);
    TEST_ASSERT_EQUAL(TEST_VAL1 + TEST_VAL2, retval);
}

void svc_custom_table_args3(void)
{
    asm volatile(
        "mov r3, 0\n"
    );
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_CUSTOM, "", TEST_VAL1, TEST_VAL2, TEST_VAL3);
    TEST_ASSERT_EQUAL(TEST_VAL1 + TEST_VAL2 + TEST_VAL3, retval);
}

void svc_custom_table_args4(void)
{
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_CUSTOM, "", TEST_VAL1, TEST_VAL2, TEST_VAL3, TEST_VAL4);
    TEST_ASSERT_EQUAL(TEST_VAL1 + TEST_VAL2 + TEST_VAL3 + TEST_VAL4, retval);
}

void svc_fixed_table_args0(void)
{
    asm volatile(
        "mov r0, 0\n"
        "mov r1, 0\n"
        "mov r2, 0\n"
        "mov r3, 0\n"
    );
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_FIXED(0), "");
    TEST_ASSERT_EQUAL(0, retval);
}

void svc_fixed_table_args1(void)
{
    asm volatile(
        "mov r1, 0\n"
        "mov r2, 0\n"
        "mov r3, 0\n"
    );
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_FIXED(1), "", TEST_VAL1);
    TEST_ASSERT_EQUAL(TEST_VAL1, retval);
}

void svc_fixed_table_args2(void)
{
    asm volatile(
        "mov r2, 0\n"
        "mov r3, 0\n"
    );
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_FIXED(2), "", TEST_VAL1, TEST_VAL2);
    TEST_ASSERT_EQUAL(TEST_VAL1 + TEST_VAL2, retval);
}

void svc_fixed_table_args3(void)
{
    asm volatile(
        "mov r3, 0\n"
    );
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_FIXED(3), "", TEST_VAL1, TEST_VAL2, TEST_VAL3);
    TEST_ASSERT_EQUAL(TEST_VAL1 + TEST_VAL2 + TEST_VAL3, retval);
}

void svc_fixed_table_args4(void)
{
    uint32_t retval = UVISOR_SVC(UVISOR_SVC_ID_TEST_FIXED(4), "", TEST_VAL1, TEST_VAL2, TEST_VAL3, TEST_VAL4);
    TEST_ASSERT_EQUAL(TEST_VAL1 + TEST_VAL2 + TEST_VAL3 + TEST_VAL4, retval);
}
