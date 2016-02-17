/* mbed
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

#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdint.h>

typedef uint32_t socklen_t;

class Socket
{
public:
    /** Set blocking or non-blocking mode of the socket and a timeout on
        blocking socket operations
    \param blocking  true for blocking mode, false for non-blocking mode.
    \param timeout   timeout in ms [Default: (1500)ms].
    */
    void set_blocking(bool blocking, unsigned int timeout=1500);

    /** Set socket options
    \param level     stack level (see: lwip/sockets.h)
    \param optname   option ID
    \param optval    option value
    \param socklen_t length of the option value
    \return 0 on success, -1 on failure
    */
    int set_option(int level, int optname, const void *optval, socklen_t optlen);

    /** Get socket options
        \param level     stack level (see: lwip/sockets.h)
        \param optname   option ID
        \param optval    buffer pointer where to write the option value
        \param socklen_t length of the option value
        \return 0 on success, -1 on failure
        */
    int get_option(int level, int optname, void *optval, socklen_t *optlen);

    /** Close the socket
        \param shutdown   free the left-over data in message queues
     */
    int close(bool shutdown=true);
};

#endif // __SOCKET_H__
