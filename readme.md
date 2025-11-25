# 🧠 Proyecto Mini0 — Analizador Léxico (Flex + GCC)

Este proyecto implementa el **analizador léxico** del lenguaje *Mini0* usando **Flex**, **GCC** y **Make**.

---
# Lexico
LETRA           [a-zA-Z]

DIGITO          [0-9]

HEXDIGITO       [0-9a-fA-F]

ID              ({LETRA}|)({LETRA}|{DIGITO}|)*

DECIMAL         {DIGITO}+

HEXADECIMAL     0x{HEXDIGITO}+

STRING          \"([^\\\"\n]|\\.)*\"

ESPACIO         [ \t\r]

SALTO_LINEA     \n

---

## 🚀 PASO 1: ABRIR WSL

Abre tu terminal de Linux (WSL) desde Windows:

```bash
wsl
```

---

## 🧩 PASO 2: INSTALAR DEPENDENCIAS *(solo la primera vez)*

Ejecuta los siguientes comandos:

```bash
apk add flex gcc make musl-dev flex-dev
```

> 🛠️ **Dependencias:**
>
> * `flex` → Generador de analizadores léxicos
> * `gcc` → Compilador de C
> * `make` → Automatiza la compilación
> * `musl-dev`, `flex-dev` → Librerías de desarrollo

---

## 📂 PASO 3: IR A LA CARPETA DEL PROYECTO

Cambia a la ruta donde está el proyecto:

```bash
cd /mnt/host/d/USUARIO/Desktop/clon
```

> ⚠️ Ajusta la ruta según tu usuario o ubicación.

---

## ⚙️ PASO 4: COMPILAR EL PROYECTO

Ejecuta los comandos:

```bash
make clean
make
```

Deberías ver algo como:

```
flex mini0_lex.l
gcc -Wall -g -c lex.yy.c
gcc -Wall -g -c token.c
gcc -Wall -g -o mini0_lex lex.yy.o token.o -lfl
```

> ✅ Si no hay errores, la compilación fue exitosa.

---

## 🧪 PASO 5: CREAR ARCHIVOS DE PRUEBA

Crea los siguientes archivos dentro del mismo directorio:

### 1️⃣ `test_basic.mini0`

```bash
cat > test_basic.mini0 << 'EOF'
fun main(): int
    x: int
    y: bool
    x = 42
    y = true
    
    if x > 10
        return 1
    end
    
    return 0
end
EOF
```

---

### 2️⃣ `test_hex.mini0`

```bash
cat > test_hex.mini0 << 'EOF'
fun main(): int
    x: int
    x = 0xFF
    x = 0x0F
    return x
end
EOF
```

---

### 3️⃣ `test_string.mini0`

```bash
cat > test_string.mini0 << 'EOF'
fun main(): int
    s: string
    s = "Hola\nMundo"
    s = "Tab:\taqui"
    s = "Comillas: \"hola\""
    return 0
end
EOF
```

---

### 4️⃣ `test_errors.mini0`

```bash
cat > test_errors.mini0 << 'EOF'
fun main(): int
    x = 10$20
    y = @variable
    z = test#error
    return 0
end
EOF
```

---

### 5️⃣ `test_completo.mini0`

```bash
cat > test_completo.mini0 << 'EOF'
contador: int

fun factorial(n: int): int
    resultado: int
    resultado = 1
    
    while n > 1
        resultado = resultado * n
        n = n - 1
    loop
    
    return resultado
end

fun main(): int
    numeros: []int
    x: int
    flag: bool
    
    numeros = new [10] int
    numeros[0] = 0xA5
    
    x = factorial(5)
    
    if x >= 100 and x <= 200
        flag = true
    else
        flag = false
    end
    
    return x
end
EOF
```

---

## ▶️ PASO 6: EJECUTAR LAS PRUEBAS

Ejecuta cada archivo de prueba:

```bash
./mini0_lex test_basic.mini0
./mini0_lex test_hex.mini0
./mini0_lex test_string.mini0
./mini0_lex test_errors.mini0
./mini0_lex test_completo.mini0
```

---

## 🔍 PASO 7: RESULTADOS ESPERADOS

### ✅ Prueba **Hexadecimal**

```
Linea   3: LITNUMERAL      [0xFF] -> valor: 255
Linea   4: LITNUMERAL      [0x0F] -> valor: 15
```

✔ Conversión correcta de hexadecimal a decimal.

---

### ✅ Prueba **Strings**

```
Linea   3: LITSTRING       ["Hola\nMundo"] -> valor: "Hola
Mundo"
Linea   4: LITSTRING       ["Tab:\taqui"] -> valor: "Tab:	aqui"
Linea   5: LITSTRING       ["Comillas: \"hola\""] -> valor: "Comillas: "hola""
```

✔ Los caracteres de escape fueron procesados correctamente.

---

### ⚠️ Prueba **Errores Léxicos**

```
Error léxico en línea 2: carácter no reconocido '$'
Error léxico en línea 3: carácter no reconocido '@'
Error léxico en línea 4: carácter no reconocido '#'
```

✔ Errores léxicos detectados correctamente.

---

## 🧾 RESUMEN DE COMANDOS

```bash
# 1. Abrir WSL
wsl

# 2. Instalar dependencias (solo primera vez)
apk add flex gcc make musl-dev flex-dev

# 3. Ir a la carpeta del proyecto
cd /mnt/host/d/USUARIO/Desktop/clon

# 4. Compilar
make clean
make

# 5. Ejecutar pruebas
./mini0_lex test_basic.mini0
## 6. Analizador sintáctico (nuevo)

Se añadió un analizador sintáctico recursivo-descendente en C. Para compilar el parser use:

```bash
make clean
make
```

El binario del parser se llama `mini0_parser`. Ejemplos para ejecutar:

```bash
./mini0_parser tests/parser_valid.mini0
./mini0_parser tests/parser_error_missing_end.mini0
./mini0_parser tests/parser_error_bad_syntax.mini0
```

En caso de éxito el parser imprimirá "Analisis sintactico completado" y retornará 0. En caso de errores léxicos o sintácticos el programa imprimirá la(s) causa(s) en stderr y retornará un código distinto de cero.
./mini0_lex test_hex.mini0
./mini0_lex test_string.mini0
./mini0_lex test_errors.mini0
./mini0_lex test_completo.mini0

