#pragma once

#include "__init__.h"
#include "cogs.h"
#include "term.h"
#include "db/users.h"
#include "db/plans.h"
#include "bot_handler.h"

#include <clib/String/str.h>
#include <clib/Net/socket.h>

typedef struct Client {
    str         *Username;
    User        *account;
    Socket      *sock;
} Client;

//
//          Constant Title Updater For Client Socket [THREADED]
//
void        *title_updater(void **args);

//
//          Client Authentication
//
void        AuthenticateUser(void **args);

// 
//          Create New Client Instance
//
Client      *new_client(str *username, User *acc, Socket *sock);

//
//          Client Command-line Interface
//
void        UserCLI(void *c, Socket *client, Client *u);