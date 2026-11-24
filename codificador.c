#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "codificador.h"

// Obtiene el código numérico de 3 bits (0 a 7) de un registro de 32 bits IA-32
static int obtener_codigo_registro32(const char *reg) {
    if (strcmp(reg, "EAX") == 0) return 0; // 000
    if (strcmp(reg, "ECX") == 0) return 1; // 001
    if (strcmp(reg, "EDX") == 0) return 2; // 010
    if (strcmp(reg, "EBX") == 0) return 3; // 011
    if (strcmp(reg, "ESP") == 0) return 4; // 100
    if (strcmp(reg, "EBP") == 0) return 5; // 101
    if (strcmp(reg, "ESI") == 0) return 6; // 110
    if (strcmp(reg, "EDI") == 0) return 7; // 111
    return -1;
}

// Construye el byte ModR/M: [ mod (2 bits) | reg (3 bits) | r/m (3 bits) ]
static uint8_t calcular_modrm(uint8_t mod, uint8_t reg, uint8_t rm) {
    return (uint8_t)(((mod & 0x03) << 6) | ((reg & 0x07) << 3) | (rm & 0x07));
}

// Convierte una cadena numérica (decimal, 0xHex, 10h, 1010b) a entero de 32 bits
static bool parsear_inmediato32(const char *texto, uint32_t *resultado) {
    char *fin = NULL;
    size_t len = strlen(texto);
    if (len == 0) return false;

    // Caso 1: Sufijo 'h' (hexadecimal estilo Intel/NASM, ej. 10h)
    char ultimo = (char)tolower((unsigned char)texto[len - 1]);
    if (ultimo == 'h' && len > 1) {
        char copia[32];
        if (len >= sizeof(copia)) return false;
        memcpy(copia, texto, len - 1);
        copia[len - 1] = '\0';
        long long val = strtoll(copia, &fin, 16);
        if (*fin == '\0') {
            *resultado = (uint32_t)val;
            return true;
        }
        return false;
    }

    // Caso 2: Sufijo 'b' (binario estilo Intel/NASM, ej. 1010b)
    if (ultimo == 'b' && len > 1 && !(texto[0] == '0' && tolower((unsigned char)texto[1]) == 'x')) {
        char copia[32];
        if (len >= sizeof(copia)) return false;
        memcpy(copia, texto, len - 1);
        copia[len - 1] = '\0';
        long long val = strtoll(copia, &fin, 2);
        if (*fin == '\0') {
            *resultado = (uint32_t)val;
            return true;
        }
        return false;
    }

    // Caso 3: Decimal estándar o hexadecimal con prefijo 0x
    long long val = strtoll(texto, &fin, 0);
    if (*fin == '\0' && fin != texto) {
        *resultado = (uint32_t)val;
        return true;
    }

    return false;
}

// Escribe un valor de 32 bits en 4 bytes respetando el orden Little-Endian
static void escribir_little_endian32(uint32_t valor, uint8_t destino[4]) {
    destino[0] = (uint8_t)(valor & 0xFF);         // Byte menos significativo (LSB)
    destino[1] = (uint8_t)((valor >> 8) & 0xFF);
    destino[2] = (uint8_t)((valor >> 16) & 0xFF);
    destino[3] = (uint8_t)((valor >> 24) & 0xFF); // Byte más significativo (MSB)
}

