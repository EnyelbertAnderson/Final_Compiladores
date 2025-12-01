#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"

// Lexer-provided symbols
extern int analizar_lexico(const char *ruta_archivo);
// tokens, num_tokens, linea_actual are declared in token.h as extern

static int pos = 0; // índice del token actual
static bool had_error = false;
static bool panic_mode = false;

// Forward declarations
static void parse_program();
static void parse_declaration_or_function();
static void parse_statements_until(TipoToken terminator1, TipoToken terminator2, bool allow_else);
static void synchronize();

// ------------------------------------------------------------
// Helpers
// ------------------------------------------------------------

static Token *cur_token() {
    if (pos >= num_tokens) return NULL;
    return tokens[pos];
}

static Token *peek_token(int offset) {
    int idx = pos + offset;
    if (idx >= num_tokens) return NULL;
    return tokens[idx];
}

static void advance() {
    if (pos < num_tokens) pos++;
}

static void error_at_token(Token *tk, const char *msg) {
    if (panic_mode) return; // No reportar errores en cascada
    
    panic_mode = true;
    had_error = true;
    
    if (!tk) {
        fprintf(stderr, "Error sintáctico en EOF: %s\n", msg);
    } else {
        fprintf(stderr, "Error sintáctico en línea %d: %s (token: %s '%s')\n",
                tk->linea, msg, tipo_token_str(tk->tipo), tk->lexema);
    }
}

static void expect(TipoToken t, const char *msg) {
    Token *tk = cur_token();
    if (!tk || tk->tipo != t) {
        error_at_token(tk, msg);
        return;
    }
    advance();
}

static void skip_newlines() {
    while (cur_token() && cur_token()->tipo == TK_NL) {
        advance();
    }
}

static bool is_sync_token(TipoToken t) {
    return t == TK_NL || 
           t == TK_END || 
           t == TK_ELSE || 
           t == TK_LOOP ||
           t == TK_FUN ||
           t == TK_IF ||
           t == TK_WHILE ||
           t == TK_RETURN;
}

static void synchronize() {
    panic_mode = false;
    
    while (cur_token()) {
        Token *tk = cur_token();
        
        // Si encontramos un token de sincronización, detenemos
        if (is_sync_token(tk->tipo)) {
            // Si es newline, lo consumimos y salimos
            if (tk->tipo == TK_NL) {
                advance();
            }
            // Para otros tokens, no los consumimos (son inicio de statement)
            return;
        }
        
        advance();
    }
}

// ------------------------------------------------------------
// Expressions
// ------------------------------------------------------------

static void parse_expression();
static void parse_primary();

static void parse_primary() {
    Token *tk = cur_token();
    if (!tk) {
        error_at_token(NULL, "Se esperaba una expresión, se encontró EOF");
        return;
    }

    if (tk->tipo == TK_LITNUMERAL || tk->tipo == TK_LITSTRING ||
        tk->tipo == TK_TRUE || tk->tipo == TK_FALSE) {
        advance();
        return;
    }

    if (tk->tipo == TK_ID) {
        advance();
        while (cur_token()) {
            if (cur_token()->tipo == TK_LPAREN) {
                advance();
                if (cur_token() && cur_token()->tipo != TK_RPAREN) {
                    parse_expression();
                    while (cur_token() && cur_token()->tipo == TK_COMMA) {
                        advance();
                        parse_expression();
                    }
                }
                expect(TK_RPAREN, "Se esperaba ')' en llamada de función");
                if (panic_mode) return;
            }
            else if (cur_token()->tipo == TK_LBRACKET) {
                advance();
                parse_expression();
                expect(TK_RBRACKET, "Se esperaba ']' en acceso a arreglo");
                if (panic_mode) return;
            }
            else break;
        }
        return;
    }

    if (tk->tipo == TK_LPAREN) {
        advance();
        parse_expression();
        expect(TK_RPAREN, "Se esperaba ')' después de la expresión");
        return;
    }

    error_at_token(tk, "Expresión no válida");
}

static void parse_unary() {
    Token *tk = cur_token();
    if (tk && (tk->tipo == TK_MINUS || tk->tipo == TK_NOT)) {
        advance();
        parse_unary();
        return;
    }
    parse_primary();
}

static void parse_multiplicative() {
    parse_unary();
    while (cur_token() && (cur_token()->tipo == TK_MULT || cur_token()->tipo == TK_DIV)) {
        advance();
        parse_unary();
    }
}

static void parse_additive() {
    parse_multiplicative();
    while (cur_token() && (cur_token()->tipo == TK_PLUS || cur_token()->tipo == TK_MINUS)) {
        advance();
        parse_multiplicative();
    }
}

