#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

// Convierte una cadena a mayúsculas para normalizar mnemónicos y registros ("mov" -> "MOV")
static void a_mayusculas(const char *origen, char *destino, size_t max_len) {
    size_t i = 0;
    while (origen[i] != '\0' && i < max_len - 1) {
        destino[i] = (char)toupper((unsigned char)origen[i]);
        i++;
    }
    destino[i] = '\0';
}

// Determina el tamaño en bits de un registro IA-32 según su nombre
static int obtener_tamano_registro(const char *reg_mayus) {
    // Registros de 32 bits
    if (strcmp(reg_mayus, "EAX") == 0 || strcmp(reg_mayus, "EBX") == 0 ||
        strcmp(reg_mayus, "ECX") == 0 || strcmp(reg_mayus, "EDX") == 0 ||
        strcmp(reg_mayus, "ESI") == 0 || strcmp(reg_mayus, "EDI") == 0 ||
        strcmp(reg_mayus, "ESP") == 0 || strcmp(reg_mayus, "EBP") == 0) {
        return 32;
    }
    // Registros de 16 bits
    if (strcmp(reg_mayus, "AX") == 0 || strcmp(reg_mayus, "BX") == 0 ||
        strcmp(reg_mayus, "CX") == 0 || strcmp(reg_mayus, "DX") == 0 ||
        strcmp(reg_mayus, "SI") == 0 || strcmp(reg_mayus, "DI") == 0 ||
        strcmp(reg_mayus, "SP") == 0 || strcmp(reg_mayus, "BP") == 0) {
        return 16;
    }
    // Registros de 8 bits
    if (strcmp(reg_mayus, "AL") == 0 || strcmp(reg_mayus, "AH") == 0 ||
        strcmp(reg_mayus, "BL") == 0 || strcmp(reg_mayus, "BH") == 0 ||
        strcmp(reg_mayus, "CL") == 0 || strcmp(reg_mayus, "CH") == 0 ||
        strcmp(reg_mayus, "DL") == 0 || strcmp(reg_mayus, "DH") == 0) {
        return 8;
    }
    return 0;
}

// Analiza un operando individual comprendido entre los índices [inicio, fin) de tokens
static bool analizar_operando(const Token *tokens, size_t inicio, size_t fin, Operando *op) {
    if (inicio >= fin) return false;

    size_t num_tokens = fin - inicio;
    memset(op, 0, sizeof(Operando));

    // Caso 1: Operando simple de 1 solo token (Registro, Inmediato o Etiqueta)
    if (num_tokens == 1) {
        const Token *t = &tokens[inicio];
        if (t->tipo == TOKEN_REGISTRO) {
            op->tipo = OPERANDO_REGISTRO;
            a_mayusculas(t->valor ? "" : t->lexema, op->valor, sizeof(op->valor));
            a_mayusculas(t->lexema, op->valor, sizeof(op->valor));
            op->tamano = obtener_tamano_registro(op->valor);
            return true;
        }
        if (t->tipo == TOKEN_NUMERO) {
            op->tipo = OPERANDO_INMEDIATO;
            strncpy(op->valor, t->lexema, sizeof(op->valor) - 1);
            op->tamano = 32; // Por defecto en modo 32 bits
            return true;
        }
        if (t->tipo == TOKEN_IDENTIFICADOR) {
            op->tipo = OPERANDO_ETIQUETA;
            strncpy(op->valor, t->lexema, sizeof(op->valor) - 1);
            op->tamano = 32;
            return true;
        }
        return false;
    }

    // Caso 2: Número negativo (ej. -10 -> TOKEN_OPERADOR "-" seguido de TOKEN_NUMERO "10")
    if (num_tokens == 2 && tokens[inicio].tipo == TOKEN_OPERADOR &&
        strcmp(tokens[inicio].lexema, "-") == 0 && tokens[inicio + 1].tipo == TOKEN_NUMERO) {
        op->tipo = OPERANDO_INMEDIATO;
        snprintf(op->valor, sizeof(op->valor), "-%s", tokens[inicio + 1].lexema);
        op->tamano = 32;
        return true;
    }

    // Caso 3: Direccionamiento de memoria entre corchetes (ej. [EBX+4])
    if (tokens[inicio].tipo == TOKEN_CORCHETE_ABRE &&
        tokens[fin - 1].tipo == TOKEN_CORCHETE_CIERRA && num_tokens >= 3) {
        op->tipo = OPERANDO_MEMORIA;
        op->tamano = 32;
        op->valor[0] = '\0';

        // Reconstruimos la expresión completa dentro de op->valor
        for (size_t i = inicio; i < fin; i++) {
            // Validamos que dentro de los corchetes solo haya registros, números u operadores
            if (i > inicio && i < fin - 1) {
                TipoToken tt = tokens[i].tipo;
                if (tt != TOKEN_REGISTRO && tt != TOKEN_NUMERO &&
                    tt != TOKEN_OPERADOR && tt != TOKEN_IDENTIFICADOR) {
                    return false;
                }
            }
            strncat(op->valor, tokens[i].lexema, sizeof(op->valor) - strlen(op->valor) - 1);
        }
        return true;
    }

    return false;
}

