# Análisis LL(1) - Parser Mini-0

## 1. Gramática formal transformada (sin ambigüedades)

### Notación
- Terminales: `MAYÚSCULAS` o entre comillas `'symbol'`
- No-terminales: `<minúsculas>`
- ε: cadena vacía
- | : alternativa
- * : cero o más repeticiones
- ? : opcional

### Producciones
```
(1)  <program> → <decl-or-fun-list>
(2)  <decl-or-fun-list> → <decl-or-fun> <decl-or-fun-list>
(3)  <decl-or-fun-list> → ε

(4)  <decl-or-fun> → FUN ID '(' <param-list> ')' ':' <type> NL* <stmt-list> END NL*
(5)  <decl-or-fun> → ID ':' <type> NL*

(6)  <param-list> → <param> <param-list'>
(7)  <param-list> → ε
(8)  <param-list'> → ',' <param> <param-list'>
(9)  <param-list'> → ε

(10) <param> → ID ':' <type>

(11) <type> → <array-prefix> <base-type>
(12) <array-prefix> → '[' ']' <array-prefix>
(13) <array-prefix> → ε
(14) <base-type> → INT | BOOL | STRING | CHAR

(15) <stmt-list> → <statement> <stmt-list>
(16) <stmt-list> → ε

(17) <statement> → ID ':' <type> NL*
(18) <statement> → ID <assign-or-call> NL*
(19) <statement> → RETURN <expr> NL*
(20) <statement> → IF <expr> NL* <stmt-list> <else-part> END NL*
(21) <statement> → WHILE <expr> NL* <stmt-list> LOOP NL*
(22) <statement> → <expr> NL*

(23) <assign-or-call> → '=' <expr>
(24) <assign-or-call> → '[' <expr> ']' '=' <expr>
(25) <assign-or-call> → '(' <arg-list> ')' <index-chain>

(26) <else-part> → ELSE NL* <stmt-list>
(27) <else-part> → ε

(28) <arg-list> → <expr> <arg-list'>
(29) <arg-list> → ε
(30) <arg-list'> → ',' <expr> <arg-list'>
(31) <arg-list'> → ε

(32) <index-chain> → '[' <expr> ']' <index-chain>
(33) <index-chain> → ε

(34) <expr> → <or-expr>
(35) <or-expr> → <and-expr> <or-expr'>
(36) <or-expr'> → OR <and-expr> <or-expr'>
(37) <or-expr'> → ε

(38) <and-expr> → <equality-expr> <and-expr'>
(39) <and-expr'> → AND <equality-expr> <and-expr'>
(40) <and-expr'> → ε

(41) <equality-expr> → <relational-expr> <equality-expr'>
(42) <equality-expr'> → '=' <relational-expr> <equality-expr'>
(43) <equality-expr'> → '<>' <relational-expr> <equality-expr'>
(44) <equality-expr'> → ε

(45) <relational-expr> → <additive-expr> <relational-expr'>
(46) <relational-expr'> → '>' <additive-expr> <relational-expr'>
(47) <relational-expr'> → '<' <additive-expr> <relational-expr'>
(48) <relational-expr'> → '>=' <additive-expr> <relational-expr'>
(49) <relational-expr'> → '<=' <additive-expr> <relational-expr'>
(50) <relational-expr'> → ε

(51) <additive-expr> → <multiplicative-expr> <additive-expr'>
(52) <additive-expr'> → '+' <multiplicative-expr> <additive-expr'>
(53) <additive-expr'> → '-' <multiplicative-expr> <additive-expr'>
(54) <additive-expr'> → ε

(55) <multiplicative-expr> → <unary-expr> <multiplicative-expr'>
(56) <multiplicative-expr'> → '*' <unary-expr> <multiplicative-expr'>
(57) <multiplicative-expr'> → '/' <unary-expr> <multiplicative-expr'>
(58) <multiplicative-expr'> → ε

(59) <unary-expr> → '-' <unary-expr>
(60) <unary-expr> → NOT <unary-expr>
(61) <unary-expr> → <primary-expr>

(62) <primary-expr> → LITNUMERAL
(63) <primary-expr> → LITSTRING
(64) <primary-expr> → TRUE
(65) <primary-expr> → FALSE
(66) <primary-expr> → ID <postfix-chain>
(67) <primary-expr> → '(' <expr> ')'

(68) <postfix-chain> → '(' <arg-list> ')' <postfix-chain>
(69) <postfix-chain> → '[' <expr> ']' <postfix-chain>
(70) <postfix-chain> → ε
```

