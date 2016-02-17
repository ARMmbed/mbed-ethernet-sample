/*
 * PackageLicenseDeclared: Apache-2.0
 * Copyright (c) 2015 ARM Limited
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

#include <stddef.h>
#include <stdint.h>
#include "mbed/test_env.h"
#include "core-util/SharedPointer.h"
#include "mbed-alloc/ualloc.h"
#include "mbed-block/BlockDynamic.h"

using namespace mbed::util;

const char* test_string = "Hello World";

SharedPointer<Block> global_block;

void test2Task()
{
    // count is 1: global pointer
    MBED_HOSTTEST_ASSERT(global_block.use_count() == 1);

    MBED_HOSTTEST_RESULT(true);
}


void test1Task(SharedPointer<Block> block)
{
    // compare memory content
    for (std::size_t idx = 0; idx < block->getLength(); idx++)
    {
        MBED_HOSTTEST_ASSERT(block->at(idx) == test_string[idx]);
    }

    // count is 3: global pointer, minar pointer, and local pointer
    MBED_HOSTTEST_ASSERT(block.use_count() == 3);

    minar::Scheduler::postCallback(test2Task);
}

void app_start(int, char*[]) {
    MBED_HOSTTEST_TIMEOUT(2);
    MBED_HOSTTEST_SELECT(default);
    MBED_HOSTTEST_DESCRIPTION(Block test);
    MBED_HOSTTEST_START("Block_TEST");

    // global shared pointer is NULL
    MBED_HOSTTEST_ASSERT(global_block.use_count() == 0);

    // create reference counted memory block and assign to global shared pointer
    global_block = SharedPointer<Block>(new BlockDynamic(sizeof(test_string)));
    global_block->memcpy(0, test_string, sizeof(test_string));

    // count is 1
    MBED_HOSTTEST_ASSERT(global_block.use_count() == 1);

    // use block as parameter in scheduler
    FunctionPointer1<void, SharedPointer<Block> > fp(test1Task);
    minar::Scheduler::postCallback(fp.bind(global_block));

    // count is 2: global pointer and minar pointer
    MBED_HOSTTEST_ASSERT(global_block.use_count() == 2);
}