static void parse_relational() {
    parse_additive();
    while (cur_token() && (cur_token()->tipo == TK_GT || cur_token()->tipo == TK_LT ||
                           cur_token()->tipo == TK_GE || cur_token()->tipo == TK_LE)) {
        advance();
        parse_additive();
    }
}

static void parse_equality() {
    parse_relational();
    while (cur_token() && (cur_token()->tipo == TK_EQ || cur_token()->tipo == TK_NE)) {
        advance();
        parse_relational();
    }
}

static void parse_and_expr() {
    parse_equality();
    while (cur_token() && cur_token()->tipo == TK_AND) {
        advance();
        parse_equality();
    }
}

static void parse_or_expr() {
    parse_and_expr();
    while (cur_token() && cur_token()->tipo == TK_OR) {
        advance();
        parse_and_expr();
    }
}

static void parse_expression() {
    parse_or_expr();
}

// ------------------------------------------------------------
// Statements
// ------------------------------------------------------------

static bool is_statement_start(Token *tk) {
    if (!tk) return false;
    switch (tk->tipo) {
        case TK_ID:
        case TK_RETURN:
        case TK_IF:
        case TK_WHILE:
        case TK_LPAREN:
        case TK_LITNUMERAL:
        case TK_LITSTRING:
        case TK_MINUS:
        case TK_NOT:
        case TK_TRUE:
        case TK_FALSE:
            return true;
        default:
            return false;
    }
}

static void parse_type() {
    while (cur_token() && cur_token()->tipo == TK_LBRACKET) {
        advance();
        if (!cur_token() || cur_token()->tipo != TK_RBRACKET) {
            error_at_token(cur_token(), "Se esperaba ']' en tipo de arreglo");
            return;
        }
        advance();
    }

    if (!cur_token()) {
        error_at_token(NULL, "Se esperaba un tipo (int, bool, string, char)");
        return;
    }

    if (cur_token()->tipo == TK_INT || cur_token()->tipo == TK_BOOL ||
        cur_token()->tipo == TK_STRING || cur_token()->tipo == TK_CHAR) {
        advance();
        return;
    }

    error_at_token(cur_token(), "Tipo desconocido");
}

static void parse_statement() {
    Token *tk = cur_token();
    if (!tk) return;

    // Declaración de variable
    if (tk->tipo == TK_ID && peek_token(1) && peek_token(1)->tipo == TK_COLON) {
        advance();
        advance();
        parse_type();
        if (panic_mode) {
            synchronize();
            return;
        }
        skip_newlines();
        return;
    }

    // Asignación
    if (tk->tipo == TK_ID) {
        Token *next = peek_token(1);
        if (next && (next->tipo == TK_EQ || next->tipo == TK_LBRACKET)) {
            advance();
            if (cur_token() && cur_token()->tipo == TK_LBRACKET) {
                advance();
                parse_expression();
                expect(TK_RBRACKET, "Se esperaba ']' en asignación de arreglo");
                if (panic_mode) {
                    synchronize();
                    return;
                }
            }
            expect(TK_EQ, "Se esperaba '=' en asignación");
            if (panic_mode) {
                synchronize();
                return;
            }
            parse_expression();
            if (panic_mode) {
                synchronize();
                return;
            }
            skip_newlines();
            return;
        }
    }

    // Return
    if (tk->tipo == TK_RETURN) {
        advance();
        parse_expression();
        if (panic_mode) {
            synchronize();
            return;
        }
        skip_newlines();
        return;
    }

    // If
    if (tk->tipo == TK_IF) {
        advance();
        parse_expression();
        if (panic_mode) {
            synchronize();
            return;
        }
        skip_newlines();

        parse_statements_until(TK_END, TK_ELSE, true);
        return;
    }

    // While
    if (tk->tipo == TK_WHILE) {
        advance();
        parse_expression();
        if (panic_mode) {
            synchronize();
            return;
        }
        skip_newlines();

        parse_statements_until(TK_LOOP, TK_LOOP, false);
        return;
    }

    // Expression statement
    if (is_statement_start(tk)) {
        parse_expression();
        if (panic_mode) {
            synchronize();
            return;
        }
        skip_newlines();
        return;
    }

    error_at_token(tk, "Sentencia no reconocida");
    synchronize();
}

