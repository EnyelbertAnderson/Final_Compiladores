#ifndef PARSER_H
#define PARSER_H

#include "token.h"

// Interfaz principal del analizador sintáctico
// Devuelve 0 en caso de éxito sintáctico, distinto de cero en caso de error.
int analizar_sintactico(const char *ruta_archivo);

#endif
