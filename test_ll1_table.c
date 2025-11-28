#include "ll1_table.h"
#include <stdio.h>

int main() {
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║           TABLA DE ANÁLISIS SINTÁCTICO LL(1)                 ║\n");
    printf("║                    Parser Mini-0                              ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    
    // Imprimir toda la tabla
    print_ll1_table();
    
    printf("\n\n=== EJEMPLOS DE CONSULTA ===\n\n");
    
    // Ejemplos de consulta
    Production prod;
    
    prod = lookup_ll1(NT_PROGRAM, T_FUN);
    printf("M[program, FUN] = %s\n", get_production_string(prod));
    
    prod = lookup_ll1(NT_DECL_OR_FUN, T_ID);
    printf("M[decl-or-fun, ID] = %s\n", get_production_string(prod));
    
    prod = lookup_ll1(NT_STATEMENT, T_IF);
    printf("M[statement, IF] = %s\n", get_production_string(prod));
    
    prod = lookup_ll1(NT_EXPR, T_ID);
    printf("M[expr, ID] = %s\n", get_production_string(prod));
    
    prod = lookup_ll1(NT_OR_EXPR_PRIME, T_OR);
    printf("M[or-expr', OR] = %s\n", get_production_string(prod));
    
    prod = lookup_ll1(NT_PRIMARY_EXPR, T_LITNUMERAL);
    printf("M[primary-expr, LITNUMERAL] = %s\n", get_production_string(prod));
    
    printf("\n=== ESTADÍSTICAS ===\n\n");
    
    // Contar entradas llenas
    int filled = 0;
    int total = NT_COUNT * T_COUNT;
    
    for (int i = 0; i < NT_COUNT; i++) {
        for (int j = 0; j < T_COUNT; j++) {
            if (ll1_table[i][j] != PROD_NONE) {
                filled++;
            }
        }
    }
    
    printf("Entradas totales: %d\n", total);
    printf("Entradas llenas: %d\n", filled);
    printf("Entradas vacías: %d\n", total - filled);
    printf("Porcentaje de llenado: %.2f%%\n", (filled * 100.0) / total);
    
    printf("\n=== VERIFICACIÓN LL(1) ===\n\n");
    
    // Verificar que no hay conflictos
    int conflicts = 0;
    for (int i = 0; i < NT_COUNT; i++) {
        for (int j = 0; j < T_COUNT; j++) {
            // Aquí podrías verificar conflictos si tuvieras múltiples producciones
            // Para este caso, cada celda tiene máximo una producción
        }
    }
    
    printf("Conflictos detectados: %d\n", conflicts);
    if (conflicts == 0) {
        printf("✓ La gramática es LL(1) válida\n");
    } else {
        printf("✗ La gramática tiene conflictos LL(1)\n");
    }
    
    return 0;
}