static void parse_statements_until(TipoToken terminator1, TipoToken terminator2, bool allow_else) {
    skip_newlines();

    while (cur_token()) {
        Token *tk = cur_token();
        
        // Encontramos un terminador
        if (tk->tipo == terminator1 || tk->tipo == terminator2) break;

        parse_statement();
        
        // Si hubo error, buscar el terminador del bloque
        if (panic_mode) {
            while (cur_token()) {
                Token *t = cur_token();
                if (t->tipo == terminator1 || t->tipo == terminator2) {
                    panic_mode = false;
                    break;
                }
                advance();
            }
            break;
        }
    }

    // Manejar else si está permitido
    if (cur_token() && cur_token()->tipo == terminator2 && allow_else) {
        advance();
        skip_newlines();

        while (cur_token() && cur_token()->tipo != terminator1) {
            parse_statement();
            
            if (panic_mode) {
                while (cur_token() && cur_token()->tipo != terminator1) {
                    advance();
                }
                panic_mode = false;
                break;
            }
        }
    }

    // Consumir el terminador final
    if (cur_token() && cur_token()->tipo == terminator1) {
        advance();
        skip_newlines();
        return;
    }

    error_at_token(cur_token(), "Bloque no terminado correctamente");
}

// ------------------------------------------------------------
// Functions
// ------------------------------------------------------------

static void parse_function() {
    advance();

    if (!cur_token() || cur_token()->tipo != TK_ID) {
        error_at_token(cur_token(), "Se esperaba nombre de función");
        synchronize();
        return;
    }
    advance();

    expect(TK_LPAREN, "Se esperaba '(' en función");
    if (panic_mode) {
        synchronize();
        return;
    }

    // Parsear parámetros
    if (cur_token() && cur_token()->tipo != TK_RPAREN) {
        if (cur_token()->tipo != TK_ID) {
            error_at_token(cur_token(), "Se esperaba identificador en parámetro");
            synchronize();
            return;
        }
        advance();
        expect(TK_COLON, "Se esperaba ':' en parámetro");
        if (panic_mode) {
            synchronize();
            return;
        }
        parse_type();
        if (panic_mode) {
            synchronize();
            return;
        }

        while (cur_token() && cur_token()->tipo == TK_COMMA) {
            advance();
            if (!cur_token() || cur_token()->tipo != TK_ID) {
                error_at_token(cur_token(), "Se esperaba identificador en parámetro");
                synchronize();
                return;
            }
            advance();
            expect(TK_COLON, "Se esperaba ':' en parámetro");
            if (panic_mode) {
                synchronize();
                return;
            }
            parse_type();
            if (panic_mode) {
                synchronize();
                return;
            }
        }
    }

    expect(TK_RPAREN, "Se esperaba ')' en parámetros de función");
    if (panic_mode) {
        synchronize();
        return;
    }

    expect(TK_COLON, "Se esperaba ':' antes del tipo de retorno");
    if (panic_mode) {
        synchronize();
        return;
    }
    
    parse_type();
    if (panic_mode) {
        synchronize();
        return;
    }
    
    skip_newlines();

    parse_statements_until(TK_END, TK_END, false);
}

static void parse_declaration_or_function() {
    skip_newlines();
    Token *tk = cur_token();
    if (!tk) return;

    if (tk->tipo == TK_FUN) {
        parse_function();
        return;
    }

    // Declaración global
    if (tk->tipo == TK_ID && peek_token(1) && peek_token(1)->tipo == TK_COLON) {
        advance();
        advance();
        parse_type();
        if (panic_mode) {
            synchronize();
            return;
        }
        skip_newlines();
        return;
    }

    error_at_token(tk, "Se esperaba 'fun' o declaración global");
    synchronize();
}

// ------------------------------------------------------------
// Program
// ------------------------------------------------------------

static void parse_program() {
    skip_newlines();
    while (cur_token()) {
        parse_declaration_or_function();
        
        // Si hubo error, intentar recuperarse
        if (panic_mode) {
            synchronize();
        }
    }
}

// ------------------------------------------------------------
// Public API
// ------------------------------------------------------------

int analizar_sintactico(const char *ruta_archivo) {
    int lex_err = analizar_lexico(ruta_archivo);
    if (lex_err != 0) return lex_err;

    pos = 0;
    had_error = false;
    panic_mode = false;

    parse_program();

    if (had_error) {
        fprintf(stderr, "\nAnalisis sintactico fallido. Se detectaron errores.\n");
        return 2;
    }

    printf("Analisis sintactico completado\n");
    return 0;
}

#ifdef BUILD_WITH_PARSER_MAIN
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_mini0>\n", argv[0]);
        return 1;
    }
    return analizar_sintactico(argv[1]);
}
#endif
