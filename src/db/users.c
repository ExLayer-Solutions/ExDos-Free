#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../../headers/db/users.h"

User *new_user(int i, Arr *args) {
    User *u = (User *)malloc(sizeof(User));

    u->idx              = i;
    u->USERNAME         = string(args->arr[0]);
    u->IP_ADDR          = string(args->arr[1]);
    u->PASSWD           = string(args->arr[2]);
    u->PLAN             = atoi(args->arr[3]);
    u->MAX_TIME         = atoi(args->arr[4]);
    u->MAX_CONCURRENTS  = atoi(args->arr[5]);
    u->VIP              = atoi(args->arr[6]);
    u->EXPIRY           = string(args->arr[7]);
    u->RANK             = atoi(args->arr[8]);

    return u;
}

// Create a new user && add to db
User *create_new_user(str *username, str *ip_addr, str *password) {
    User *u = (User *)malloc(sizeof(User));

    u->USERNAME = username;
    u->IP_ADDR = ip_addr;
    u->PASSWD = password;
    u->PLAN = 0;
    u->MAX_TIME = 0;
    u->MAX_CONCURRENTS = 0;
    u->VIP = 0;
    u->EXPIRY = string("N/A");
    u->RANK = 0;

    return u;
}

int ChangeUsername(User *u, str *_new) {
    if(!u)
        return 0;

    u->USERNAME = _new;
    return 1;
}

int ChangeIPAddress(User *u, str *_new) {
    if(!u)
        return 0;

    u->IP_ADDR = _new;
    return 1;
}

int ChangePassword(User *u, str *_new) {
    if(!u)
        return 0;

    u->PASSWD = _new;
    return 1;
}

int UpdatePlan(User *u, str *new_plan, str *new_time, str *new_con, str *vip, str *expiry) {
    if(!u)
        return 0;

    u->PLAN                 = atoi(new_plan->data);
    u->MAX_TIME             = atoi(new_time->data);
    u->MAX_CONCURRENTS      = atoi(new_con->data);
    u->VIP                  = atoi(vip->data);
    u->EXPIRY               = expiry;

    return 1;
}

int ChangeRank(User *u, Rank_T r) {
    if(!u)
        return 0;
        
    switch(r) {
        case NORMAL:    { u->RANK = 0; }
        case ADMIN:     { u->RANK = 1; }
        case RESELLER:  { u->RANK = 2; }
        case OWNER:     { u->RANK = 3; }
        default: 
            return 0;
    }

    return 1;
}

void SaveDB(User **users, int count) {
    str *new = string(NULL);
    for(int i = 0; i < count; i++)
    {
        if(!users[i])
            break;
            
        char *DB_LINE[] = {
            "('", users[i]->USERNAME->data, "','", 
            users[i]->IP_ADDR->data, "','", 
            users[i]->PASSWD->data, "','", 
            int2ch(users[i]->PLAN), "','",
            int2ch(users[i]->MAX_TIME), "','",
            int2ch(users[i]->MAX_CONCURRENTS), "','",
            int2ch(users[i]->VIP), "','",
            users[i]->EXPIRY->data, "','",
            int2ch(users[i]->RANK), "')\n"};

        for(int c = 0; c < 19; c++) {
            if(DB_LINE[c] == NULL)
                break;

            new->AppendString(new, DB_LINE[c]);
        }
    }

    if(!CreateFile(string("assets/db/users.db"), new))
        printf("[ X ] Failed to save...!\n");

    free(new);
}