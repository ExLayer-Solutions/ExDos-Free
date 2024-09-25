#pragma once

#include <clib/OS/cFile.h>
#include <clib/String/str.h>
#include <clib/Map/map.h>

typedef struct Plan {
    int     level;
    char    *name;
    int     max_time;
    int     vip;
    int     rank;
} Plan;

#define PLAN_DB_PATH "assets/db/plans.json"

// == [ PLAN METHODS ] ==

Plan    **get_all_plans();