---

## 2. Conjuntos FIRST
```
FIRST(<program>) = {FUN, ID, ε}
FIRST(<decl-or-fun-list>) = {FUN, ID, ε}
FIRST(<decl-or-fun>) = {FUN, ID}

FIRST(<param-list>) = {ID, ε}
FIRST(<param-list'>) = {',', ε}
FIRST(<param>) = {ID}

FIRST(<type>) = {'[', INT, BOOL, STRING, CHAR}
FIRST(<array-prefix>) = {'[', ε}
FIRST(<base-type>) = {INT, BOOL, STRING, CHAR}

FIRST(<stmt-list>) = {ID, RETURN, IF, WHILE, '(', LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE, ε}
FIRST(<statement>) = {ID, RETURN, IF, WHILE, '(', LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE}

FIRST(<assign-or-call>) = {'=', '[', '('}
FIRST(<else-part>) = {ELSE, ε}

FIRST(<arg-list>) = {'(', ID, LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE, ε}
FIRST(<arg-list'>) = {',', ε}
FIRST(<index-chain>) = {'[', ε}

FIRST(<expr>) = {'(', ID, LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE}
FIRST(<or-expr>) = {'(', ID, LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE}
FIRST(<or-expr'>) = {OR, ε}

FIRST(<and-expr>) = {'(', ID, LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE}
FIRST(<and-expr'>) = {AND, ε}

FIRST(<equality-expr>) = {'(', ID, LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE}
FIRST(<equality-expr'>) = {'=', '<>', ε}

FIRST(<relational-expr>) = {'(', ID, LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE}
FIRST(<relational-expr'>) = {'>', '<', '>=', '<=', ε}

FIRST(<additive-expr>) = {'(', ID, LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE}
FIRST(<additive-expr'>) = {'+', '-', ε}

FIRST(<multiplicative-expr>) = {'(', ID, LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE}
FIRST(<multiplicative-expr'>) = {'*', '/', ε}

FIRST(<unary-expr>) = {'-', NOT, '(', ID, LITNUMERAL, LITSTRING, TRUE, FALSE}

FIRST(<primary-expr>) = {LITNUMERAL, LITSTRING, TRUE, FALSE, ID, '('}
FIRST(<postfix-chain>) = {'(', '[', ε}
```

---

## 3. Conjuntos FOLLOW
```
FOLLOW(<program>) = {$}
FOLLOW(<decl-or-fun-list>) = {$}
FOLLOW(<decl-or-fun>) = {FUN, ID, $}

FOLLOW(<param-list>) = {')'}
FOLLOW(<param-list'>) = {')'}
FOLLOW(<param>) = {',', ')'}

FOLLOW(<type>) = {NL, ',', ')'}
FOLLOW(<array-prefix>) = {INT, BOOL, STRING, CHAR}
FOLLOW(<base-type>) = {NL, ',', ')'}

FOLLOW(<stmt-list>) = {END, ELSE, LOOP}
FOLLOW(<statement>) = {ID, RETURN, IF, WHILE, END, ELSE, LOOP, '(', LITNUMERAL, LITSTRING, '-', NOT, TRUE, FALSE}

FOLLOW(<assign-or-call>) = {NL}
FOLLOW(<else-part>) = {END}

FOLLOW(<arg-list>) = {')'}
FOLLOW(<arg-list'>) = {')'}
FOLLOW(<index-chain>) = {NL, ',', ')', ']', AND, OR, '=', '<>', '>', '<', '>=', '<=', '+', '-', '*', '/'}

FOLLOW(<expr>) = {NL, ',', ')', ']', AND, OR, '=', '<>', '>', '<', '>=', '<=', '+', '-', '*', '/'}
FOLLOW(<or-expr>) = {NL, ',', ')', ']'}
FOLLOW(<or-expr'>) = {NL, ',', ')', ']'}

FOLLOW(<and-expr>) = {OR, NL, ',', ')', ']'}
FOLLOW(<and-expr'>) = {OR, NL, ',', ')', ']'}

FOLLOW(<equality-expr>) = {AND, OR, NL, ',', ')', ']'}
FOLLOW(<equality-expr'>) = {AND, OR, NL, ',', ')', ']'}

FOLLOW(<relational-expr>) = {'=', '<>', AND, OR, NL, ',', ')', ']'}
FOLLOW(<relational-expr'>) = {'=', '<>', AND, OR, NL, ',', ')', ']'}

FOLLOW(<additive-expr>) = {'>', '<', '>=', '<=', '=', '<>', AND, OR, NL, ',', ')', ']'}
FOLLOW(<additive-expr'>) = {'>', '<', '>=', '<=', '=', '<>', AND, OR, NL, ',', ')', ']'}

FOLLOW(<multiplicative-expr>) = {'+', '-', '>', '<', '>=', '<=', '=', '<>', AND, OR, NL, ',', ')', ']'}
FOLLOW(<multiplicative-expr'>) = {'+', '-', '>', '<', '>=', '<=', '=', '<>', AND, OR, NL, ',', ')', ']'}

FOLLOW(<unary-expr>) = {'*', '/', '+', '-', '>', '<', '>=', '<=', '=', '<>', AND, OR, NL, ',', ')', ']'}

FOLLOW(<primary-expr>) = {'*', '/', '+', '-', '>', '<', '>=', '<=', '=', '<>', AND, OR, NL, ',', ')', ']'}
FOLLOW(<postfix-chain>) = {'*', '/', '+', '-', '>', '<', '>=', '<=', '=', '<>', AND, OR, NL, ',', ')', ']'}
```

