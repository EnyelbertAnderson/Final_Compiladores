# 🧠 Proyecto Mini0 — Analizador Léxico (Flex + GCC)

Este proyecto implementa el **analizador léxico** del lenguaje *Mini0*, utilizando **Flex** y **GCC**.

---

## 🚀 PASO 1: ABRIR WSL

Abre la terminal de WSL (por ejemplo, **Alpine** o **Ubuntu**) desde Windows:

```bash
wsl
```

---

## 🧩 PASO 2: INSTALAR DEPENDENCIAS *(solo la primera vez)*

Ejecuta los siguientes comandos para instalar las herramientas necesarias:

```bash
apk add flex gcc make musl-dev flex-dev
```

> 🛠️ **Notas:**
>
> * `flex`: generador de analizadores léxicos.
> * `gcc`: compilador de C.
> * `make`: automatiza el proceso de compilación.
> * `musl-dev` y `flex-dev`: librerías base para compilación.

---

## 📂 PASO 3: IR A LA CARPETA DEL PROYECTO

Cambia al directorio donde está el proyecto.
Ajusta la ruta según tu caso:

```bash
cd /mnt/host/d/USUARIO/Desktop/clon
```

---

## ⚙️ PASO 4: COMPILAR EL PROYECTO

Ejecuta:

```bash
make clean
make
```

Deberías ver una salida similar:

```
flex mini0_lex.l
gcc -Wall -g -c lex.yy.c
gcc -Wall -g -c token.c
gcc -Wall -g -o mini0_lex lex.yy.o token.o -lfl
```

> ✅ Si no aparece ningún error, la compilación fue **exitosa**.

---

## 🧪 PASO 5: CREAR ARCHIVOS DE PRUEBA

Crea los siguientes archivos de prueba directamente desde la terminal:

### 🧩 1. Prueba básica (`test_basic.mini0`)

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

### 🔢 2. Prueba de Hexadecimales (`test_hex.mini0`)

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

### 🧵 3. Prueba de Cadenas (`test_string.mini0`)

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

### ⚠️ 4. Prueba de Errores Léxicos (`test_errors.mini0`)

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

### 🧮 5. Prueba Completa (`test_completo.mini0`)

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

Ejecuta cada prueba una por una:

```bash
./mini0_lex test_basic.mini0
./mini0_lex test_hex.mini0
./mini0_lex test_string.mini0
./mini0_lex test_errors.mini0
./mini0_lex test_completo.mini0
```

---

## 🔍 PASO 7: RESULTADOS ESPERADOS

### ✅ **Prueba Hexadecimal**

```
Linea   3: LITNUMERAL      [0xFF] -> valor: 255
Linea   4: LITNUMERAL      [0x0F] -> valor: 15
```

✔ Conversión correcta de hexadecimal a decimal.

---

### ✅ **Prueba de Cadenas**

```
Linea   3: LITSTRING       ["Hola\nMundo"] -> valor: "Hola
Mundo"
Linea   4: LITSTRING       ["Tab:\taqui"] -> valor: "Tab:	aqui"
Linea   5: LITSTRING       ["Comillas: \"hola\""] -> valor: "Comillas: "hola""
```

✔ Los caracteres de escape se procesaron correctamente.

---

### ⚠️ **Prueba de Errores Léxicos**

```
Error léxico en línea 2: carácter no reconocido '$'
Error léxico en línea 3: carácter no reconocido '@'
Error léxico en línea 4: carácter no reconocido '#'
```

✔ Errores detectados y reportados correctamente.

---

## 🧾 RESUMEN DE COMANDOS

```bash
# 1. Abrir WSL
wsl

# 2. Instalar dependencias (solo la primera vez)
apk add flex gcc make musl-dev flex-dev

# 3. Ir a la carpeta del proyecto
cd /mnt/host/d/USUARIO/Desktop/clon

# 4. Compilar
make clean
make

# 5. Ejecutar pruebas
./mini0_lex test_basic.mini0
./mini0_lex test_hex.mini0
./mini0_lex test_string.mini0
./mini0_lex test_errors.mini0
./mini0_lex test_completo.mini0
```

---

### 💡 Autor

**Enyelbert A. Panta Huaracha**
**Piero O. De la Cruz Mancilla**
Proyecto académico – *Análisis Léxico Mini0*
📘 Basado en Flex + GCC + Makefile
🧰 Compatible con **WSL / Alpine Linux**

---
