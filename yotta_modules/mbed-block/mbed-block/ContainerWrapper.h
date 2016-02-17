/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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

#ifndef __CONTAINER_WRAPPER_H__
#define __CONTAINER_WRAPPER_H__

#if defined(YOTTA_MINAR_VERSION_STRING)
// Building for mbed OS, use Array from core-util
#include "core-util/Array.h"
#else
// Building for mbed Classic, use std::vector
#include <vector>
#endif

template <typename T>
class ContainerWrapper
{
public:
    ContainerWrapper()
        :   container()
    {
#if defined(YOTTA_MINAR_VERSION_STRING)
        UAllocTraits_t traits = {0};

        container.init(4, 4, traits);
#endif
    }

    std::size_t size() const
    {
#if defined(YOTTA_MINAR_VERSION_STRING)
        return container.get_num_elements();
#else
        return container.size();
#endif
    }

    void push_back(const T& element)
    {
        container.push_back(element);
    }

    const T& at(std::size_t index) const
    {
        return container.at(index);
    }
private:
#if defined(YOTTA_MINAR_VERSION_STRING)
    mbed::util::Array<T> container;
#else
    std::vector<T> container;
#endif
};

#endif // __CONTAINER_WRAPPER_H__