bool analizar_instruccion(const Token *tokens, size_t cantidad, Instruccion *instruccion) {
    if (tokens == NULL || cantidad == 0 || instruccion == NULL) return false;

    memset(instruccion, 0, sizeof(Instruccion));
    instruccion->numeroLinea = tokens[0].numeroLinea;

    size_t idx = 0;

    // Si la línea inicia con una etiqueta (ej. "inicio:"), la saltamos para leer la instrucción
    if (tokens[idx].tipo == TOKEN_IDENTIFICADOR) {
        size_t len = strlen(tokens[idx].lexema);
        if (len > 0 && tokens[idx].lexema[len - 1] == ':') {
            idx++;
            if (idx >= cantidad) {
                // La línea solo tenía una etiqueta (válido, pero no genera instrucción)
                return false;
            }
        }
    }

    // El primer token útil debe ser un MNEMÓNICO
    if (tokens[idx].tipo != TOKEN_MNEMONICO) {
        fprintf(stderr, "[Error Sintactico - Linea %zu]: Se esperaba un mnemonico, se encontro '%s'\n",
                tokens[idx].numeroLinea, tokens[idx].lexema);
        return false;
    }

    a_mayusculas(tokens[idx].lexema, instruccion->mnemonico, sizeof(instruccion->mnemonico));
    idx++;

    // Si ya no hay más tokens, es una instrucción de 0 operandos (ej. NOP, RET)
    if (idx >= cantidad) {
        instruccion->cantidadOperandos = 0;
        return true;
    }

    // Buscar si existe una COMA separando dos operandos
    size_t pos_coma = cantidad;
    int conteo_comas = 0;
    for (size_t i = idx; i < cantidad; i++) {
        if (tokens[i].tipo == TOKEN_COMA) {
            if (conteo_comas == 0) pos_coma = i;
            conteo_comas++;
        }
    }

    if (conteo_comas > 1) {
        fprintf(stderr, "[Error Sintactico - Linea %zu]: Demasiadas comas en la instruccion '%s'\n",
                instruccion->numeroLinea, instruccion->mnemonico);
        return false;
    }

    // Caso A: Instrucción de 1 operando (no hay coma)
    if (conteo_comas == 0) {
        if (!analizar_operando(tokens, idx, cantidad, &instruccion->operandos[0])) {
            fprintf(stderr, "[Error Sintactico - Linea %zu]: Operando invalido en '%s'\n",
                    instruccion->numeroLinea, instruccion->mnemonico);
            return false;
        }
        instruccion->cantidadOperandos = 1;
        return true;
    }

    // Caso B: Instrucción de 2 operandos (hay 1 coma)
    if (pos_coma == idx || pos_coma == cantidad - 1) {
        fprintf(stderr, "[Error Sintactico - Linea %zu]: Falta operando antes o despues de la coma en '%s'\n",
                instruccion->numeroLinea, instruccion->mnemonico);
        return false;
    }

    if (!analizar_operando(tokens, idx, pos_coma, &instruccion->operandos[0])) {
        fprintf(stderr, "[Error Sintactico - Linea %zu]: Primer operando invalido en '%s'\n",
                instruccion->numeroLinea, instruccion->mnemonico);
        return false;
    }

    if (!analizar_operando(tokens, pos_coma + 1, cantidad, &instruccion->operandos[1])) {
        fprintf(stderr, "[Error Sintactico - Linea %zu]: Segundo operando invalido en '%s'\n",
                instruccion->numeroLinea, instruccion->mnemonico);
        return false;
    }

    instruccion->cantidadOperandos = 2;
    return true;
}

void imprimir_instruccion(const Instruccion *inst) {
    printf("Linea %zu -> Mnemonico: %-5s | Operandos (%zu): ",
           inst->numeroLinea, inst->mnemonico, inst->cantidadOperandos);
    for (size_t i = 0; i < inst->cantidadOperandos; i++) {
        const char *tipo_str = (inst->operandos[i].tipo == OPERANDO_REGISTRO)  ? "REG" :
                               (inst->operandos[i].tipo == OPERANDO_INMEDIATO) ? "IMM" :
                               (inst->operandos[i].tipo == OPERANDO_MEMORIA)   ? "MEM" : "ETQ";
        printf("[%s: %s (%d bits)] ", tipo_str, inst->operandos[i].valor, inst->operandos[i].tamano);
    }
    printf("\n");
}
