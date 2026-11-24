#ifndef CODIFICADOR_H
#define CODIFICADOR_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "parser.h"

typedef struct {
    uint8_t bytes[16];  // Una instrucción IA-32 mide máximo 15 bytes
    size_t longitud;
} CodigoMaquina;

// Traduce una Instruccion validada a sus bytes en código máquina IA-32
bool codificar_instruccion(const Instruccion *instruccion, CodigoMaquina *codigo);

// Imprime los bytes generados en formato hexadecimal (ej. B8 0A 00 00 00)
void imprimir_codigo_hexadecimal(const CodigoMaquina *codigo);

#endif // CODIFICADOR_H
