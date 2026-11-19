#ifndef lexer_h
#define lexer_h

typedef enum{
    TOKEN_MNEMONICO,        // MOV, ADD
    TOKEN_REGISTRO,         // Ej. EAX
    TOKEN_IDENTIFICADOR,    // "while:"
    TOKEN_NUMERO,           // "10"
    TOKEN_COMA,             // ","
    TOKEN_CORCHETE_ABRE,    // "["
    TOKEN_CORCHETE_CIERRA,  // "]"
    TOKEN_OPERADOR,         
    TOKEN_DIRECTIVA,        // "section", ".data"
    TOKEN_DESCONOCIDO
} TipoToken;

typedef struct {
    TipoToken tipo;
    char *lexema;
    size_t numeroLinea;
} Token;


#endif                      //lexer.h