#pragma once

#include <clib/String/str.h>
#include <clib/Array/arr.h>
#include <clib/Net/socket.h>

typedef struct Command {
    str     *name;          // help
    Arr     *alt_names;     // {"help", "?", NULL}
    int     arg_count;
    int     rank;
    void *  (*fn)();
} Command;

typedef struct Cogs {
    Command     **cmds;
    long        idx;

    long        (*Find)     (struct Cogs *c, str *q);
    Command *   (*Get)      (struct Cogs *c, long idx);
    int         (*Add)      (struct Cogs *c, const Arr *alias, const int argc, const int r, void *fn);
    int         (*Remove)   (struct Cogs *c, str *q);
} Cogs;


Cogs *InitCogs();
long FindCmd(Cogs *c, str *q);
Command *GetCmd(Cogs *c, long idx);
int AddCog(Cogs *c, const Arr *alias, const int argc, const int r, void *fn);
int RmCmd(Cogs *c, str *q);