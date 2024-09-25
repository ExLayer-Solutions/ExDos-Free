#pragma once

#include <clib/String/str.h>
#include <clib/Array/arr.h>
#include <clib/Net/socket.h>
#include <clib/Array/multi_type.h>

typedef enum Event_T {
    NEW_CONNECTION_EVENT    = 0x700001,
    DISCONNECT_EVENT        = 0x700002,
    FAILED_LOGIN_EVENT      = 0x700003,
    SUCCESSFUL_LOGIN_EVENT  = 0x700004,
    INVALID_CMD_EVENT       = 0x700005,
    VALID_CMD_EVENT         = 0x700006
} Event_T;

typedef struct Command {
    str         *name;          // help
    Arr         *alt_names;     // {"help", "?", NULL}
    int         arg_count;
    int         rank;
    void *      (*fn)();
} Command;

typedef struct Event {
    Event_T     mode;
    void *      (*fn)();
} Event;

typedef struct Cogs {
    Event       **events;
    long        event_idx;

    Command     **cmds;
    long        idx;

    long        (*FindEvent)    (struct Cogs *c, Event_T mode);
    Event *     (*GetEvent)     (struct Cogs *c, long idx);
    int         (*AddEvent)     (struct Cogs *c, Event_T m, void *h);
    int         (*RemoveEvent)  (struct Cogs *c, Event_T mode);

    long        (*FindCmd)     (struct Cogs *c, str *q);
    Command *   (*GetCmd)      (struct Cogs *c, long idx);
    int         (*AddCmd)      (struct Cogs *c, const Arr *alias, const int argc, const int r, void *fn);
    int         (*RemoveCmd)   (struct Cogs *c, str *q);
} Cogs;

//
//          Create a new instance of Cogs
//
Cogs        *InitCogs();

//
//          Check if an event is valid
//
long        _FindEvent(Cogs *c, Event_T mode);

//
//          Get an event if its valid
//
Event       *_GetEvent(Cogs *c, long idx);

//
//          Add an event handler
//
int         _AddEvent(Cogs *c, Event_T m, void *handler);

//
//          Remove an event handler (NEEDS WORK)
//
int         _RemoveEvent(Cogs *c, Event_T mode);

//
//          Find a command (Checks if a command is valid)
//
long        FindCmd(Cogs *c, str *q);

//
//          Get the Command instanse
//
Command     *GetCmd(Cogs *c, long idx);

//
//          Add a cog/command to cogs
//
int         AddCog(Cogs *c, const Arr *alias, const int argc, const int r, void *fn);

//
//          Remove a cog/command from the cogs list
//
int         RmCmd(Cogs *c, str *q);