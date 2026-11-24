#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdbool.h>
#include "lexer.h"

typedef enum {
    OPERANDO_REGISTRO,
    OPERANDO_INMEDIATO,
    OPERANDO_MEMORIA,
    OPERANDO_ETIQUETA
} TipoOperando;

typedef struct {
    TipoOperando tipo;
    char valor[32];     // Ej. "EAX", "10", "[EBX+4]", "bucle"
    int tamano;         // En bits: 8, 16, 32 (o 0 si no aplica aún)
} Operando;

typedef struct {
    char mnemonico[10]; // Ej. "MOV", "ADD", "SUB"
    Operando operandos[2];
    size_t cantidadOperandos;
    size_t numeroLinea;
} Instruccion;

// Analiza los tokens correspondientes a UNA sola línea/instrucción
bool analizar_instruccion(const Token *tokens, size_t cantidad, Instruccion *instruccion);

// Función auxiliar de prueba para imprimir cómo quedó estructurada la instrucción
void imprimir_instruccion(const Instruccion *inst);

#endif // PARSER_H
