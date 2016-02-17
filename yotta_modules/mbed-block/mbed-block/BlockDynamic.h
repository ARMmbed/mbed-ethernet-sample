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

#ifndef __BLOCKDYNAMIC_H__
#define __BLOCKDYNAMIC_H__

#include "mbed-block/BlockStatic.h"

#include <cstring>
#include <cstdlib>

class BlockDynamic : public BlockStatic
{
public:
    /* Allocate memory */
    BlockDynamic(uint32_t size)
        :   BlockStatic()
    {
        uint8_t* buffer = (uint8_t*) malloc(size);

        BlockStatic::setData(buffer, size);
    }

    /* Take owenership of memory allocated with malloc */
    BlockDynamic(uint8_t* buffer, uint32_t size)
        :   BlockStatic()
    {
        if (buffer)
        {
            BlockStatic::setData(buffer, size);
        }
    }

    ~BlockDynamic()
    {
        free(BlockStatic::getData());
    }

    void setBlock(BlockStatic* rhs)
    {
        (void) rhs;
    }

    void setData(uint8_t* buffer, uint32_t size)
    {
        free(BlockStatic::getData());

        BlockStatic::setData(buffer, size);
    }
};

#endif // __BLOCKDYNAMIC_H__
