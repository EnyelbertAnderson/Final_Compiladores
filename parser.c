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

/* 'match' removed (unused) — parser uses explicit checks + advance() where needed */

static void error_at_token(Token *tk, const char *msg) {
    if (!tk) {
        fprintf(stderr, "Error sintáctico en EOF: %s\n", msg);
    } else {
        fprintf(stderr, "Error sintáctico en línea %d: %s (token: %s '%s')\n",
                tk->linea, msg, tipo_token_str(tk->tipo), tk->lexema);
    }
    had_error = true;
}

static void expect(TipoToken t, const char *msg) {
    Token *tk = cur_token();
    if (!tk || tk->tipo != t) {
        error_at_token(tk, msg);
        return;
    }
    advance();
}

// Utilities
static void skip_newlines() {
    while (cur_token() && cur_token()->tipo == TK_NL) advance();
}

// Forward declarations for expressions and statements
static void parse_program();
static void parse_declaration_or_function();
static void parse_statements_until(TipoToken terminator1, TipoToken terminator2, bool allow_else);

// ---- Types ----
static void parse_type() {
    // Accept zero or more [] prefixes
    while (cur_token() && cur_token()->tipo == TK_LBRACKET) {
        // expect RBRACKET after LBRACKET
        advance();
        if (!cur_token() || cur_token()->tipo != TK_RBRACKET) {
            error_at_token(cur_token(), "Se esperaba ']' en la declaración de tipo de arreglo");
            return;
        }
        advance();
    }

    // Basic types
    if (!cur_token()) { error_at_token(NULL, "Se esperaba un tipo (int, bool, string, char)"); return; }
    if (cur_token()->tipo == TK_INT || cur_token()->tipo == TK_BOOL || cur_token()->tipo == TK_STRING || cur_token()->tipo == TK_CHAR) {
        advance();
        return;
    }

    error_at_token(cur_token(), "Tipo desconocido (se esperaba int, bool, string o char)");
}

// ---- Expressions (precedence climbing via recursive descent functions) ----
static void parse_expression();

