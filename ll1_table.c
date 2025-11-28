#include "ll1_table.h"
#include <string.h>

// Tabla LL(1) inicializada con PROD_NONE
Production ll1_table[NT_COUNT][T_COUNT];

// Nombres de no-terminales
static const char* nonterminal_names[] = {
    "program",
    "decl-or-fun-list",
    "decl-or-fun",
    "param-list",
    "param-list'",
    "param",
    "type",
    "array-prefix",
    "base-type",
    "stmt-list",
    "statement",
    "assign-or-call",
    "else-part",
    "arg-list",
    "arg-list'",
    "index-chain",
    "expr",
    "or-expr",
    "or-expr'",
    "and-expr",
    "and-expr'",
    "equality-expr",
    "equality-expr'",
    "relational-expr",
    "relational-expr'",
    "additive-expr",
    "additive-expr'",
    "multiplicative-expr",
    "multiplicative-expr'",
    "unary-expr",
    "primary-expr",
    "postfix-chain"
};

// Nombres de terminales
static const char* terminal_names[] = {
    "FUN", "ID", "INT", "BOOL", "STRING", "CHAR",
    "IF", "ELSE", "WHILE", "LOOP", "RETURN", "END",
    "(", ")", "[", "]", ":", ",", "=", "<>",
    "<", ">", "<=", ">=", "+", "-", "*", "/",
    "AND", "OR", "NOT", "TRUE", "FALSE",
    "LITNUMERAL", "LITSTRING", "NL", "$"
};

