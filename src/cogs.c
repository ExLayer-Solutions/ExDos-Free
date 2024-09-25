#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/cogs.h"

Cogs *InitCogs() {
    Cogs *c = (Cogs *)malloc(sizeof(Cogs));

    *c = (Cogs){
        .events         = (Event **)malloc(sizeof(Event *) * 1),
        .event_idx      = 0,
        .cmds           = (Command **)malloc(sizeof(Command *) * 1),
        .idx            = 0,

        .FindEvent      = _FindEvent,
        .GetEvent       = _GetEvent,
        .AddEvent       = _AddEvent,
        .RemoveEvent    = _RemoveEvent,

        .FindCmd        = FindCmd,
        .GetCmd         = GetCmd,
        .AddCmd         = AddCog,
        .RemoveCmd      = RmCmd
    };

    return c;
}

long _FindEvent(Cogs *c, Event_T mode) {
    for(int i = 0; i < c->event_idx; i++)
        if(((Event *)c->events[i])->mode == mode)
            return i;
            
    return -1;
}


Event *_GetEvent(Cogs *c, long idx) {
    if(idx >= c->event_idx)
        return NULL;

    return c->events[idx];
}

int _AddEvent(Cogs *c, Event_T m, void *h) {
    if(!NEW_CONNECTION_EVENT && !DISCONNECT_EVENT && !FAILED_LOGIN_EVENT && !SUCCESSFUL_LOGIN_EVENT && !INVALID_CMD_EVENT && !VALID_CMD_EVENT)
        return 0;

    Event *e = (Event *)malloc(sizeof(Event));
    *e = (Event){
        .mode = m,
        .fn = h
    };

    c->events[c->event_idx] = (Event *)malloc(sizeof(Event));
    c->events[c->event_idx] = e;
    c->event_idx++;
    c->events = (Event **)realloc(c->events, sizeof(void *) * (c->event_idx + 1));

    return 1;
}


int _RemoveEvent(Cogs *c, Event_T mode) {

}

long FindCmd(Cogs *c, str *q) {
    for(int i = 0; i < c->idx; i++) {
        if(c->cmds[i] != NULL)
            if(!strcmp(c->cmds[i]->name->data, q->data))
                return i;
    }

    return -1;
}

Command *GetCmd(Cogs *c, long idx) {
    if(idx >= c->idx)
        return NULL;

    return c->cmds[idx];
}

int AddCog(Cogs *c, const Arr *alias, const int argc, const int r, void *fn) {
    Command *cmd = (Command *)malloc(sizeof(Command));
    *cmd = (Command){
        .name       = string(alias->arr[0]),
        .alt_names  = (Arr *)alias,
        .arg_count  = argc,
        .fn         = fn,
        .rank       = r
    };

    c->cmds[c->idx] = (Command *)malloc(sizeof(Command));
    c->cmds[c->idx] = cmd;
    c->idx++;
    c->cmds = (Command **)realloc(c->cmds, sizeof(Command *) * (c->idx + 1));
    return 1;
}

int RmCmd(Cogs *c, str *q) {
    long chk = FindCmd(c, q);
    if(chk == -1)
        return 0;

    Command **arr = (Command **)malloc(sizeof(Command *) * 1);
    long idx = 0;

    for(long i = 0; i < c->idx; i++) {
        if(c->cmds[i] == NULL)
            break;

        if(i == chk) {
            continue;
        }

        arr[idx] = (Command *)malloc(sizeof(Command));
        arr[idx] = c->cmds[i];
        idx++;
        arr = (Command **)realloc(arr, sizeof(Command *) * (idx + 1));
    }

    free(c->cmds);
    c->cmds = arr;
    c->idx = idx;

    return 1;
}