---

## 4. Tabla de análisis sintáctico LL(1)

**Nota:** Tabla simplificada mostrando las producciones más relevantes. 
En una implementación completa, cada celda contiene el número de producción a aplicar.

### Tabla principal (selección)

| No-terminal | FUN | ID | ':' | INT | IF | WHILE | RETURN | '(' | ')' | NL | END | $ |
|-------------|-----|----|----|-----|-------|-------|--------|-----|-----|----|-----|---|
| `<program>` | 1 | 1 | - | - | - | - | - | - | - | 1 | - | 1 |
| `<decl-or-fun>` | 4 | 5 | - | - | - | - | - | - | - | - | - | - |
| `<type>` | - | - | - | 11 | - | - | - | - | - | - | - | - |
| `<statement>` | - | 17,18 | - | - | 20 | 21 | 19 | 22 | - | - | - | - |
| `<expr>` | - | 34 | - | - | - | - | - | 34 | - | - | - | - |

### Detalle por secciones

#### Sección: Declaraciones
```
M[<decl-or-fun>, FUN] = (4) FUN ID '(' <param-list> ')' ':' <type> NL* <stmt-list> END NL*
M[<decl-or-fun>, ID] = (5) ID ':' <type> NL*
```

#### Sección: Tipos
```
M[<type>, '['] = (11) <array-prefix> <base-type>
M[<type>, INT] = (11) <array-prefix> <base-type>
M[<type>, BOOL] = (11) <array-prefix> <base-type>
M[<type>, STRING] = (11) <array-prefix> <base-type>
M[<type>, CHAR] = (11) <array-prefix> <base-type>

M[<array-prefix>, '['] = (12) '[' ']' <array-prefix>
M[<array-prefix>, INT] = (13) ε
M[<array-prefix>, BOOL] = (13) ε
M[<array-prefix>, STRING] = (13) ε
M[<array-prefix>, CHAR] = (13) ε
```

#### Sección: Sentencias
```
M[<statement>, ID] = (17) ID ':' <type> NL*  | (18) ID <assign-or-call> NL*
M[<statement>, RETURN] = (19) RETURN <expr> NL*
M[<statement>, IF] = (20) IF <expr> NL* <stmt-list> <else-part> END NL*
M[<statement>, WHILE] = (21) WHILE <expr> NL* <stmt-list> LOOP NL*
```

#### Sección: Expresiones (precedencia)
```
M[<expr>, ID/LITNUMERAL/LITSTRING/TRUE/FALSE/'('/-/NOT] = (34) <or-expr>

M[<or-expr>, ...] = (35) <and-expr> <or-expr'>
M[<or-expr'>, OR] = (36) OR <and-expr> <or-expr'>
M[<or-expr'>, NL/,/)/ ]] = (37) ε

M[<and-expr>, ...] = (38) <equality-expr> <and-expr'>
M[<and-expr'>, AND] = (39) AND <equality-expr> <and-expr'>
M[<and-expr'>, OR/NL/,/)/ ]] = (40) ε

... (continúa con relational, additive, multiplicative, unary)
```

