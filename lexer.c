#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

// En orden del documento de la profesora
static int es_registro(char *palabra) {            // Registros
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

static int es_mnemonico(char *palabra) {            // Comprueba si la palabra es una instruccion

    // En el oden del documento
    // 3. Movimiento de datos
    if (strcmp(palabra, "mov") == 0) return 1;
    if (strcmp(palabra, "lea") == 0) return 1;
    if (strcmp(palabra, "xchg") == 0) return 1;

    // 4. Operaciones aritmeticas
    if (strcmp(palabra, "add") == 0) return 1;
    if (strcmp(palabra, "sub") == 0) return 1;
    if (strcmp(palabra, "inc") == 0) return 1;
    if (strcmp(palabra, "dec") == 0) return 1;

    // 5. Operaciones logicas
    if (strcmp(palabra, "xor") == 0) return 1;
    if (strcmp(palabra, "and") == 0) return 1;
    if (strcmp(palabra, "or") == 0) return 1;
    if (strcmp(palabra, "test") == 0) return 1;

    // 6. Multiplicación y División
    if (strcmp(palabra, "mul") == 0) return 1;
    if (strcmp(palabra, "imul") == 0) return 1;
    if (strcmp(palabra, "div") == 0) return 1;
    if (strcmp(palabra, "idiv") == 0) return 1;

    // 7. Comparación
    if (strcmp(palabra, "cmp") == 0) return 1;

    // 8. Control de pila (Stack)
    if (strcmp(palabra, "push") == 0) return 1;
    if (strcmp(palabra, "pop") == 0) return 1;

    // 9. Saltos condicionales
    if (strcmp(palabra, "jnz") == 0) return 1;
        if (strcmp(palabra, "jne") == 0) return 1;
    if (strcmp(palabra, "jz") == 0) return 1;
        if (strcmp(palabra, "je") == 0) return 1;
    if (strcmp(palabra, "jle") == 0) return 1;
    if (strcmp(palabra, "jl") == 0) return 1;
    if (strcmp(palabra, "jge") == 0) return 1;
    if (strcmp(palabra, "jg") == 0) return 1;
    if (strcmp(palabra, "jcc") == 0) return 1;      // Jamás en mi vida había visto esta
    
    // 10. Control de Flujo
    if (strcmp(palabra, "call") == 0) return 1;
    if (strcmp(palabra, "ret") == 0) return 1;
    if (strcmp(palabra, "jmp") == 0) return 1;
    
    // 12. Extensión de datos
    if (strcmp(palabra, "movzx") == 0) return 1;    // 12.1 Extensión con ceros
    if (strcmp(palabra, "movsx") == 0) return 1;    // 12.2 Extensión con signo

    // 13. Desplazamientos y rotaciones
    //      13.1 Desplazamientos lógicos y aritméticos
    if (strcmp(palabra, "shl") == 0) return 1;
    if (strcmp(palabra, "shr") == 0) return 1;
    if (strcmp(palabra, "sar") == 0) return 1;
        //  13.2 Rotaciones
    if (strcmp(palabra, "rol") == 0) return 1;
    if (strcmp(palabra, "ror") == 0) return 1;

    // 14. Operaciones aritméticas complementarias
    if (strcmp(palabra, "neg") == 0) return 1;      // 14.1 Negación
    if (strcmp(palabra, "not") == 0) return 1;      // 14.2 Complemento a uno
    if (strcmp(palabra, "adc") == 0) return 1;      // 14.3 Suma con acarreo
    if (strcmp(palabra, "sbb") == 0) return 1;      // 14.4 Resta con préstamos

    // 15. Operaciones con cadenas
        // 15.1 Movimiento de cadenas
    if (strcmp(palabra, "movsb") == 0) return 1;
    if (strcmp(palabra, "movsw") == 0) return 1;
        if (strcmp(palabra, "movsd") == 0) return 1;
    // 15.2 Carga de cadenas
    if (strcmp(palabra, "lodsb") == 0) return 1;
    if (strcmp(palabra, "lodsw") == 0) return 1;
        if (strcmp(palabra, "lodsd") == 0) return 1;
    // Almacenamiento de cadenas (STOS)
    if (strcmp(palabra, "stosb") == 0) return 1;
    if (strcmp(palabra, "stosw") == 0) return 1;
        if (strcmp(palabra, "stosd") == 0) return 1;
    // 15,4 Comparación de cadenas
    if (strcmp(palabra, "cmpsb") == 0) return 1;
    if (strcmp(palabra, "cmpsw") == 0) return 1;
        if (strcmp(palabra, "cmpsd") == 0) return 1;
    
        // 15.5 PREFIJOS DE REPETICIÓN
    if (strcmp(palabra, "rep") == 0) return 1;
        if (strcmp(palabra, "repe") == 0) return 1;
        if (strcmp(palabra, "repz") == 0) return 1;
    if (strcmp(palabra, "repne") == 0) return 1;
        if (strcmp(palabra, "repnz") == 0) return 1;

    return 0;
}

// Comprueba si la palabra es una directiva del ensamblador
static int es_directiva(char *palabra){

    // Secciones del programa
    if (strcmp(palabra, "section") == 0) return 1;
    if (strcmp(palabra, ".data") == 0) return 1;
    if (strcmp(palabra, ".text") == 0) return 1;
    if (strcmp(palabra, ".bss") == 0) return 1;

    // Declaracion de simbolos
    if (strcmp(palabra, "global") == 0) return 1;
    if (strcmp(palabra, "extern") == 0) return 1;
    if (strcmp(palabra, "equ") == 0) return 1;

    // Declaracion de datos
    if (strcmp(palabra, "db") == 0) return 1;
    if (strcmp(palabra, "dw") == 0) return 1;
    if (strcmp(palabra, "dd") == 0) return 1;

    // Reserva de memoria
    if (strcmp(palabra, "resb") == 0) return 1;
    if (strcmp(palabra, "resw") == 0) return 1;
    if (strcmp(palabra, "resd") == 0) return 1;

    // Tamaño de los operandos
    if (strcmp(palabra, "byte") == 0) return 1;
    if (strcmp(palabra, "word") == 0) return 1;
    if (strcmp(palabra, "dword") == 0) return 1;

    if (strcmp(palabra, "bits") == 0) return 1;

    return 0;
}

// Decidir tipo numero. Evalua si es decimal, hexadecimal o binario
// 1. Recibe la palabra y calcula su longitud
static int tipo_numero(char *palabra){
// Durante prueba 1. Imprime 3, es decir token no imrprime o guarda tipo numero, solo detecta que son numeros

    int n = strlen(palabra);

    if (n == 0) // Comprobar si es cadena vacoa 
        return 0; 

// Numeros hexa terminen en h o 0x
// Numeros hexadecimales: 0xFF o 0FFh
    if (n > 1 && ((palabra[0] == '0' && palabra[1] == 'x') ||
                (palabra[n-1] == 'h' && isdigit((unsigned char)palabra[0])))){

        int inicio = 0;
        int fin = n;

        // Si empieza con 0x, ignoramos los primeros dos caracteres
        if (palabra[0] == '0' && palabra[1] == 'x'){
            inicio = 2;
        }
        // Si termina con h, ignoramos el ultimo caracter
        else if (palabra[n-1] == 'h'){
            fin = n-1;
        }

        // Comprobamos que existan digitos
        if (inicio >= fin) return 0;

        // Comprobamos que todos los caracteres sean hexadecimales
        for (int i = inicio; i < fin; i++){
            if (!isxdigit((unsigned char)palabra[i])) return 0;
        }

        return 1;
    }


// Numeros binarios ya se que terminen con b o 0b 
    if (n > 1 && ((palabra[0] == '0' && palabra[1] == 'b') ||
                palabra[n-1] == 'b')){

        int inicio = 0;
        int fin = n;

        // Si empieza con 0b, ignoramos los primeros dos caracteres
        if (palabra[0] == '0' && palabra[1] == 'b'){
            inicio = 2;
        }
        // Si termina con b, ignoramos el ultimo caracter
        else if (palabra[n-1] == 'b'){
            fin = n-1;
        }

        // Comprobamos que existan digitos
        if (inicio >= fin) return 0;

        // Comprobamos que todos los digitos sean 0 o 1
        for (int i = inicio; i < fin; i++){
            if (palabra[i] != '0' && palabra[i] != '1') return 0;
        }

        return 1;
    }

// Numeros decimales
    for (int i = 0; i < n; i++){
        if (!isdigit((unsigned char)palabra[i])) return 0;
    }

    return 1;
}

// Funcion que indistantemente de lo que haya en el archivo lo pasara a minusculas
static void minusculas(char *palabra){

    for (int i = 0; palabra[i] != '\0'; i++){
        palabra[i] = tolower((unsigned char)palabra[i]);
    }
}

// Clasificacion del tipo de token al que pertenece la palabra
static TipoToken clasificar_palabra(char *palabra){

    // Reservamos memoria para copiar la palabra
    // El +1 es para guardar tambien el caracter '\0'
    char *copia = malloc((strlen(palabra)+1)*sizeof(char));

    if (copia == NULL){
        perror("Error al reservar memoria para la palabra");
        return TOKEN_DESCONOCIDO;
    }

    strcpy(copia, palabra);              // Copiamos para no modificar la original

    minusculas(copia);                   // Funcion que hicimos al principio

    // Si no coincide con ninguna de las categorias de abajo,
    // suponemos que es un identificador (una etiqueta, variable, etc.)
    TipoToken tipo = TOKEN_IDENTIFICADOR;

    if (es_registro(copia) == 1){
        tipo = TOKEN_REGISTRO;
    }
    else if (es_mnemonico(copia) == 1){
        tipo = TOKEN_MNEMONICO;
    }
    else if (es_directiva(copia) == 1){
        tipo = TOKEN_DIRECTIVA;
    }
    else if (tipo_numero(copia) == 1){
        tipo = TOKEN_NUMERO;
    }

    free(copia);                         // Ya no necesitamos la copia

    return tipo;                         // Regresamos la categoria encontrada
}

// Prueba 1. Ver que imprime 
// Funcion temporal para comprobar que funciona la clasificacion

/*
mov: 0
eax: 1
section: 8
255: 3
0xFF: 3
1010b: 3
mensaje: 2
MOV: 0
*/ 

/*
Funcion para almacenar los tokens en memoria 
// Por la estructura en lexer. h seria algo asi
token[0]:
mov
tipo = TOKEN_MNEMONICO
numeroLinea = 1
*/

// Prueba 2. Guardar cada palabra que ya clasificamos dentro de un arreglo dinamico de estructuras
// Funcion para guardar los tokens que ya fueron clasificados
static int agregar_token(Token **tokens, int *total,
                         TipoToken tipo, char *palabra,
                         size_t numeroLinea){

    // Reservamos espacio para un token nuevo
    Token *temp = realloc(*tokens, (*total+1)*sizeof(Token));

    if (temp == NULL){
        perror("Error en REALLOC al asignar memoria");
        return 0;
    }

    *tokens = temp;

    // Reservamos memoria para guardar la palabra
    char *lexema = malloc((strlen(palabra)+1)*sizeof(char));

    if (lexema == NULL){
        perror("Error en MALLOC al asignar memoria");
        return 0;
    }

    strcpy(lexema, palabra);

    // Se guardan los datos del token llenando los tres campos que se definio en la estructura de token en lexer.h
    (*tokens)[*total].tipo = tipo;
    (*tokens)[*total].lexema = lexema;
    (*tokens)[*total].numeroLinea = numeroLinea;

    (*total)++;

    return 1;
}

/*
Lexema: mov | Tipo: 0 | Linea: 1
Lexema: eax | Tipo: 1 | Linea: 1
Lexema: , | Tipo: 4 | Linea: 1
Lexema: 15 | Tipo: 3 | Linea: 1
Total de tokens: 4
*/

// Liberamos la memoria utilizada por los tokens
void liberar_tokens(Token *tokens, int total){

    for (int i = 0; i < total; i++){
        free(tokens[i].lexema);
    }

    free(tokens);
}

// Funcion que supuestamente deberia de recorrer caracter por caracter y separar las palabras y los simbolos
// Analiza una linea del archivo caracter por caracter
static int analizar_linea(char *linea, size_t numeroLinea,
                          Token **tokens, int *total){

    int i = 0;
    int n = strlen(linea);

    while (i < n){

        // Ignoramos espacios y tabulaciones
        if (isspace((unsigned char)linea[i])){
            i++;
            continue;
        }

        // Si encontramos un comentario dejamos de leer la linea
        if (linea[i] == ';'){
            break;
        }

        // Reconocimiento de simbolos
        TipoToken tipo = TOKEN_DESCONOCIDO;

        if (linea[i] == ','){
            tipo = TOKEN_COMA;
        }
        else if (linea[i] == '['){
            tipo = TOKEN_CORCHETE_ABRE;
        }
        else if (linea[i] == ']'){
            tipo = TOKEN_CORCHETE_CIERRA;
        }
        else if (linea[i] == '+' || linea[i] == '-' ||
                 linea[i] == '*' || linea[i] == '/'){
            tipo = TOKEN_OPERADOR;
        }

        // Si encontramos un simbolo lo guardamos
        if (tipo != TOKEN_DESCONOCIDO){

            char simbolo[2];

            simbolo[0] = linea[i];
            simbolo[1] = '\0';

            if (agregar_token(tokens, total, tipo,
                              simbolo, numeroLinea) == 0){
                return 0;
            }

            i++;
            continue;
        }

        // Reconocimiento de palabras y numeros
        if (isalnum((unsigned char)linea[i]) ||
            linea[i] == '_' || linea[i] == '.'){

            int inicio = i;

            // Avanzamos hasta encontrar el final de la palabra
            while (i < n &&
                  (isalnum((unsigned char)linea[i]) ||
                   linea[i] == '_' || linea[i] == '.')){

                i++;
            }

            int longitud = i-inicio;

            // Reservamos memoria para la palabra encontrada
            char *palabra = malloc((longitud+1)*sizeof(char));

            if (palabra == NULL){
                perror("Error al reservar memoria");
                return 0;
            }

            // Copiamos los caracteres de la palabra
            for (int j = 0; j < longitud; j++){
                palabra[j] = linea[inicio+j];
            }

            palabra[longitud] = '\0';

            // Clasificamos la palabra
            tipo = clasificar_palabra(palabra);

            // Guardamos el token
            if (agregar_token(tokens, total, tipo,
                              palabra, numeroLinea) == 0){

                free(palabra);
                return 0;
            }

            free(palabra);
            continue;
        }

        // Si el caracter no pertenece a ninguna categoria
        char desconocido[2];

        desconocido[0] = linea[i];
        desconocido[1] = '\0';

        if (agregar_token(tokens, total, TOKEN_DESCONOCIDO,
                          desconocido, numeroLinea) == 0){
            return 0;
        }

        i++;
    }

    return 1;
}

/*
Prueba 3: Ver que si funcione funcion analizar_linea
*/
/*Lexema: mov | Tipo: 0 | Linea: 1
Lexema: eax | Tipo: 1 | Linea: 1
Lexema: , | Tipo: 4 | Linea: 1
Lexema: [ | Tipo: 5 | Linea: 1
Lexema: ebp | Tipo: 1 | Linea: 1
Lexema: - | Tipo: 7 | Linea: 1
Lexema: 4 | Tipo: 3 | Linea: 1
Lexema: ] | Tipo: 6 | Linea: 1
Total de tokens: 8
*/

// Recibimos todas las lineas que guardo el main en 3aetapa.c
Token *analizar_lineas(char **lineas, int n, int *total){

    Token *tokens = NULL;
    *total = 0;

    // Recorremos todas las lineas del archivo
    for (int i = 0; i < n; i++){

        // Analizamos cada linea individualmente
        if (analizar_linea(lineas[i], i+1,
                           &tokens, total) == 0){

            liberar_tokens(tokens, *total);
            *total = -1;

            return NULL;
        }
    }

    return tokens;
}

// Imprimimos todos los tokens encontrados
void imprimir_tokens(Token *tokens, int total){

    for (int i = 0; i < total; i++){

        printf("Lexema: %s | Tipo: %d | Linea: %zu\n",
               tokens[i].lexema,
               tokens[i].tipo,
               tokens[i].numeroLinea);
    }

    printf("Total de tokens: %d\n", total);
}