static void parse_primary() {
    Token *tk = cur_token();
    if (!tk) {
        error_at_token(NULL, "Se esperaba una expresión, se encontró EOF");
        return;
    }

    if (tk->tipo == TK_LITNUMERAL || tk->tipo == TK_LITSTRING || tk->tipo == TK_TRUE || tk->tipo == TK_FALSE) {
        advance();
        return;
    }

    if (tk->tipo == TK_ID) {
        // Could be: ID, ID(args), ID[expr] (array access), or combination like func(...)[...]
        advance();
        // handle chained calls/indexing
        while (cur_token()) {
            if (cur_token()->tipo == TK_LPAREN) {
                // function call
                advance();
                // args optional
                if (cur_token() && cur_token()->tipo != TK_RPAREN) {
                    parse_expression();
                    while (cur_token() && cur_token()->tipo == TK_COMMA) {
                        advance();
                        parse_expression();
                    }
                }
                expect(TK_RPAREN, "Se esperaba ')' en llamada de función");
            } else if (cur_token()->tipo == TK_LBRACKET) {
                // array index
                advance();
                parse_expression();
                expect(TK_RBRACKET, "Se esperaba ']' en acceso a arreglo");
            } else break;
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
    while (cur_token() && (cur_token()->tipo == TK_GT || cur_token()->tipo == TK_LT || cur_token()->tipo == TK_GE || cur_token()->tipo == TK_LE)) {
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

// ---- Statements ----
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

static void parse_statement() {
    Token *tk = cur_token();
    if (!tk) return;

    // Variable declaration: ID : TYPE
    if (tk->tipo == TK_ID && peek_token(1) && peek_token(1)->tipo == TK_COLON) {
        advance(); // id
        advance(); // ':'
        parse_type();
        skip_newlines();
        return;
    }

    // Assignment: ID (= or [expr] = )
    if (tk->tipo == TK_ID) {
        // Save position for a fallback
        Token *next = peek_token(1);
        if (next && (next->tipo == TK_EQ || next->tipo == TK_LBRACKET)) {
            advance(); // ID
            if (cur_token() && cur_token()->tipo == TK_LBRACKET) {
                // index
                advance();
                parse_expression();
                expect(TK_RBRACKET, "Se esperaba ']' en la asignación a arreglo");
            }
            expect(TK_EQ, "Se esperaba '=' en la asignación");
            parse_expression();
            skip_newlines();
            return;
        }
        // else, fallthrough -> expression statement
    }

    // return
    if (tk->tipo == TK_RETURN) {
        advance();
        parse_expression();
        skip_newlines();
        return;
    }

    // if
    if (tk->tipo == TK_IF) {
        advance();
        parse_expression();
        skip_newlines();

        // parse statements until ELSE or END
        parse_statements_until(TK_END, TK_ELSE, true);
        return;
    }

    // while ... loop
    if (tk->tipo == TK_WHILE) {
        advance();
        parse_expression();
        skip_newlines();
        // body until LOOP
        parse_statements_until(TK_LOOP, TK_LOOP, false);
        return;
    }

    // expression statement
    if (is_statement_start(tk)) {
        parse_expression();
        skip_newlines();
        return;
    }

    // If not recognized, error
    error_at_token(tk, "Sentencia no reconocida");
    // Attempt to recover by advancing
    advance();
}

static void parse_statements_until(TipoToken terminator1, TipoToken terminator2, bool allow_else) {
    skip_newlines();
    while (cur_token()) {
        Token *tk = cur_token();
        if (tk->tipo == terminator1 || tk->tipo == terminator2) break;
        // For functions, declarations and statements
        parse_statement();
        if (had_error) return;
    }

    if (cur_token() && cur_token()->tipo == terminator2 && allow_else) {
        // consume else
        advance();
        skip_newlines();
        while (cur_token() && cur_token()->tipo != terminator1) {
            parse_statement();
            if (had_error) return;
        }
    }

    if (cur_token() && cur_token()->tipo == terminator1) {
        // consume end/loop
        advance();
        skip_newlines();
        return;
    }

    // Missing terminator
    error_at_token(cur_token(), "Bloque no terminado correctamente (se esperaba 'end' o 'loop')");
}

// Function declaration
static void parse_function() {
    // We arrive having seen 'fun'
    advance(); // consume fun

    // Expect identifier
    if (!cur_token() || cur_token()->tipo != TK_ID) {
        error_at_token(cur_token(), "Se esperaba nombre de la función");
        return;
    }
    advance(); // id

    expect(TK_LPAREN, "Se esperaba '(' en la declaración de la función");
    // params optional
    if (cur_token() && cur_token()->tipo != TK_RPAREN) {
        // param: ID : TYPE
        if (cur_token()->tipo != TK_ID) {
            error_at_token(cur_token(), "Se esperaba identificador en la lista de parámetros");
        } else {
            advance(); expect(TK_COLON, "Se esperaba ':' en parámetro"); parse_type();
        }
        while (cur_token() && cur_token()->tipo == TK_COMMA) {
            advance();
            if (!cur_token() || cur_token()->tipo != TK_ID) { error_at_token(cur_token(), "Se esperaba identificador en lista de parámetros"); return; }
            advance(); expect(TK_COLON, "Se esperaba ':' en parámetro"); parse_type();
        }
    }
    expect(TK_RPAREN, "Se esperaba ')' después de lista de parámetros");

    expect(TK_COLON, "Se esperaba ':' antes del tipo de retorno de la función");
    parse_type();
    skip_newlines();

    // Function body: series of statements until 'end'
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

    if (tk->tipo == TK_ID && peek_token(1) && peek_token(1)->tipo == TK_COLON) {
        // global var decl
        advance(); // id
        advance(); // ':'
        parse_type();
        skip_newlines();
        return;
    }

    error_at_token(tk, "Se esperaba 'fun' o declaración global (id : tipo)");
    advance();
}

static void parse_program() {
    skip_newlines();
    while (cur_token()) {
        parse_declaration_or_function();
        if (had_error) return;
    }
}

// ---- Public entry ----
int analizar_sintactico(const char *ruta_archivo) {
    // Run lexer first
    int lex_err = analizar_lexico(ruta_archivo);
    if (lex_err != 0) return lex_err;

    // Reset parser state
    pos = 0;
    had_error = false;

    // Run parser
    parse_program();

    if (had_error) {
        // print summary
        fprintf(stderr, "\nAnalisis sintactico fallido. Se detectaron errores.\n");
        return 2;
    }

    printf("Analisis sintactico completado\n");
    return 0;
}

// If compiled as standalone, provide a main
#ifdef BUILD_WITH_PARSER_MAIN
int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s <archivo_mini0>\n", argv[0]);
        return 1;
    }
    int ret = analizar_sintactico(argv[1]);
    return ret;
}
#endif
