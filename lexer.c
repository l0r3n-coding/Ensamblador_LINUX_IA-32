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
// esto sigue asi con todas las combinaciones de las otras cosas aaaa mi cerebro = YA

// Comprueba si la palabra es una instruccion
static int es_mnemonico(char *palabra) {

    // Movimiento de datos
    if (strcmp(palabra, "mov") == 0) return 1;
    if (strcmp(palabra, "movzx") == 0) return 1;
    if (strcmp(palabra, "movsx") == 0) return 1;
    if (strcmp(palabra, "lea") == 0) return 1;
    if (strcmp(palabra, "xchg") == 0) return 1;

    // Operaciones aritmeticas
    if (strcmp(palabra, "add") == 0) return 1;
    if (strcmp(palabra, "sub") == 0) return 1;
    if (strcmp(palabra, "inc") == 0) return 1;
    if (strcmp(palabra, "dec") == 0) return 1;
    if (strcmp(palabra, "mul") == 0) return 1;
    if (strcmp(palabra, "imul") == 0) return 1;
    if (strcmp(palabra, "div") == 0) return 1;
    if (strcmp(palabra, "idiv") == 0) return 1;

    // Operaciones logicas
    if (strcmp(palabra, "and") == 0) return 1;
    if (strcmp(palabra, "or") == 0) return 1;
    if (strcmp(palabra, "xor") == 0) return 1;
    if (strcmp(palabra, "not") == 0) return 1;
    if (strcmp(palabra, "neg") == 0) return 1;
    if (strcmp(palabra, "test") == 0) return 1;
    if (strcmp(palabra, "cmp") == 0) return 1;

    // Operaciones de pila
    if (strcmp(palabra, "push") == 0) return 1;
    if (strcmp(palabra, "pop") == 0) return 1;

    // Saltos y control de flujo
    if (strcmp(palabra, "jmp") == 0) return 1;
    if (strcmp(palabra, "je") == 0) return 1;
    if (strcmp(palabra, "jne") == 0) return 1;
    if (strcmp(palabra, "jz") == 0) return 1;
    if (strcmp(palabra, "jnz") == 0) return 1;
    if (strcmp(palabra, "jg") == 0) return 1;
    if (strcmp(palabra, "jl") == 0) return 1;
    if (strcmp(palabra, "jge") == 0) return 1;
    if (strcmp(palabra, "jle") == 0) return 1;
    if (strcmp(palabra, "call") == 0) return 1;
    if (strcmp(palabra, "ret") == 0) return 1;

    // Desplazamientos
    if (strcmp(palabra, "shl") == 0) return 1;
    if (strcmp(palabra, "shr") == 0) return 1;
    if (strcmp(palabra, "sar") == 0) return 1;

    return 0;
}



