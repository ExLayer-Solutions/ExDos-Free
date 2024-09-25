#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/term.h"

Map *fill_account_arr_value(User *u) {
    if(!u)
        return NULL;

    Map *info = create_map();

    info->Utils(info, __ADD_KEY, ACCOUNT_INFO[0][0], u->USERNAME->data);
    info->Utils(info, __ADD_KEY, ACCOUNT_INFO[1][0], u->IP_ADDR->data);
    info->Utils(info, __ADD_KEY, ACCOUNT_INFO[2][0], int2ch(u->PLAN));
    info->Utils(info, __ADD_KEY, ACCOUNT_INFO[3][0], int2ch(u->MAX_TIME));
    info->Utils(info, __ADD_KEY, ACCOUNT_INFO[4][0], int2ch(u->MAX_CONCURRENTS));
    info->Utils(info, __ADD_KEY, ACCOUNT_INFO[5][0], int2ch(u->VIP));
    info->Utils(info, __ADD_KEY, ACCOUNT_INFO[6][0], u->EXPIRY->data);
    info->Utils(info, __ADD_KEY, ACCOUNT_INFO[7][0], int2ch((int)u->RANK));

    return info;
}

str *change_vars(char *data, User *u) {
    str *new = string(data);

    for(int i = 0; i < 23; i++) {
        while(strstr(new->data, COLORS[i][0]) != NULL) {
            new->ReplaceString(new, COLORS[i][0], COLORS[i][1]);
        }
    }

    if(!u)
        return new;

    Map *account_vars = fill_account_arr_value(u);
    for(int i = 0; i < account_vars->idx; i++)
        if(strstr(new->data, (char *)((Key *)((Map *)account_vars)->keys[i])->name))
            new->ReplaceString(new, (char *)((Key *)((Map *)account_vars)->keys[i])->name, (char *)((Key *)((Map *)account_vars)->keys[i])->value);

    free(account_vars);
    return new;
}

void ChangeTermTitle(Socket *client, str *t) {
    char *title_args[] = {"\033]0;", t->data, "\007", NULL};
    str *new = string(NULL);

    for(int i = 0; i < 3; i++) {
        new->AppendString(new, title_args[i]);
    }
    
    Write(client, new);
    free(new);
}

void changeTerminalSize(Socket *client, str *row, str *column) {
    char *args[] = {"\x1b[8;", row->data, ";", column->data, "t"};
    str *new = string(NULL);

    for(int i = 0; i < 5; i++)
        new->AppendString(new, args[i]);

    Write(client, new);
    free(new);
}

void place_text(Socket *client, str *row, str *column, str *text) {
    char *args[] = {"\x1b[", row->data, ";", column->data, "f", text->data};
    str *new = string(NULL);

    for(int i = 0; i < 6; i++)
        new->AppendString(new, args[i]);

    Write(client, new);
    free(new);
}


void list_text(Socket *client, str *row, str *column, str *text) {
    int row_counter =  atoi(row->data);
    str *new_copy = string(text->data);
    Arr *lines = Array(new_copy->Split(new_copy, "\n"));
    
    for(int i = 0; i < lines->idx; i++) {
        str *c = string(int2ch(row_counter));
        char *args[] = {"\x1b[", c->data, ";", column->data, "f", lines->arr[i]};
        str *new = string(NULL);

        for(int i = 0; i < 6; i++)
            new->AppendString(new, args[i]);

        place_text(client, c, column, new);
        row_counter++;

        free(new);
        free(c);
    }

    free(new_copy);
    free(lines);
}

str *create_string(char **arr) {
    str *new = string(NULL);
    int i = 0;
    while(arr[i] != NULL) {
        new->AppendString(new, arr[i]);
        i++;
    }

    return new;
}