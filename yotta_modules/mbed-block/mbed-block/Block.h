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

#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <stdint.h>
#include <cstddef>


class Block
{
public:

    virtual ~Block(void) {}
    /*
        Copy data into block object.
    */
    virtual void memcpy(std::size_t index, const void* source, std::size_t num) = 0;

    /*
        Copy data out from block object.
    */
    virtual void memcpy(void* destination, std::size_t index, std::size_t num) = 0;

    /*  Index access.
    */
    virtual uint8_t& operator[](const std::size_t index) = 0;
    virtual uint8_t& at(const std::size_t index) = 0;

    /*
        Accessors and mutators.
    */
    virtual uint32_t getMaxLength(void) const
    {
        return maxLength;
    }

    virtual uint32_t getLength(void) const
    {
        return length;
    }

    virtual uint32_t getOffset(void) const
    {
        return offset;
    }

    virtual void setLength(uint32_t _length)
    {
        length = _length;
    }

    virtual void setOffset(uint32_t _offset)
    {
        offset = _offset;
    }

protected:
    virtual void setMaxLength(uint32_t _maxLength)
    {
        maxLength = _maxLength;
    }

    uint32_t length;
    uint32_t offset;
    uint32_t maxLength;
};

#endif // __BLOCK_H__
