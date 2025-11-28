#ifndef LL1_TABLE_H
#define LL1_TABLE_H

#include <stdio.h>

// Definición de no-terminales
typedef enum {
    NT_PROGRAM,
    NT_DECL_OR_FUN_LIST,
    NT_DECL_OR_FUN,
    NT_PARAM_LIST,
    NT_PARAM_LIST_PRIME,
    NT_PARAM,
    NT_TYPE,
    NT_ARRAY_PREFIX,
    NT_BASE_TYPE,
    NT_STMT_LIST,
    NT_STATEMENT,
    NT_ASSIGN_OR_CALL,
    NT_ELSE_PART,
    NT_ARG_LIST,
    NT_ARG_LIST_PRIME,
    NT_INDEX_CHAIN,
    NT_EXPR,
    NT_OR_EXPR,
    NT_OR_EXPR_PRIME,
    NT_AND_EXPR,
    NT_AND_EXPR_PRIME,
    NT_EQUALITY_EXPR,
    NT_EQUALITY_EXPR_PRIME,
    NT_RELATIONAL_EXPR,
    NT_RELATIONAL_EXPR_PRIME,
    NT_ADDITIVE_EXPR,
    NT_ADDITIVE_EXPR_PRIME,
    NT_MULTIPLICATIVE_EXPR,
    NT_MULTIPLICATIVE_EXPR_PRIME,
    NT_UNARY_EXPR,
    NT_PRIMARY_EXPR,
    NT_POSTFIX_CHAIN,
    NT_COUNT
} NonTerminal;

// Definición de terminales (tokens)
typedef enum {
    T_FUN,
    T_ID,
    T_INT,
    T_BOOL,
    T_STRING,
    T_CHAR,
    T_IF,
    T_ELSE,
    T_WHILE,
    T_LOOP,
    T_RETURN,
    T_END,
    T_LPAREN,       // (
    T_RPAREN,       // )
    T_LBRACKET,     // [
    T_RBRACKET,     // ]
    T_COLON,        // :
    T_COMMA,        // ,
    T_EQ,           // =
    T_NEQ,          // <>
    T_LT,           // <
    T_GT,           // >
    T_LTE,          // <=
    T_GTE,          // >=
    T_PLUS,         // +
    T_MINUS,        // -
    T_STAR,         // *
    T_SLASH,        // /
    T_AND,
    T_OR,
    T_NOT,
    T_TRUE,
    T_FALSE,
    T_LITNUMERAL,
    T_LITSTRING,
    T_NL,
    T_EOF,
    T_COUNT
} Terminal;

// Definición de producciones
typedef enum {
    PROD_NONE = 0,
    PROD_1,   // program -> decl-or-fun-list
    PROD_2,   // decl-or-fun-list -> decl-or-fun decl-or-fun-list
    PROD_3,   // decl-or-fun-list -> epsilon
    PROD_4,   // decl-or-fun -> FUN ID ( param-list ) : type NL* stmt-list END NL*
    PROD_5,   // decl-or-fun -> ID : type NL*
    PROD_6,   // param-list -> param param-list'
    PROD_7,   // param-list -> epsilon
    PROD_8,   // param-list' -> , param param-list'
    PROD_9,   // param-list' -> epsilon
    PROD_10,  // param -> ID : type
    PROD_11,  // type -> array-prefix base-type
    PROD_12,  // array-prefix -> [ ] array-prefix
    PROD_13,  // array-prefix -> epsilon
    PROD_14,  // base-type -> INT | BOOL | STRING | CHAR
    PROD_15,  // stmt-list -> statement stmt-list
    PROD_16,  // stmt-list -> epsilon
    PROD_17,  // statement -> ID : type NL*
    PROD_18,  // statement -> ID assign-or-call NL*
    PROD_19,  // statement -> RETURN expr NL*
    PROD_20,  // statement -> IF expr NL* stmt-list else-part END NL*
    PROD_21,  // statement -> WHILE expr NL* stmt-list LOOP NL*
    PROD_22,  // statement -> expr NL*
    PROD_23,  // assign-or-call -> = expr
    PROD_24,  // assign-or-call -> [ expr ] = expr
    PROD_25,  // assign-or-call -> ( arg-list ) index-chain
    PROD_26,  // else-part -> ELSE NL* stmt-list
    PROD_27,  // else-part -> epsilon
    PROD_28,  // arg-list -> expr arg-list'
    PROD_29,  // arg-list -> epsilon
    PROD_30,  // arg-list' -> , expr arg-list'
    PROD_31,  // arg-list' -> epsilon
    PROD_32,  // index-chain -> [ expr ] index-chain
    PROD_33,  // index-chain -> epsilon
    PROD_34,  // expr -> or-expr
    PROD_35,  // or-expr -> and-expr or-expr'
    PROD_36,  // or-expr' -> OR and-expr or-expr'
    PROD_37,  // or-expr' -> epsilon
    PROD_38,  // and-expr -> equality-expr and-expr'
    PROD_39,  // and-expr' -> AND equality-expr and-expr'
    PROD_40,  // and-expr' -> epsilon
    PROD_41,  // equality-expr -> relational-expr equality-expr'
    PROD_42,  // equality-expr' -> = relational-expr equality-expr'
    PROD_43,  // equality-expr' -> <> relational-expr equality-expr'
    PROD_44,  // equality-expr' -> epsilon
    PROD_45,  // relational-expr -> additive-expr relational-expr'
    PROD_46,  // relational-expr' -> > additive-expr relational-expr'
    PROD_47,  // relational-expr' -> < additive-expr relational-expr'
    PROD_48,  // relational-expr' -> >= additive-expr relational-expr'
    PROD_49,  // relational-expr' -> <= additive-expr relational-expr'
    PROD_50,  // relational-expr' -> epsilon
    PROD_51,  // additive-expr -> multiplicative-expr additive-expr'
    PROD_52,  // additive-expr' -> + multiplicative-expr additive-expr'
    PROD_53,  // additive-expr' -> - multiplicative-expr additive-expr'
    PROD_54,  // additive-expr' -> epsilon
    PROD_55,  // multiplicative-expr -> unary-expr multiplicative-expr'
    PROD_56,  // multiplicative-expr' -> * unary-expr multiplicative-expr'
    PROD_57,  // multiplicative-expr' -> / unary-expr multiplicative-expr'
    PROD_58,  // multiplicative-expr' -> epsilon
    PROD_59,  // unary-expr -> - unary-expr
    PROD_60,  // unary-expr -> NOT unary-expr
    PROD_61,  // unary-expr -> primary-expr
    PROD_62,  // primary-expr -> LITNUMERAL
    PROD_63,  // primary-expr -> LITSTRING
    PROD_64,  // primary-expr -> TRUE
    PROD_65,  // primary-expr -> FALSE
    PROD_66,  // primary-expr -> ID postfix-chain
    PROD_67,  // primary-expr -> ( expr )
    PROD_68,  // postfix-chain -> ( arg-list ) postfix-chain
    PROD_69,  // postfix-chain -> [ expr ] postfix-chain
    PROD_70,  // postfix-chain -> epsilon
} Production;

// Tabla LL(1): [NonTerminal][Terminal] -> Production
extern Production ll1_table[NT_COUNT][T_COUNT];

// Funciones auxiliares
const char* get_nonterminal_name(NonTerminal nt);
const char* get_terminal_name(Terminal t);
const char* get_production_string(Production prod);
void print_ll1_table();
Production lookup_ll1(NonTerminal nt, Terminal t);

#endif
