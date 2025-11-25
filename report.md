# Informe técnico — Analizador sintáctico Mini-0

Resumen breve

- Implementé un analizador sintáctico recursivo-descendente (top-down) para Mini-0 en C (`parser.c`, `parser.h`) que usa el analizador léxico existente (Flex) para obtener los tokens.
- Se añadió un ejecutable `mini0_parser` al `Makefile`.

Cambios en la gramática

La gramática original fue adaptada para eliminar recursión por la izquierda y ambigüedades, así se facilita el análisis recursivo-descendente. Puntos clave:

- Tipos: se permite un prefijo repetido de `[]` para declarar arreglos, seguido por un tipo base (`int`, `bool`, `string`, `char`). Ej.: `[]int`, `[][]string`.
- Declaraciones globales: `ID : type`.
- Definición de funciones: `fun ID ( params ) : type` seguidas por el cuerpo y `end` terminador. Los parámetros se escriben como `id : type` separados por comas.
- Sentencias: declaración de variables, asignaciones (`id = expr` o `id[expr] = expr`), `return expr`, `if expr ... [else ...] end`, `while expr ... loop`.
- Las expresiones soportan llamadas a funciones `id(args...)`, acceso a arreglos `id[expr]`, literales numéricos y strings, operadores `* / + - > < >= <= = <> and or not` y precedencias estándar (unario > multiplicativo > aditivo > relacional > igualdad > and > or).

Manejo de la ambigüedad entre asignación y comparación

- El token `=` se usa tanto para asignación como para comparación.
- Diferenciación por contexto: si aparece después de un identificador (o `id[expr]`) a nivel de sentencia, se toma como asignación; mientras que en el interior de una expresión se considera un operador de igualdad.

Errores detectados y reportados

- Errores léxicos (ya manejados por el lexer): caracteres no reconocidos — el lexer imprime mensajes con línea y devuelve código distinto de cero.
- Errores sintácticos detectados por el parser:
  - Token inesperado o faltante (p.ej. ")", "banala: expected type", etc.).
  - Bloques no terminados (falta `end` o `loop`).
  - Sentencias no reconocidas.
  - Expresiones incompletas o mal formadas.

Formato de mensajes

- El parser imprime un mensaje claro en `stderr` con la línea y el token problemático cuando detecta un error sintáctico.
- El programa sale con código de error (no cero) si hubo errores léxicos o sintácticos.

Notas de implementación

- El analizador léxico fue modificado para exponer las estructuras globales de tokens y una función `analizar_lexico(const char *ruta_archivo)` que llena `tokens` y `num_tokens`.
- El parser recorre el buffer de tokens en memoria (`tokens[]`) y valida la estructura sintáctica.

Archivos añadidos/ modificados

- Añadidos: `parser.c`, `parser.h`, `report.md`, tests en `tests/`.
- Modificados: `mini0_lex.l` (eliminado main y exportada función `analizar_lexico`), `token.h` (declaraciones extern), `Makefile` (target `mini0_parser`).

Pruebas

- Se añadieron pruebas de muestra en `tests/` que ejercitan casos válidos y de error.

---

Si quieres que añada un AST, generador de código, o pruebas automatizadas (un `make test` que ejecute casos y compare salidas), puedo hacerlo en la siguiente fase.
