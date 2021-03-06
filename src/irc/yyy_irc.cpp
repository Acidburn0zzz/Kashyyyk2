/* 
 *  Copyright (c) 2017 Martin McDonough.  All rights reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 
 * - Products derived from this software may not be called "Kashyyyk", nor may
 *     "YYY" appear in their name, without prior written permission of
 *     the copyright holders.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
/*---------------------------------------------------------------------------*/

#include "yyy_irc.hpp"

#include "yyy_irc_core.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*---------------------------------------------------------------------------*/

namespace YYY {

/*---------------------------------------------------------------------------*/

static inline char irc_to_lower(const char c){
    return (c >= 'A' && c <= 'Z') ? ((c - 'A') + 'a') :
        (c == '{') ? '[' : 
        (c == '|') ? '\\' : 
        (c == '}') ? ']' : c;
}

/*---------------------------------------------------------------------------*/

bool IRCProtocol::parseMessage(const char *src, size_t len, Message &out){
    return YYY_IRCParseMessage(src, len, &out) != 0;
}

/*---------------------------------------------------------------------------*/

const char *IRCProtocol::messageToString(const Message &msg, size_t &len){
    return YYY_IRCMessageToString(&msg, &len);
}

/*---------------------------------------------------------------------------*/

void IRCProtocol::freeMessageString(const char *str){
    free((void*)str);
}

/*---------------------------------------------------------------------------*/

bool IRCProtocol::compareIdentifiers(const char *str0, const char *str1, unsigned short len){
    for(unsigned short i = 0; i < len; i++){
        const char c0 = str0[i], c1 = str1[i];
        if(c0 != c1 && irc_to_lower(c0) != irc_to_lower(c1))
            return false;
    }
    return true;
}

/*---------------------------------------------------------------------------*/

size_t IRCProtocol::getNumHelloMessages() const {
    return 2;
}

/*---------------------------------------------------------------------------*/

#define YYY_IRC_USER_NAME "KashyyykUser"

void IRCProtocol::createHelloMessage(size_t i, const Configuration &conf, Message &out_msg) const{
    assert(i == 0 || i == 1);
    out_msg.m.any_from.from_len = 0;
#define SET_PARAM(TYPE, WHAT, FROM) out_msg.m.TYPE.WHAT = conf.m_ ## FROM; out_msg.m.TYPE.WHAT ## _len = strlen(conf.m_ ## FROM);
    switch(i){
        case 0:
            out_msg.type = eYYYChatIdentify;
            SET_PARAM(identify, user, user);
            SET_PARAM(identify, real, real);
            return;
        case 1:
            out_msg.type = eYYYChatNick;
            SET_PARAM(nick, nick, nick);
            return;
    }
}

} // namespace YYY
