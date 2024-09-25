#pragma once

#include "../utils.h"

#include <clib/OS/cFile.h>
#include <clib/String/str.h>
#include <clib/Map/map.h>

typedef enum Rank_T {
    NORMAL      = 0,
    ADMIN       = 1,
    RESELLER    = 2,
    OWNER       = 3
} Rank_T;

typedef struct User {
    long    idx;
    str     *USERNAME;
    str     *IP_ADDR;
    str     *PASSWD;
    int     PLAN;
    int     MAX_TIME;
    int     MAX_CONCURRENTS;
    int     VIP;
    str     *EXPIRY;
    Rank_T  RANK;
} User;

#define USER_DB_PATH "assets/db/users.db"

User    *new_user(int i, Arr *args);
User    *create_new_user(str *username, str *ip_addr, str *password);
int     ChangeIPAddress(User *u, str *_new);
int     ChangePassword(User *u, str *_new);
char    *int2ch(int i);
void    SaveDB(User **users, int count);