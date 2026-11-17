#ifndef lexer_h
#define lexer_h

typedef enum{
    TOKEN_MNEMONICO,
    TOKEN_REGISTRO,
    TOKEN_IDENTIFICADOR,
    TOKEN_NUMERO,
    TOKEN_COMA,
    TOKEN_CORCHETE_ABRE,
    TOKEN_CORCHETE_CIERRA,
    TOKEN_OPERADOR,
    TOKEN_DIRECTIVA,
    TOKEN_FIN_LINEA,
    TOKEN_DESCONOCIDO
}

typedef struct {
    TipoToken tipo;
    char *lexema;
    size_t numeroLinea;
    size_t columna;
} Token;
};


#endif