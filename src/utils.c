#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *int2ch(int i) {
    char *buf = (char *)malloc(10);
    sprintf(buf, "%d", i);
    return buf;
}