#pragma once

#include "__init__.h"

#include <clib/String/str.h>
#include <clib/Array/arr.h>
#include <clib/Net/socket.h>

typedef struct Bot {
    str     *Nickname;
    str     *ARCH;
    str     *OS;
    str     *RAM;
    Socket  *socket;
} Bot;

void AuthenticateBot(void **args);
Bot *new_bot(Socket *sock);
str *generate_nickname(int length);
