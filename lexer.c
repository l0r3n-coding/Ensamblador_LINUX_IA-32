#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static int es_registro(char *lpalabra) {
    if (strcmp(palabra, "eax") == 0) return 1;
    if (strcmp(palabra, "ebx") == 0) return 1;
    if (strcmp(palabra, "ecx") == 0) return 1;
    if (strcmp(palabra, "edx") == 0) return 1;
    if (strcmp(palabra, "esi") == 0) return 1;
    if (strcmp(palabra, "edi") == 0) return 1;
    if (strcmp(palabra, "esp") == 0) return 1;
    if (strcmp(palabra, "ebp") == 0) return 1;
    return 0;
}

// esto sigue asi con todas las combinaciones de las otras cosas aaaa mi cerebro