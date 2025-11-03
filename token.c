#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

// Crear un nuevo token
Token* crear_token(TipoToken tipo, const char *lexema, int linea) {
    Token *token = (Token*)malloc(sizeof(Token));
    token->tipo = tipo;
    token->lexema = strdup(lexema);
    token->linea = linea;
    
    // Inicializar valores
    token->valor.valor_int = 0;
    token->valor.valor_string = NULL;
    
    return token;
}

// Destruir un token y liberar memoria
void destruir_token(Token *token) {
    if (token) {
        if (token->lexema) free(token->lexema);
        if (token->tipo == TK_LITSTRING && token->valor.valor_string) {
            free(token->valor.valor_string);
        }
        free(token);
    }
}

// Obtener nombre del tipo de token
const char* tipo_token_str(TipoToken tipo) {
    switch(tipo) {
        case TK_IF: return "IF";
        case TK_ELSE: return "ELSE";
        case TK_END: return "END";
        case TK_WHILE: return "WHILE";
        case TK_LOOP: return "LOOP";
        case TK_FUN: return "FUN";
        case TK_RETURN: return "RETURN";
        case TK_NEW: return "NEW";
        case TK_STRING: return "STRING";
        case TK_INT: return "INT";
        case TK_CHAR: return "CHAR";
        case TK_BOOL: return "BOOL";
        case TK_TRUE: return "TRUE";
        case TK_FALSE: return "FALSE";
        case TK_AND: return "AND";
        case TK_OR: return "OR";
        case TK_NOT: return "NOT";
        case TK_LPAREN: return "LPAREN";
        case TK_RPAREN: return "RPAREN";
        case TK_COMMA: return "COMMA";
        case TK_COLON: return "COLON";
        case TK_GT: return "GT";
        case TK_LT: return "LT";
        case TK_GE: return "GE";
        case TK_LE: return "LE";
        case TK_EQ: return "EQ";
        case TK_NE: return "NE";
        case TK_LBRACKET: return "LBRACKET";
        case TK_RBRACKET: return "RBRACKET";
        case TK_PLUS: return "PLUS";
        case TK_MINUS: return "MINUS";
        case TK_MULT: return "MULT";
        case TK_DIV: return "DIV";
        case TK_ID: return "ID";
        case TK_LITNUMERAL: return "LITNUMERAL";
        case TK_LITSTRING: return "LITSTRING";
        case TK_NL: return "NL";
        case TK_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

// Imprimir un token con su información
void imprimir_token(Token *token) {
    printf("Linea %3d: %-15s", token->linea, tipo_token_str(token->tipo));
    
    switch(token->tipo) {
        case TK_ID:
            printf(" [%s]", token->lexema);
            break;
        case TK_LITNUMERAL:
            printf(" [%s] -> valor: %d", token->lexema, token->valor.valor_int);
            break;
        case TK_LITSTRING:
            printf(" [%s] -> valor: \"%s\"", token->lexema, token->valor.valor_string);
            break;
        case TK_ERROR:
            printf(" [%s]", token->lexema);
            break;
        default:
            printf(" [%s]", token->lexema);
            break;
    }
    
    printf("\n");
}