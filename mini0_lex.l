%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"

// Lista de tokens encontrados
Token **tokens = NULL;
int num_tokens = 0;
int capacidad_tokens = 100;

// Número de línea actual
int linea_actual = 1;

// Función para agregar un token a la lista
void agregar_token(Token *token) {
    if (tokens == NULL) {
        tokens = (Token**)malloc(capacidad_tokens * sizeof(Token*));
    }
    if (num_tokens >= capacidad_tokens) {
        capacidad_tokens *= 2;
        tokens = (Token**)realloc(tokens, capacidad_tokens * sizeof(Token*));
    }
    tokens[num_tokens++] = token;
}

// Función para procesar escapes en strings
char* procesar_string(const char *str) {
    int len = strlen(str);
    char *resultado = (char*)malloc(len + 1);
    int j = 0;
    
    // Saltar las comillas inicial y final
    for (int i = 1; i < len - 1; i++) {
        if (str[i] == '\\' && i + 1 < len - 1) {
            switch(str[i + 1]) {
                case 'n': resultado[j++] = '\n'; i++; break;
                case 't': resultado[j++] = '\t'; i++; break;
                case '\\': resultado[j++] = '\\'; i++; break;
                case '"': resultado[j++] = '"'; i++; break;
                default: resultado[j++] = str[i]; break;
            }
        } else {
            resultado[j++] = str[i];
        }
    }
    resultado[j] = '\0';
    return resultado;
}

// Función para convertir hexadecimal a decimal
int hex_to_dec(const char *hex) {
    int valor;
    sscanf(hex, "%x", &valor);
    return valor;
}

%}

/* Definiciones de patrones */
LETRA           [a-zA-Z]
DIGITO          [0-9]
HEXDIGITO       [0-9a-fA-F]
ID              ({LETRA}|_)({LETRA}|{DIGITO}|_)*
DECIMAL         {DIGITO}+
HEXADECIMAL     0x{HEXDIGITO}+
STRING          \"([^\\\"\n]|\\.)*\"
ESPACIO         [ \t\r]
SALTO_LINEA     \n

/* Opciones de Flex */
%option noyywrap

%%

    /* Comentarios */
"//".*                  { /* Comentario de una línea - ignorar */ }
"/*"                    {
                            int c;
                            while ((c = input()) != 0) {
                                if (c == '\n') linea_actual++;
                                if (c == '*') {
                                    if ((c = input()) == '/') break;
                                    unput(c);
                                }
                            }
                        }

    /* Saltos de línea */
{SALTO_LINEA}           { 
                            Token *token = crear_token(TK_NL, "\\n", linea_actual);
                            agregar_token(token);
                            linea_actual++; 
                        }

    /* Espacios en blanco */
{ESPACIO}+              { /* Ignorar espacios y tabulaciones */ }

    /* Palabras reservadas */
"if"                    { agregar_token(crear_token(TK_IF, yytext, linea_actual)); }
"else"                  { agregar_token(crear_token(TK_ELSE, yytext, linea_actual)); }
"end"                   { agregar_token(crear_token(TK_END, yytext, linea_actual)); }
"while"                 { agregar_token(crear_token(TK_WHILE, yytext, linea_actual)); }
"loop"                  { agregar_token(crear_token(TK_LOOP, yytext, linea_actual)); }
"fun"                   { agregar_token(crear_token(TK_FUN, yytext, linea_actual)); }
"return"                { agregar_token(crear_token(TK_RETURN, yytext, linea_actual)); }
"new"                   { agregar_token(crear_token(TK_NEW, yytext, linea_actual)); }
"string"                { agregar_token(crear_token(TK_STRING, yytext, linea_actual)); }
"int"                   { agregar_token(crear_token(TK_INT, yytext, linea_actual)); }
"char"                  { agregar_token(crear_token(TK_CHAR, yytext, linea_actual)); }
"bool"                  { agregar_token(crear_token(TK_BOOL, yytext, linea_actual)); }
"true"                  { agregar_token(crear_token(TK_TRUE, yytext, linea_actual)); }
"false"                 { agregar_token(crear_token(TK_FALSE, yytext, linea_actual)); }
"and"                   { agregar_token(crear_token(TK_AND, yytext, linea_actual)); }
"or"                    { agregar_token(crear_token(TK_OR, yytext, linea_actual)); }
"not"                   { agregar_token(crear_token(TK_NOT, yytext, linea_actual)); }

    /* Operadores de dos caracteres (deben ir antes que los de uno) */