bool codificar_instruccion(const Instruccion *inst, CodigoMaquina *codigo) {
    if (inst == NULL || codigo == NULL) return false;

    memset(codigo, 0, sizeof(CodigoMaquina));

    // Verificamos que tenga 2 operandos (para MOV, ADD, SUB)
    if (inst->cantidadOperandos != 2) {
        fprintf(stderr, "[Error Codificador - Linea %zu]: '%s' requiere 2 operandos (recibidos: %zu)\n",
                inst->numeroLinea, inst->mnemonico, inst->cantidadOperandos);
        return false;
    }

    const Operando *op1 = &inst->operandos[0]; // Destino
    const Operando *op2 = &inst->operandos[1]; // Fuente

    // =========================================================================
    // 1. INSTRUCCIÓN MOV
    // =========================================================================
    if (strcmp(inst->mnemonico, "MOV") == 0) {
        // Forma 1: MOV reg32, imm32  -> Opcode: 0xB8 + cod_reg, seguido de imm32 (4 bytes)
        if (op1->tipo == OPERANDO_REGISTRO && op1->tamano == 32 && op2->tipo == OPERANDO_INMEDIATO) {
            int reg_dest = obtener_codigo_registro32(op1->valor);
            uint32_t inm = 0;

            if (reg_dest < 0 || !parsear_inmediato32(op2->valor, &inm)) {
                fprintf(stderr, "[Error Codificador - Linea %zu]: Operandos invalidos en MOV reg, imm\n",
                        inst->numeroLinea);
                return false;
            }

            codigo->bytes[0] = (uint8_t)(0xB8 + reg_dest);
            escribir_little_endian32(inm, &codigo->bytes[1]);
            codigo->longitud = 5;
            return true;
        }

        // Forma 2: MOV reg32, reg32 -> Opcode: 0x89 + ModR/M (mod=11, reg=fuente, r/m=destino)
        if (op1->tipo == OPERANDO_REGISTRO && op1->tamano == 32 &&
            op2->tipo == OPERANDO_REGISTRO && op2->tamano == 32) {
            int reg_dest = obtener_codigo_registro32(op1->valor);
            int reg_fte  = obtener_codigo_registro32(op2->valor);

            if (reg_dest < 0 || reg_fte < 0) return false;

            codigo->bytes[0] = 0x89;
            codigo->bytes[1] = calcular_modrm(0b11, (uint8_t)reg_fte, (uint8_t)reg_dest);
            codigo->longitud = 2;
            return true;
        }
    }

    // =========================================================================
    // 2. INSTRUCCIÓN ADD
    // =========================================================================
    if (strcmp(inst->mnemonico, "ADD") == 0) {
        // Forma: ADD reg32, reg32 -> Opcode: 0x01 + ModR/M (mod=11, reg=fuente, r/m=destino)
        if (op1->tipo == OPERANDO_REGISTRO && op1->tamano == 32 &&
            op2->tipo == OPERANDO_REGISTRO && op2->tamano == 32) {
            int reg_dest = obtener_codigo_registro32(op1->valor);
            int reg_fte  = obtener_codigo_registro32(op2->valor);

            if (reg_dest < 0 || reg_fte < 0) return false;

            codigo->bytes[0] = 0x01;
            codigo->bytes[1] = calcular_modrm(0b11, (uint8_t)reg_fte, (uint8_t)reg_dest);
            codigo->longitud = 2;
            return true;
        }
    }

    // =========================================================================
    // 3. INSTRUCCIÓN SUB
    // =========================================================================
    if (strcmp(inst->mnemonico, "SUB") == 0) {
        // Forma: SUB reg32, reg32 -> Opcode: 0x29 + ModR/M (mod=11, reg=fuente, r/m=destino)
        if (op1->tipo == OPERANDO_REGISTRO && op1->tamano == 32 &&
            op2->tipo == OPERANDO_REGISTRO && op2->tamano == 32) {
            int reg_dest = obtener_codigo_registro32(op1->valor);
            int reg_fte  = obtener_codigo_registro32(op2->valor);

            if (reg_dest < 0 || reg_fte < 0) return false;

            codigo->bytes[0] = 0x29;
            codigo->bytes[1] = calcular_modrm(0b11, (uint8_t)reg_fte, (uint8_t)reg_dest);
            codigo->longitud = 2;
            return true;
        }
    }

    // Si llega aquí, la combinación aún no está soportada en esta etapa
    fprintf(stderr, "[Aviso Codificador - Linea %zu]: Instruccion o combinacion de operandos no soportada (%s)\n",
            inst->numeroLinea, inst->mnemonico);
    return false;
}

void imprimir_codigo_hexadecimal(const CodigoMaquina *codigo) {
    if (codigo == NULL) return;
    for (size_t i = 0; i < codigo->longitud; i++) {
        printf("%02X", codigo->bytes[i]);
        if (i + 1 < codigo->longitud) printf(" ");
    }
    printf("\n");
}
