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

#ifndef __BLOCKSTATICREADONLY_H__
#define __BLOCKSTATICREADONLY_H__

#include "mbed-block/Block.h"

#include <cstring>

class BlockStaticReadOnly : public BlockStatic
{
public:
    BlockStaticReadOnly(const uint8_t* _data = NULL, uint32_t _length = 0)
        :   data(_data),
            shieldByte(0)
    {
        Block::length = _length;
        Block::offset = 0;
        Block::maxLength = _length;
    }

    /*
        Overwrite copy data into block object.
    */
    virtual void memcpy(std::size_t, const void*, std::size_t)
    {
        // do nothing
    }

    /*
        Copy data out from block object.
    */
    virtual void memcpy(void* destination, std::size_t index, std::size_t num)
    {
        // NULL pointer check
        if (data)
        {
            // protect against buffer underrun
            if (index + num > length)
            {
                num = length - index;
            }

            // copy data out from block
            std::memcpy(destination, &(data[index]), num);
        }
    }

    /*  Make subscript operator read-only.
    */
    virtual uint8_t& operator[](const std::size_t index)
    {
        if (data)
        {
            shieldByte = data[index];
        }

        return shieldByte;
    }

    virtual uint8_t& at(const std::size_t index)
    {
        if (data)
        {
            shieldByte = data[index];
        }

        return shieldByte;
    }

    /*
        Accessor.
    */
    const uint8_t* getData()
    {
        return data;
    }

private:
    const uint8_t* data;
    uint8_t shieldByte;
};

#endif // __BLOCKSTATICREADONLY_H__
