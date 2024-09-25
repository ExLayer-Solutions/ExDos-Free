#pragma once

#include <clib/String/str.h>
#include <clib/OS/cFile.h>
#include "../headers/term.h"

typedef struct Theme {
    str *Name;
    str *Layout;
    str *Help;
} Theme;

#define THEMES_PATH "assets/themes/"

Theme *RetrieveTheme(const str *name);
