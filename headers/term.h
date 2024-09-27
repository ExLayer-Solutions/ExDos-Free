#pragma once

#include "utils.h"
#include "db/users.h"
#include "db/plans.h"

#include <clib/Array/arr.h>
#include <clib/Map/map.h>
#include <clib/String/str.h>
#include <clib/Net/socket.h>

static const char *COLORS[][2] = {
    {"{C_RED}", "\x1b[31m"},
    {"{C_YELLOW}", "\x1b[93m"},
    {"{C_BLUE}", "\x1b[34m"},
    {"{C_PURPLE}", "\x1b[95m"},
    {"{C_GREEN}", "\x1b[32m"},
    {"{C_BLACK}", "\x1b[30m"},
    {"{C_GREY}", "\x1b[90m"},
    {"{C_CYAN}", "\x1b[36m"},
    {"{C_WHITE}", "\x1b[97m"},
    {"{C_DEFAULT}", "\x1b[39m"},
    {"{BG_BLACK}", "\x1b[40m"},
    {"{BG_RED}", "\x1b[41m"},
    {"{BG_GREEN}", "\x1b[42m"},
    {"{BG_YELLOW}", "\x1b[43m"},
    {"{BG_BLUE}", "\x1b[44m"},
    {"{BG_PURPLE}", "\x1b[45m"},
    {"{BG_CYAN}", "\x1b[46m"},
    {"{BG_LIGHTGREY}", "\x1b[47m"},
    {"{BG_DARKGREY}", "\x1b[100m"},
    {"{BG_LIGHTRED}", "\x1b[101m"},
    {"{BG_LIGHTGREEN}", "\x1b[102m"},
    {"{BG_LIGHTYELLOW}", "\x1b[103m"},
    {"{BG_DEFAULT}", "\x1b[49m"},
    NULL
};

static const char *ACCOUNT_INFO[][2] = {
    {"{\\W}", ""},
    {"{IP_ADDR}", ""},
    {"{PLAN}", ""},
    {"{MAX_TIME}", ""},
    {"{MAX_CONS}", ""},
    {"{VIP}", ""},
    {"{EXPIRY}", ""},
    {"{RANK}", ""},
    NULL
};

str     *change_vars(char *data, User *u);
void    clear_term(Socket *client);
void    ChangeTermTitle(Socket *client, str *t);
void    changeTerminalSize(Socket *client, str *row, str *column);
void    place_text(Socket *client, str *row, str *column, str *text);
void    list_text(Socket *client, str *row, str *column, str *text);
str     *create_string(char **arr);
