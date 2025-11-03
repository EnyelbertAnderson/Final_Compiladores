#ifndef TOKEN_H
#define TOKEN_H

// Tipos de tokens
typedef enum {
    // Palabras reservadas
    TK_IF,
    TK_ELSE,
    TK_END,
    TK_WHILE,
    TK_LOOP,
    TK_FUN,
    TK_RETURN,
    TK_NEW,
    TK_STRING,
    TK_INT,
    TK_CHAR,
    TK_BOOL,
    TK_TRUE,
    TK_FALSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    
    // Operadores y puntuación
    TK_LPAREN,      // (
    TK_RPAREN,      // )
    TK_COMMA,       // ,
    TK_COLON,       // :
    TK_GT,          // >
    TK_LT,          // 
    TK_GE,          // >=
    TK_LE,          // <=
    TK_EQ,          // =
    TK_NE,          // <>
    TK_LBRACKET,    // [
    TK_RBRACKET,    // ]
    TK_PLUS,        // +
    TK_MINUS,       // -
    TK_MULT,        // *
    TK_DIV,         // /
    
    // Identificadores y literales
    TK_ID,
    TK_LITNUMERAL,
    TK_LITSTRING,
    
    // Salto de línea
    TK_NL,
    
    // Error
    TK_ERROR
} TipoToken;

// Estructura para almacenar un token
typedef struct {
    TipoToken tipo;
    char *lexema;       // Texto original del token
    union {
        int valor_int;
        char *valor_string;
    } valor;
    int linea;
} Token;

// Prototipos de funciones
Token* crear_token(TipoToken tipo, const char *lexema, int linea);
void destruir_token(Token *token);
void imprimir_token(Token *token);
const char* tipo_token_str(TipoToken tipo);

#endif