// Inicializar la tabla LL(1)
void init_ll1_table() {
    // Inicializar toda la tabla con PROD_NONE
    for (int i = 0; i < NT_COUNT; i++) {
        for (int j = 0; j < T_COUNT; j++) {
            ll1_table[i][j] = PROD_NONE;
        }
    }
    
    // Llenar la tabla según la gramática
    
    // program -> decl-or-fun-list
    ll1_table[NT_PROGRAM][T_FUN] = PROD_1;
    ll1_table[NT_PROGRAM][T_ID] = PROD_1;
    ll1_table[NT_PROGRAM][T_EOF] = PROD_1;
    
    // decl-or-fun-list -> decl-or-fun decl-or-fun-list
    ll1_table[NT_DECL_OR_FUN_LIST][T_FUN] = PROD_2;
    ll1_table[NT_DECL_OR_FUN_LIST][T_ID] = PROD_2;
    // decl-or-fun-list -> epsilon
    ll1_table[NT_DECL_OR_FUN_LIST][T_EOF] = PROD_3;
    
    // decl-or-fun -> FUN ...
    ll1_table[NT_DECL_OR_FUN][T_FUN] = PROD_4;
    // decl-or-fun -> ID : type
    ll1_table[NT_DECL_OR_FUN][T_ID] = PROD_5;
    
    // param-list -> param param-list'
    ll1_table[NT_PARAM_LIST][T_ID] = PROD_6;
    // param-list -> epsilon
    ll1_table[NT_PARAM_LIST][T_RPAREN] = PROD_7;
    
    // param-list' -> , param param-list'
    ll1_table[NT_PARAM_LIST_PRIME][T_COMMA] = PROD_8;
    // param-list' -> epsilon
    ll1_table[NT_PARAM_LIST_PRIME][T_RPAREN] = PROD_9;
    
    // param -> ID : type
    ll1_table[NT_PARAM][T_ID] = PROD_10;
    
    // type -> array-prefix base-type
    ll1_table[NT_TYPE][T_LBRACKET] = PROD_11;
    ll1_table[NT_TYPE][T_INT] = PROD_11;
    ll1_table[NT_TYPE][T_BOOL] = PROD_11;
    ll1_table[NT_TYPE][T_STRING] = PROD_11;
    ll1_table[NT_TYPE][T_CHAR] = PROD_11;
    
    // array-prefix -> [ ] array-prefix
    ll1_table[NT_ARRAY_PREFIX][T_LBRACKET] = PROD_12;
    // array-prefix -> epsilon
    ll1_table[NT_ARRAY_PREFIX][T_INT] = PROD_13;
    ll1_table[NT_ARRAY_PREFIX][T_BOOL] = PROD_13;
    ll1_table[NT_ARRAY_PREFIX][T_STRING] = PROD_13;
    ll1_table[NT_ARRAY_PREFIX][T_CHAR] = PROD_13;
    
    // base-type -> INT | BOOL | STRING | CHAR
    ll1_table[NT_BASE_TYPE][T_INT] = PROD_14;
    ll1_table[NT_BASE_TYPE][T_BOOL] = PROD_14;
    ll1_table[NT_BASE_TYPE][T_STRING] = PROD_14;
    ll1_table[NT_BASE_TYPE][T_CHAR] = PROD_14;
    
    // stmt-list -> statement stmt-list
    ll1_table[NT_STMT_LIST][T_ID] = PROD_15;
    ll1_table[NT_STMT_LIST][T_RETURN] = PROD_15;
    ll1_table[NT_STMT_LIST][T_IF] = PROD_15;
    ll1_table[NT_STMT_LIST][T_WHILE] = PROD_15;
    ll1_table[NT_STMT_LIST][T_LPAREN] = PROD_15;
    ll1_table[NT_STMT_LIST][T_LITNUMERAL] = PROD_15;
    ll1_table[NT_STMT_LIST][T_LITSTRING] = PROD_15;
    ll1_table[NT_STMT_LIST][T_MINUS] = PROD_15;
    ll1_table[NT_STMT_LIST][T_NOT] = PROD_15;
    ll1_table[NT_STMT_LIST][T_TRUE] = PROD_15;
    ll1_table[NT_STMT_LIST][T_FALSE] = PROD_15;
    // stmt-list -> epsilon
    ll1_table[NT_STMT_LIST][T_END] = PROD_16;
    ll1_table[NT_STMT_LIST][T_ELSE] = PROD_16;
    ll1_table[NT_STMT_LIST][T_LOOP] = PROD_16;
    
    // statement -> ID : type (declaración) o ID assign-or-call (asignación/llamada)
    // Nota: Requiere lookahead en la práctica
    ll1_table[NT_STATEMENT][T_ID] = PROD_17; // Por defecto, se maneja con lookahead
    ll1_table[NT_STATEMENT][T_RETURN] = PROD_19;
    ll1_table[NT_STATEMENT][T_IF] = PROD_20;
    ll1_table[NT_STATEMENT][T_WHILE] = PROD_21;
    ll1_table[NT_STATEMENT][T_LPAREN] = PROD_22;
    ll1_table[NT_STATEMENT][T_LITNUMERAL] = PROD_22;
    ll1_table[NT_STATEMENT][T_LITSTRING] = PROD_22;
    ll1_table[NT_STATEMENT][T_MINUS] = PROD_22;
    ll1_table[NT_STATEMENT][T_NOT] = PROD_22;
    ll1_table[NT_STATEMENT][T_TRUE] = PROD_22;
    ll1_table[NT_STATEMENT][T_FALSE] = PROD_22;
    
    // assign-or-call -> = expr
    ll1_table[NT_ASSIGN_OR_CALL][T_EQ] = PROD_23;
    // assign-or-call -> [ expr ] = expr
    ll1_table[NT_ASSIGN_OR_CALL][T_LBRACKET] = PROD_24;
    // assign-or-call -> ( arg-list ) index-chain
    ll1_table[NT_ASSIGN_OR_CALL][T_LPAREN] = PROD_25;
    
    // else-part -> ELSE NL* stmt-list
    ll1_table[NT_ELSE_PART][T_ELSE] = PROD_26;
    // else-part -> epsilon
    ll1_table[NT_ELSE_PART][T_END] = PROD_27;
    
    // arg-list -> expr arg-list'
    ll1_table[NT_ARG_LIST][T_ID] = PROD_28;
    ll1_table[NT_ARG_LIST][T_LPAREN] = PROD_28;
    ll1_table[NT_ARG_LIST][T_LITNUMERAL] = PROD_28;
    ll1_table[NT_ARG_LIST][T_LITSTRING] = PROD_28;
    ll1_table[NT_ARG_LIST][T_MINUS] = PROD_28;
    ll1_table[NT_ARG_LIST][T_NOT] = PROD_28;
    ll1_table[NT_ARG_LIST][T_TRUE] = PROD_28;
    ll1_table[NT_ARG_LIST][T_FALSE] = PROD_28;
    // arg-list -> epsilon
    ll1_table[NT_ARG_LIST][T_RPAREN] = PROD_29;
    
    // arg-list' -> , expr arg-list'
    ll1_table[NT_ARG_LIST_PRIME][T_COMMA] = PROD_30;
    // arg-list' -> epsilon
    ll1_table[NT_ARG_LIST_PRIME][T_RPAREN] = PROD_31;
    
    // index-chain -> [ expr ] index-chain
    ll1_table[NT_INDEX_CHAIN][T_LBRACKET] = PROD_32;
    // index-chain -> epsilon (en FOLLOW)
    ll1_table[NT_INDEX_CHAIN][T_NL] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_COMMA] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_RPAREN] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_RBRACKET] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_AND] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_OR] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_EQ] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_NEQ] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_LT] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_GT] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_LTE] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_GTE] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_PLUS] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_MINUS] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_STAR] = PROD_33;
    ll1_table[NT_INDEX_CHAIN][T_SLASH] = PROD_33;
    
    // expr -> or-expr
    ll1_table[NT_EXPR][T_ID] = PROD_34;
    ll1_table[NT_EXPR][T_LPAREN] = PROD_34;
    ll1_table[NT_EXPR][T_LITNUMERAL] = PROD_34;
    ll1_table[NT_EXPR][T_LITSTRING] = PROD_34;
    ll1_table[NT_EXPR][T_MINUS] = PROD_34;
    ll1_table[NT_EXPR][T_NOT] = PROD_34;
    ll1_table[NT_EXPR][T_TRUE] = PROD_34;
    ll1_table[NT_EXPR][T_FALSE] = PROD_34;
    
    // or-expr -> and-expr or-expr'
    ll1_table[NT_OR_EXPR][T_ID] = PROD_35;
    ll1_table[NT_OR_EXPR][T_LPAREN] = PROD_35;
    ll1_table[NT_OR_EXPR][T_LITNUMERAL] = PROD_35;
    ll1_table[NT_OR_EXPR][T_LITSTRING] = PROD_35;
    ll1_table[NT_OR_EXPR][T_MINUS] = PROD_35;
    ll1_table[NT_OR_EXPR][T_NOT] = PROD_35;
    ll1_table[NT_OR_EXPR][T_TRUE] = PROD_35;
    ll1_table[NT_OR_EXPR][T_FALSE] = PROD_35;
    
    // or-expr' -> OR and-expr or-expr'
    ll1_table[NT_OR_EXPR_PRIME][T_OR] = PROD_36;
    // or-expr' -> epsilon
    ll1_table[NT_OR_EXPR_PRIME][T_NL] = PROD_37;
    ll1_table[NT_OR_EXPR_PRIME][T_COMMA] = PROD_37;
    ll1_table[NT_OR_EXPR_PRIME][T_RPAREN] = PROD_37;
    ll1_table[NT_OR_EXPR_PRIME][T_RBRACKET] = PROD_37;
    
    // and-expr -> equality-expr and-expr'
    ll1_table[NT_AND_EXPR][T_ID] = PROD_38;
    ll1_table[NT_AND_EXPR][T_LPAREN] = PROD_38;
    ll1_table[NT_AND_EXPR][T_LITNUMERAL] = PROD_38;
    ll1_table[NT_AND_EXPR][T_LITSTRING] = PROD_38;
    ll1_table[NT_AND_EXPR][T_MINUS] = PROD_38;
    ll1_table[NT_AND_EXPR][T_NOT] = PROD_38;
    ll1_table[NT_AND_EXPR][T_TRUE] = PROD_38;
    ll1_table[NT_AND_EXPR][T_FALSE] = PROD_38;
    
    // and-expr' -> AND equality-expr and-expr'
    ll1_table[NT_AND_EXPR_PRIME][T_AND] = PROD_39;
    // and-expr' -> epsilon
    ll1_table[NT_AND_EXPR_PRIME][T_OR] = PROD_40;
    ll1_table[NT_AND_EXPR_PRIME][T_NL] = PROD_40;
    ll1_table[NT_AND_EXPR_PRIME][T_COMMA] = PROD_40;
    ll1_table[NT_AND_EXPR_PRIME][T_RPAREN] = PROD_40;
    ll1_table[NT_AND_EXPR_PRIME][T_RBRACKET] = PROD_40;
    
    // equality-expr -> relational-expr equality-expr'
    ll1_table[NT_EQUALITY_EXPR][T_ID] = PROD_41;
    ll1_table[NT_EQUALITY_EXPR][T_LPAREN] = PROD_41;
    ll1_table[NT_EQUALITY_EXPR][T_LITNUMERAL] = PROD_41;
    ll1_table[NT_EQUALITY_EXPR][T_LITSTRING] = PROD_41;
    ll1_table[NT_EQUALITY_EXPR][T_MINUS] = PROD_41;
    ll1_table[NT_EQUALITY_EXPR][T_NOT] = PROD_41;
    ll1_table[NT_EQUALITY_EXPR][T_TRUE] = PROD_41;
    ll1_table[NT_EQUALITY_EXPR][T_FALSE] = PROD_41;
    
    // equality-expr' -> = relational-expr equality-expr'
    ll1_table[NT_EQUALITY_EXPR_PRIME][T_EQ] = PROD_42;
    // equality-expr' -> <> relational-expr equality-expr'
    ll1_table[NT_EQUALITY_EXPR_PRIME][T_NEQ] = PROD_43;
    // equality-expr' -> epsilon
    ll1_table[NT_EQUALITY_EXPR_PRIME][T_AND] = PROD_44;
    ll1_table[NT_EQUALITY_EXPR_PRIME][T_OR] = PROD_44;
    ll1_table[NT_EQUALITY_EXPR_PRIME][T_NL] = PROD_44;
    ll1_table[NT_EQUALITY_EXPR_PRIME][T_COMMA] = PROD_44;
    ll1_table[NT_EQUALITY_EXPR_PRIME][T_RPAREN] = PROD_44;
    ll1_table[NT_EQUALITY_EXPR_PRIME][T_RBRACKET] = PROD_44;
    
    // relational-expr -> additive-expr relational-expr'
    ll1_table[NT_RELATIONAL_EXPR][T_ID] = PROD_45;
    ll1_table[NT_RELATIONAL_EXPR][T_LPAREN] = PROD_45;
    ll1_table[NT_RELATIONAL_EXPR][T_LITNUMERAL] = PROD_45;
    ll1_table[NT_RELATIONAL_EXPR][T_LITSTRING] = PROD_45;
    ll1_table[NT_RELATIONAL_EXPR][T_MINUS] = PROD_45;
    ll1_table[NT_RELATIONAL_EXPR][T_NOT] = PROD_45;
    ll1_table[NT_RELATIONAL_EXPR][T_TRUE] = PROD_45;
    ll1_table[NT_RELATIONAL_EXPR][T_FALSE] = PROD_45;
    
    // relational-expr' -> > additive-expr relational-expr'
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_GT] = PROD_46;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_LT] = PROD_47;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_GTE] = PROD_48;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_LTE] = PROD_49;
    // relational-expr' -> epsilon
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_EQ] = PROD_50;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_NEQ] = PROD_50;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_AND] = PROD_50;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_OR] = PROD_50;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_NL] = PROD_50;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_COMMA] = PROD_50;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_RPAREN] = PROD_50;
    ll1_table[NT_RELATIONAL_EXPR_PRIME][T_RBRACKET] = PROD_50;
    
    // additive-expr -> multiplicative-expr additive-expr'
    ll1_table[NT_ADDITIVE_EXPR][T_ID] = PROD_51;
    ll1_table[NT_ADDITIVE_EXPR][T_LPAREN] = PROD_51;
    ll1_table[NT_ADDITIVE_EXPR][T_LITNUMERAL] = PROD_51;
    ll1_table[NT_ADDITIVE_EXPR][T_LITSTRING] = PROD_51;
    ll1_table[NT_ADDITIVE_EXPR][T_MINUS] = PROD_51;
    ll1_table[NT_ADDITIVE_EXPR][T_NOT] = PROD_51;
    ll1_table[NT_ADDITIVE_EXPR][T_TRUE] = PROD_51;
    ll1_table[NT_ADDITIVE_EXPR][T_FALSE] = PROD_51;
    
    // additive-expr' -> + multiplicative-expr additive-expr'
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_PLUS] = PROD_52;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_MINUS] = PROD_53;
    // additive-expr' -> epsilon
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_GT] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_LT] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_GTE] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_LTE] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_EQ] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_NEQ] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_AND] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_OR] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_NL] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_COMMA] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_RPAREN] = PROD_54;
    ll1_table[NT_ADDITIVE_EXPR_PRIME][T_RBRACKET] = PROD_54;
    
    // multiplicative-expr -> unary-expr multiplicative-expr'
    ll1_table[NT_MULTIPLICATIVE_EXPR][T_ID] = PROD_55;
    ll1_table[NT_MULTIPLICATIVE_EXPR][T_LPAREN] = PROD_55;
    ll1_table[NT_MULTIPLICATIVE_EXPR][T_LITNUMERAL] = PROD_55;
    ll1_table[NT_MULTIPLICATIVE_EXPR][T_LITSTRING] = PROD_55;
    ll1_table[NT_MULTIPLICATIVE_EXPR][T_MINUS] = PROD_55;
    ll1_table[NT_MULTIPLICATIVE_EXPR][T_NOT] = PROD_55;
    ll1_table[NT_MULTIPLICATIVE_EXPR][T_TRUE] = PROD_55;
    ll1_table[NT_MULTIPLICATIVE_EXPR][T_FALSE] = PROD_55;
    
    // multiplicative-expr' -> * unary-expr multiplicative-expr'
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_STAR] = PROD_56;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_SLASH] = PROD_57;
    // multiplicative-expr' -> epsilon
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_PLUS] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_MINUS] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_GT] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_LT] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_GTE] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_LTE] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_EQ] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_NEQ] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_AND] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_OR] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_NL] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_COMMA] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_RPAREN] = PROD_58;
    ll1_table[NT_MULTIPLICATIVE_EXPR_PRIME][T_RBRACKET] = PROD_58;
    
    // unary-expr -> - unary-expr
    ll1_table[NT_UNARY_EXPR][T_MINUS] = PROD_59;
    // unary-expr -> NOT unary-expr
    ll1_table[NT_UNARY_EXPR][T_NOT] = PROD_60;
    // unary-expr -> primary-expr
    ll1_table[NT_UNARY_EXPR][T_ID] = PROD_61;
    ll1_table[NT_UNARY_EXPR][T_LPAREN] = PROD_61;
    ll1_table[NT_UNARY_EXPR][T_LITNUMERAL] = PROD_61;
    ll1_table[NT_UNARY_EXPR][T_LITSTRING] = PROD_61;
    ll1_table[NT_UNARY_EXPR][T_TRUE] = PROD_61;
    ll1_table[NT_UNARY_EXPR][T_FALSE] = PROD_61;
    
    // primary-expr -> LITNUMERAL
    ll1_table[NT_PRIMARY_EXPR][T_LITNUMERAL] = PROD_62;
    ll1_table[NT_PRIMARY_EXPR][T_LITSTRING] = PROD_63;
    ll1_table[NT_PRIMARY_EXPR][T_TRUE] = PROD_64;
    ll1_table[NT_PRIMARY_EXPR][T_FALSE] = PROD_65;
    // primary-expr -> ID postfix-chain
    ll1_table[NT_PRIMARY_EXPR][T_ID] = PROD_66;
    // primary-expr -> ( expr )
    ll1_table[NT_PRIMARY_EXPR][T_LPAREN] = PROD_67;
    
    // postfix-chain -> ( arg-list ) postfix-chain
    ll1_table[NT_POSTFIX_CHAIN][T_LPAREN] = PROD_68;
    // postfix-chain -> [ expr ] postfix-chain
    ll1_table[NT_POSTFIX_CHAIN][T_LBRACKET] = PROD_69;
    // postfix-chain -> epsilon
    ll1_table[NT_POSTFIX_CHAIN][T_STAR] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_SLASH] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_PLUS] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_MINUS] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_GT] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_LT] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_GTE] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_LTE] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_EQ] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_NEQ] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_AND] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_OR] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_NL] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_COMMA] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_RPAREN] = PROD_70;
    ll1_table[NT_POSTFIX_CHAIN][T_RBRACKET] = PROD_70;
}

