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

// Compara una palabra con una lista. Asi no hacemos un strcmp por cada instruccion xd
static int esta_en_lista(char *palabra, const char *lista[], int n){
    for (int i = 0; i < n; i++){ // Recorre la lista desde la posición 0 hasta la posición n-1
        if (strcmp(palabra, lista[i]) == 0) return 1;
    }
    return 0;
}

static int es_registro(char *palabra){
    const char *registros[] = {
        "eax", "ebx", "ecx", "edx", "esi", "edi", "esp", "ebp",
        "ax", "bx", "cx", "dx", "si", "di", "sp", "bp",
        "al", "ah", "bl", "bh", "cl", "ch", "dl", "dh",
        "cs", "ds", "es", "ss", "fs", "gs"
    };
    return esta_en_lista(palabra, registros, sizeof(registros)/sizeof(registros[0])); // Llama a la funcion esta_en_lista y le pasa:
// (palabra que queremos buscar, el arreglo donde vamos a buscar, cuántos elementos tiene el arreglo) 
// tamaño total del arreglo / tamaño elemento = # elementos 
}

static int es_mnemonico(char *palabra){
    // Instrucciones de la referencia IA-32 (y algunas formas usuales de NASM).
    const char *mnemonicos[] = {
        "mov", "lea", "xchg", "movzx", "movsx",
        "add", "sub", "inc", "dec", "adc", "sbb",
        "xor", "and", "or", "test", "not", "neg", "cmp",
        "mul", "imul", "div", "idiv",
        "push", "pop", "call", "ret", "jmp",
        "je", "jz", "jne", "jnz", "jg", "jge", "jl", "jle",
        "shl", "shr", "sar", "rol", "ror",
        "movsb", "movsw", "movsd", "lodsb", "lodsw", "lodsd",
        "stosb", "stosw", "stosd", "cmpsb", "cmpsw", "cmpsd",
        "rep", "repe", "repz", "repne", "repnz", "int", "nop"
    };
    return esta_en_lista(palabra, mnemonicos, sizeof(mnemonicos)/sizeof(mnemonicos[0]));
}

static int es_directiva(char *palabra){
    const char *directivas[] = {
        "section", "segment", "global", "extern", "bits", "default",
        "db", "dw", "dd", "dq", "resb", "resw", "resd", "resq",
        "equ", "times", "byte", "word", "dword", "qword",
        ".text", ".data", ".bss", ".rodata"
    };
    return esta_en_lista(palabra, directivas, sizeof(directivas)/sizeof(directivas[0]));
}

static int es_numero(char *palabra){ // determina si una palabra que leyo lexer es un num valido
    char *fin;
    if (palabra[0] == '\0') return 0; // ¿cadena vacia?
    // strtoll acepta decimales y 0xFF
    // strtoll significa string to long long y convierte una cadena a un entero de tipo long long
    // strtoll intenta convertir la cadena a un entero long long.
    // El 0 indica que detecte automáticamente la base numérica.
    // Reconoce, por ejemplo, 123 (decimal) y 0xFF (hexadecimal).
    strtoll(palabra, &fin, 0);
    if (*fin == '\0' && fin != palabra) return 1;

    int len = strlen(palabra); // longitud palabra 
    if (len < 2) return 0;
    char sufijo = tolower((unsigned char)palabra[len-1]);
    int base = (sufijo == 'h') ? 16 : (sufijo == 'b') ? 2 : (sufijo == 'o' || sufijo == 'q') ? 8 : 0;
    if (base == 0) return 0;
    for (int i = 0; i < len-1; i++){
        char c = tolower((unsigned char)palabra[i]);
        int valor = isdigit((unsigned char)c) ? c-'0' : (c >= 'a' && c <= 'f') ? c-'a'+10 : -1;
        // Convierte el carácter a su valor numérico.
        // isdigit comprueba si es un dígito entre 0 y 9
        // c-'0' convierte, por ejemplo, '5' al entero 5
        // Si es una letra entre 'a' y 'f', calcula su
        // valor hexadecimal entre 10 y 15
        // Si no es un carácter reconocido, asigna -1
        
        if (valor < 0 || valor >= base) return 0;
        // valor < 0 significa que el carácter no es válido
        // valor >= base significa que el dígito no existe en esa base
    }
    return 1;
}