#### Sección: Primarias y postfijos
```
M[<primary-expr>, LITNUMERAL] = (62) LITNUMERAL
M[<primary-expr>, LITSTRING] = (63) LITSTRING
M[<primary-expr>, TRUE] = (64) TRUE
M[<primary-expr>, FALSE] = (65) FALSE
M[<primary-expr>, ID] = (66) ID <postfix-chain>
M[<primary-expr>, '('] = (67) '(' <expr> ')'

M[<postfix-chain>, '('] = (68) '(' <arg-list> ')' <postfix-chain>
M[<postfix-chain>, '['] = (69) '[' <expr> ']' <postfix-chain>
M[<postfix-chain>, otros] = (70) ε
```

---

## 5. Verificación de la gramática LL(1)

### Condiciones LL(1):
Para cada no-terminal A con producciones A → α₁ | α₂ | ... | αₙ:

1.  **FIRST disjuntos:** FIRST(αᵢ) ∩ FIRST(αⱼ) = ∅ para i ≠ j
2.  **Si ε ∈ FIRST(αᵢ):** FIRST(αⱼ) ∩ FOLLOW(A) = ∅ para todo j ≠ i

### Casos verificados:

**<decl-or-fun>:**
- FIRST(FUN ...) = {FUN}
- FIRST(ID ':' ...) = {ID}
-  Disjuntos

**<statement> (ambigüedad ID):**
- Producción 17: ID ':' <type> (declaración)
- Producción 18: ID <assign-or-call> (asignación)
-  Resuelto con lookahead: si siguiente es ':', es declaración

**<expr> (precedencia):**
-  Resuelto con gramática estratificada (or → and → equality → ... → primary)
- Cada nivel tiene FIRST disjuntos o usa primos (') para eliminar recursión izquierda

---

## 6. Conflictos resueltos

### Conflicto 1: Declaración vs Asignación
```
Entrada: "x = 10"  vs  "x: int"
```
**Solución:** Lookahead en `<statement>`:
- Si `peek_token(1) == ':'` → declaración (prod. 17)
- Si `peek_token(1) == '='` o `'['` → asignación (prod. 18)

### Conflicto 2: Operador '=' (asignación vs comparación)
```
Contexto sentencia: x = 10  (asignación)
Contexto expresión: if x = 10  (comparación)
```
**Solución:** Por contexto sintáctico:
- En `<statement>` después de ID → asignación
- En `<expr>` → operador de igualdad

### Conflicto 3: Recursión izquierda en expresiones
```
Original: E → E + T | T  (recursiva izquierda)
```
**Solución:** Transformación con primos:
```
E → T E'
E' → + T E' | ε
```

---

## 7. Correspondencia con la implementación

El parser en `parser.c` implementa esta gramática mediante:
```c
// Funciones que corresponden a no-terminales:
parse_program()           → <program>
parse_type()              → <type>
parse_statement()         → <statement>
parse_expression()        → <expr>
parse_or_expr()           → <or-expr>
parse_and_expr()          → <and-expr>
parse_equality()          → <equality-expr>
parse_relational()        → <relational-expr>
parse_additive()          → <additive-expr>
parse_multiplicative()    → <multiplicative-expr>
parse_unary()             → <unary-expr>
parse_primary()           → <primary-expr>
```

**Lookahead:** Se usa `cur_token()` y `peek_token(1)` para decisiones LL(1).

---

## 8. Ejemplo de derivación

### Programa de entrada:
```mini0
fun main(): int
    x: int
    x = 5 + 3
    return x
end
```

### Derivación (izquierda):
```
<program>
⇒ <decl-or-fun-list>
⇒ <decl-or-fun> <decl-or-fun-list>
⇒ FUN ID '(' <param-list> ')' ':' <type> NL <stmt-list> END NL <decl-or-fun-list>
⇒ FUN main '(' ε ')' ':' <type> NL <stmt-list> END NL ε
⇒ FUN main '(' ')' ':' INT NL <stmt-list> END NL
⇒ FUN main '(' ')' ':' INT NL <statement> <stmt-list> END NL
⇒ FUN main '(' ')' ':' INT NL ID ':' <type> NL <stmt-list> END NL
⇒ FUN main '(' ')' ':' INT NL x ':' INT NL <stmt-list> END NL
⇒ ... (continúa con x = 5 + 3, return x)
```

---