">="                    { agregar_token(crear_token(TK_GE, yytext, linea_actual)); }
"<="                    { agregar_token(crear_token(TK_LE, yytext, linea_actual)); }
"<>"                    { agregar_token(crear_token(TK_NE, yytext, linea_actual)); }

    /* Operadores y puntuación de un carácter */
"("                     { agregar_token(crear_token(TK_LPAREN, yytext, linea_actual)); }
")"                     { agregar_token(crear_token(TK_RPAREN, yytext, linea_actual)); }
","                     { agregar_token(crear_token(TK_COMMA, yytext, linea_actual)); }
":"                     { agregar_token(crear_token(TK_COLON, yytext, linea_actual)); }
">"                     { agregar_token(crear_token(TK_GT, yytext, linea_actual)); }
"<"                     { agregar_token(crear_token(TK_LT, yytext, linea_actual)); }
"="                     { agregar_token(crear_token(TK_EQ, yytext, linea_actual)); }
"["                     { agregar_token(crear_token(TK_LBRACKET, yytext, linea_actual)); }
"]"                     { agregar_token(crear_token(TK_RBRACKET, yytext, linea_actual)); }
"+"                     { agregar_token(crear_token(TK_PLUS, yytext, linea_actual)); }
"-"                     { agregar_token(crear_token(TK_MINUS, yytext, linea_actual)); }
"*"                     { agregar_token(crear_token(TK_MULT, yytext, linea_actual)); }
"/"                     { agregar_token(crear_token(TK_DIV, yytext, linea_actual)); }

    /* Literales numéricos */
{HEXADECIMAL}           {
                            Token *token = crear_token(TK_LITNUMERAL, yytext, linea_actual);
                            token->valor.valor_int = hex_to_dec(yytext + 2); // Saltar "0x"
                            agregar_token(token);
                        }
{DECIMAL}               {
                            Token *token = crear_token(TK_LITNUMERAL, yytext, linea_actual);
                            token->valor.valor_int = atoi(yytext);
                            agregar_token(token);
                        }

    /* Strings */
{STRING}                {
                            Token *token = crear_token(TK_LITSTRING, yytext, linea_actual);
                            token->valor.valor_string = procesar_string(yytext);
                            agregar_token(token);
                        }

    /* Identificadores */
{ID}                    { agregar_token(crear_token(TK_ID, yytext, linea_actual)); }

    /* Error: cualquier otro carácter */
.                       {
                            Token *token = crear_token(TK_ERROR, yytext, linea_actual);
                            agregar_token(token);
                            fprintf(stderr, "Error léxico en línea %d: carácter no reconocido '%s'\n", 
                                    linea_actual, yytext);
                        }

%%

// Función pública para analizar un archivo y llenar el buffer de tokens.
// Devuelve 0 en caso de éxito, distinto de cero en caso de error (p.ej. archivo no encontrado).
int analizar_lexico(const char *ruta_archivo) {
    // Reiniciar estructura de tokens si se reutiliza la misma ejecución
    if (tokens) {
        for (int i = 0; i < num_tokens; i++) {
            destruir_token(tokens[i]);
        }
        free(tokens);
        tokens = NULL;
        capacidad_tokens = 100;
        num_tokens = 0;
    }

    yyin = fopen(ruta_archivo, "r");
    if (!yyin) {
        fprintf(stderr, "Error: no se puede abrir el archivo '%s'\n", ruta_archivo);
        return 1;
    }

    // Ejecutar análisis léxico (llenará `tokens` y `num_tokens`)
    yylex();

    fclose(yyin);
    return 0;
}

/* Proporcionar main para el ejecutable `mini0_lex`.
   Muestra uso cuando no se proporciona archivo y, si se proporciona,
   ejecuta el análisis léxico e imprime los tokens (comportamiento previo).
*/
 #ifndef BUILD_WITH_PARSER_MAIN
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_mini0>\n", argv[0]);
        return 1;
    }

    int rc = analizar_lexico(argv[1]);
    if (rc != 0) return rc;

    // Imprimir todos los tokens encontrados
    printf("\n=== ANÁLISIS LÉXICO COMPLETADO ===\n");
    printf("Total de tokens encontrados: %d\n\n", num_tokens);

    for (int i = 0; i < num_tokens; i++) {
        imprimir_token(tokens[i]);
    }

    // Liberar memoria
    for (int i = 0; i < num_tokens; i++) {
        destruir_token(tokens[i]);
    }
    free(tokens);
    tokens = NULL;
    num_tokens = 0;

    return 0;
}
#endif