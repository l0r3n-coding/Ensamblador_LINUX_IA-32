#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codificador.h"

void liberar_lineas(char **lineas, int n) {
    if (lineas == NULL) return;
    for (int i = 0; i < n; i++) {
        free(lineas[i]);
    }
    free(lineas);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <archivo.asm>\n", argv[0]);
        return -1;
    }

    FILE *archivo = fopen(argv[1], "r");
    if (archivo == NULL) {
        perror("No se pudo abrir el archivo");
        return -1;
    }

    char **lineas = NULL;
    char *linea_actual = NULL;
    int n = 0;
    size_t len = 0;

    while (getline(&linea_actual, &len, archivo) != -1) {
        linea_actual[strcspn(linea_actual, "\r\n")] = '\0';

        char **temp = realloc(lineas, (n + 1) * sizeof(char *));
        if (temp == NULL) {
            perror("Error en realloc");
            free(linea_actual);
            liberar_lineas(lineas, n);
            fclose(archivo);
            return -1;
        }
        lineas = temp;
        lineas[n] = linea_actual;
        linea_actual = NULL;
        len = 0;
        n++;
    }
    free(linea_actual);
    fclose(archivo);

    // 1. FASE LÉXICA
    size_t total_tokens = 0;
    Token *tokens = tokenizar_archivo(lineas, n, &total_tokens);
    if (tokens == NULL) {
        liberar_lineas(lineas, n);
        return -1;
    }

    printf("=== 1. ANALISIS LEXICO ===\n");
    imprimir_tokens(tokens, total_tokens);

    // 2. FASE SINTÁCTICA Y CODIFICACIÓN (Agrupando tokens por línea)
    printf("\n=== 2. ANALISIS SINTACTICO Y CODIGO MAQUINA ===\n");
    size_t inicio_linea = 0;

    for (size_t i = 0; i < total_tokens; i++) {
        if (tokens[i].tipo == TOKEN_FIN_LINEA) {
            size_t cant_tokens_linea = i - inicio_linea;

            if (cant_tokens_linea > 0) {
                // Si la línea empieza con una directiva (ej. "BITS 32"), la reconocemos y seguimos
                if (tokens[inicio_linea].tipo == TOKEN_DIRECTIVA) {
                    printf("Linea %zu -> Directiva detectada: %s (omitida de codificacion)\n",
                           tokens[inicio_linea].numeroLinea, tokens[inicio_linea].lexema);
                } else {
                    Instruccion inst;
                    if (analizar_instruccion(&tokens[inicio_linea], cant_tokens_linea, &inst)) {
                        imprimir_instruccion(&inst);

                        CodigoMaquina codigo;
                        if (codificar_instruccion(&inst, &codigo)) {
                            printf("   Codigo Maquina Hex: ");
                            imprimir_codigo_hexadecimal(&codigo);
                        }
                    }
                }
            }
            inicio_linea = i + 1;
        }
    }

    liberar_tokens(tokens, total_tokens);
    liberar_lineas(lineas, n);
    return 0;
}