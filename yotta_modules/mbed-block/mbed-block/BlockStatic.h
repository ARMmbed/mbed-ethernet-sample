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

#ifndef __BLOCKSTATIC_H__
#define __BLOCKSTATIC_H__

#include "mbed-block/Block.h"

#include <cstring>

class BlockStatic : public Block
{
public:
    BlockStatic(uint8_t* _data = NULL, uint32_t _maxLength = 0)
        :   data(_data)
    {
        Block::length = _maxLength;
        Block::offset = 0;
        Block::maxLength = _maxLength;
    }

    /*
        Copy data into block object.
    */
    virtual void memcpy(std::size_t index, const void* source, std::size_t num)
    {
        // NULL pointer check
        if (data)
        {
            // protect against buffer overflow
            if (index + num > maxLength)
            {
                num = maxLength - index;
                length = maxLength;
            }

            // update current length
            if (index + num > length)
            {
                length = index + num;
            }

            // copy data to block
            std::memcpy(&(data[index]), source, num);
        }
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

    /*  Subscript operator.
    */
    virtual uint8_t& operator[](const std::size_t index)
    {
        if (data)
        {
            return data[index];
        }
        else
        {
            return shieldByte;
        }
    }

    virtual uint8_t& at(const std::size_t index)
    {
        if (data)
        {
            return data[index];
        }
        else
        {
            return shieldByte;
        }
    }

    /*
        Update block with new buffer.
    */
    void setBlock(BlockStatic* rhs)
    {
        data = rhs->getData();
        length = rhs->getLength();
        offset = rhs->getOffset();
        maxLength = rhs->getMaxLength();
    }

    void setData(uint8_t* _data, uint32_t _maxLength)
    {
        data = _data;
        length = _maxLength;
        offset = 0;
        maxLength = _maxLength;
    }

    /*
        Accessor.
    */
    uint8_t* getData()
    {
        return data;
    }

private:
    uint8_t* data;
    uint8_t shieldByte;
};

#endif // __BLOCKSTATIC_H__
