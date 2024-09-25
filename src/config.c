#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/config.h"

Theme *RetrieveTheme(const str *name) {
    Theme *t = (Theme *)malloc(sizeof(Theme));

    t->Name = (str *)name;

    str *theme_full_path = string(THEMES_PATH);
    theme_full_path->AppendString(theme_full_path, name->data);
    theme_full_path->AppendString(theme_full_path, "/layout.ex");

    cFile *layout = Openfile(theme_full_path->data);
    layout->Read(layout);
    
    str *help_path = string(THEMES_PATH);
    help_path->AppendString(help_path, name->data);
    help_path->AppendString(help_path, "/commands/help.ex");
    cFile *help = Openfile(help_path->data);
    help->Read(help);

    t->Help = string(help->data);
    t->Layout = string(layout->data);

    free(theme_full_path);
    free(layout);
    free(help_path);
    free(help);
    return t;
}