const char* get_nonterminal_name(NonTerminal nt) {
    if (nt >= 0 && nt < NT_COUNT) {
        return nonterminal_names[nt];
    }
    return "UNKNOWN";
}

const char* get_terminal_name(Terminal t) {
    if (t >= 0 && t < T_COUNT) {
        return terminal_names[t];
    }
    return "UNKNOWN";
}

const char* get_production_string(Production prod) {
    static char buffer[256];
    
    switch(prod) {
        case PROD_1: return "program -> decl-or-fun-list";
        case PROD_2: return "decl-or-fun-list -> decl-or-fun decl-or-fun-list";
        case PROD_3: return "decl-or-fun-list -> epsilon";
        case PROD_4: return "decl-or-fun -> FUN ID ( param-list ) : type NL* stmt-list END NL*";
        case PROD_5: return "decl-or-fun -> ID : type NL*";
        case PROD_6: return "param-list -> param param-list'";
        case PROD_7: return "param-list -> epsilon";
        case PROD_8: return "param-list' -> , param param-list'";
        case PROD_9: return "param-list' -> epsilon";
        case PROD_10: return "param -> ID : type";
        case PROD_11: return "type -> array-prefix base-type";
        case PROD_12: return "array-prefix -> [ ] array-prefix";
        case PROD_13: return "array-prefix -> epsilon";
        case PROD_14: return "base-type -> INT | BOOL | STRING | CHAR";
        case PROD_15: return "stmt-list -> statement stmt-list";
        case PROD_16: return "stmt-list -> epsilon";
        case PROD_17: return "statement -> ID : type NL*";
        case PROD_18: return "statement -> ID assign-or-call NL*";
        case PROD_19: return "statement -> RETURN expr NL*";
        case PROD_20: return "statement -> IF expr NL* stmt-list else-part END NL*";
        case PROD_21: return "statement -> WHILE expr NL* stmt-list LOOP NL*";
        case PROD_22: return "statement -> expr NL*";
        case PROD_23: return "assign-or-call -> = expr";
        case PROD_24: return "assign-or-call -> [ expr ] = expr";
        case PROD_25: return "assign-or-call -> ( arg-list ) index-chain";
        case PROD_26: return "else-part -> ELSE NL* stmt-list";
        case PROD_27: return "else-part -> epsilon";
        case PROD_28: return "arg-list -> expr arg-list'";
        case PROD_29: return "arg-list -> epsilon";
        case PROD_30: return "arg-list' -> , expr arg-list'";
        case PROD_31: return "arg-list' -> epsilon";
        case PROD_32: return "index-chain -> [ expr ] index-chain";
        case PROD_33: return "index-chain -> epsilon";
        case PROD_34: return "expr -> or-expr";
        case PROD_35: return "or-expr -> and-expr or-expr'";
        case PROD_36: return "or-expr' -> OR and-expr or-expr'";
        case PROD_37: return "or-expr' -> epsilon";
        case PROD_38: return "and-expr -> equality-expr and-expr'";
        case PROD_39: return "and-expr' -> AND equality-expr and-expr'";
        case PROD_40: return "and-expr' -> epsilon";
        case PROD_41: return "equality-expr -> relational-expr equality-expr'";
        case PROD_42: return "equality-expr' -> = relational-expr equality-expr'";
        case PROD_43: return "equality-expr' -> <> relational-expr equality-expr'";
        case PROD_44: return "equality-expr' -> epsilon";
        case PROD_45: return "relational-expr -> additive-expr relational-expr'";
        case PROD_46: return "relational-expr' -> > additive-expr relational-expr'";
        case PROD_47: return "relational-expr' -> < additive-expr relational-expr'";
        case PROD_48: return "relational-expr' -> >= additive-expr relational-expr'";
        case PROD_49: return "relational-expr' -> <= additive-expr relational-expr'";
        case PROD_50: return "relational-expr' -> epsilon";
        case PROD_51: return "additive-expr -> multiplicative-expr additive-expr'";
        case PROD_52: return "additive-expr' -> + multiplicative-expr additive-expr'";
        case PROD_53: return "additive-expr' -> - multiplicative-expr additive-expr'";
        case PROD_54: return "additive-expr' -> epsilon";
        case PROD_55: return "multiplicative-expr -> unary-expr multiplicative-expr'";
        case PROD_56: return "multiplicative-expr' -> * unary-expr multiplicative-expr'";
        case PROD_57: return "multiplicative-expr' -> / unary-expr multiplicative-expr'";
        case PROD_58: return "multiplicative-expr' -> epsilon";
        case PROD_59: return "unary-expr -> - unary-expr";
        case PROD_60: return "unary-expr -> NOT unary-expr";
        case PROD_61: return "unary-expr -> primary-expr";
        case PROD_62: return "primary-expr -> LITNUMERAL";
        case PROD_63: return "primary-expr -> LITSTRING";
        case PROD_64: return "primary-expr -> TRUE";
        case PROD_65: return "primary-expr -> FALSE";
        case PROD_66: return "primary-expr -> ID postfix-chain";
        case PROD_67: return "primary-expr -> ( expr )";
        case PROD_68: return "postfix-chain -> ( arg-list ) postfix-chain";
        case PROD_69: return "postfix-chain -> [ expr ] postfix-chain";
        case PROD_70: return "postfix-chain -> epsilon";
        default: return "NONE";
    }
}

void print_ll1_table() {
    printf("\n=== TABLA DE ANALISIS SINTACTICO LL(1) ===\n\n");
    
    // Inicializar la tabla
    init_ll1_table();
    
    // Imprimir encabezado
    printf("%-25s", "No-Terminal");
    for (int t = 0; t < T_COUNT; t++) {
        printf(" %-6s", get_terminal_name(t));
    }
    printf("\n");
    printf("--------------------------------------------------------------------------------\n");
    
    // Imprimir cada fila
    for (int nt = 0; nt < NT_COUNT; nt++) {
        printf("%-25s", get_nonterminal_name(nt));
        for (int t = 0; t < T_COUNT; t++) {
            Production prod = ll1_table[nt][t];
            if (prod != PROD_NONE) {
                printf(" (%2d)  ", prod);
            } else {
                printf("   -   ");
            }
        }
        printf("\n");
    }
}

Production lookup_ll1(NonTerminal nt, Terminal t) {
    if (nt >= 0 && nt < NT_COUNT && t >= 0 && t < T_COUNT) {
        return ll1_table[nt][t];
    }
    return PROD_NONE;
}
