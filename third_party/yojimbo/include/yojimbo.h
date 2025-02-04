/*
    Yojimbo Client/Server Network Library.

    Copyright © 2016 - 2024, Mas Bandwidth LLC.

    Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

        1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

        2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
           in the documentation and/or other materials provided with the distribution.

        3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived
           from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
    USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef YOJIMBO_H
#define YOJIMBO_H

#include "../serialize/serialize.h"
#include "yojimbo_config.h"
#include "yojimbo_constants.h"
#include "yojimbo_bit_array.h"
#include "yojimbo_utils.h"
#include "yojimbo_queue.h"
#include "yojimbo_sequence_buffer.h"
#include "yojimbo_address.h"
#include "yojimbo_serialize.h"
#include "yojimbo_message.h"
#include "yojimbo_channel.h"
#include "yojimbo_reliable_ordered_channel.h"
#include "yojimbo_unreliable_unordered_channel.h"
#include "yojimbo_connection.h"
#include "yojimbo_network_simulator.h"
#include "yojimbo_adapter.h"
#include "yojimbo_network_info.h"
#include "yojimbo_server_interface.h"
#include "yojimbo_base_server.h"
#include "yojimbo_server.h"
#include "yojimbo_client_interface.h"
#include "yojimbo_base_client.h"
#include "yojimbo_client.h"

/** @file */

/// The library namespace.

namespace yojimbo
{
    using namespace serialize;
}

/**
    Initialize the yojimbo library.
    Call this before calling any yojimbo library functions.
    @returns True if the library was successfully initialized, false otherwise.
 */

bool InitializeYojimbo();

/**
    Enable packet tagging.
    Enable before you create any client or servers, and DSCP packet tagging will be applied to all packets sent.
    Packet tagging can significantly reduce jitter on modern Wi-Fi 6+ routers, by marking your game traffic to be sent in the real-time queue.
 */

void EnablePacketTagging();

/**
    Shutdown the yojimbo library.
    Call this after you finish using the library and it will run some checks for you (for example, checking for memory leaks in debug build).
 */

void ShutdownYojimbo();

#endif // #ifndef YOJIMBO_H
