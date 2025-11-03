
PASO 2: ABRIR WSL (EN WINDOWS)
bashwsl

PASO 3: INSTALAR DEPENDENCIAS (SOLO PRIMERA VEZ)
bashapk add flex gcc make musl-dev flex-dev

PASO 4: IR A LA CARPETA DEL PROYECTO
bashcd /mnt/host/d/USUARIO/Desktop/clon
(Ajusta la ruta según tu caso)

PASO 5: COMPILAR
bashmake clean
make
```

Verás:
```
flex mini0_lex.l
gcc -Wall -g -c lex.yy.c
gcc -Wall -g -c token.c
gcc -Wall -g -o mini0_lex lex.yy.o token.o -lfl

PASO 6: CREAR ARCHIVOS DE PRUEBA
test_basic.mini0
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
test_hex.mini0
bashcat > test_hex.mini0 << 'EOF'
fun main(): int
    x: int
    x = 0xFF
    x = 0x0F
    return x
end
EOF
test_string.mini0
bashcat > test_string.mini0 << 'EOF'
fun main(): int
    s: string
    s = "Hola\nMundo"
    s = "Tab:\taqui"
    s = "Comillas: \"hola\""
    return 0
end
EOF
test_errors.mini0
bashcat > test_errors.mini0 << 'EOF'
fun main(): int
    x = 10$20
    y = @variable
    z = test#error
    return 0
end
EOF
test_completo.mini0
bashcat > test_completo.mini0 << 'EOF'
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

PASO 7: EJECUTAR LAS PRUEBAS
Prueba básica:
bash./mini0_lex test_basic.mini0
Prueba hexadecimal:
bash./mini0_lex test_hex.mini0
Prueba strings:
bash./mini0_lex test_string.mini0
Prueba errores:
bash./mini0_lex test_errors.mini0
Prueba completa:
bash./mini0_lex test_completo.mini0
```

---

### PASO 8: VERIFICAR RESULTADOS

**Para test_hex.mini0 debes ver:**
```
Linea   3: LITNUMERAL      [0xFF] -> valor: 255
Linea   4: LITNUMERAL      [0x0F] -> valor: 15
```
✓ Conversión correcta de hexadecimal a decimal

**Para test_string.mini0 debes ver:**
```
Linea   3: LITSTRING       ["Hola\nMundo"] -> valor: "Hola
Mundo"
Linea   4: LITSTRING       ["Tab:\taqui"] -> valor: "Tab:	aqui"
Linea   5: LITSTRING       ["Comillas: \"hola\""] -> valor: "Comillas: "hola""
```
✓ Escapes procesados correctamente

**Para test_errors.mini0 debes ver:**
```
Error léxico en línea 2: carácter no reconocido '$'
Error léxico en línea 3: carácter no reconocido '@'
Error léxico en línea 4: carácter no reconocido '#'
✓ Errores detectados

RESUMEN DE COMANDOS
bash# 1. Abrir WSL
wsl

# 2. Instalar (solo primera vez)
apk add flex gcc make musl-dev flex-dev

# 3. Ir a carpeta
cd /mnt/host/d/USUARIO/Desktop/clon

# 4. Compilar
make

# 5. Ejecutar
./mini0_lex test_basic.mini0
./mini0_lex test_hex.mini0
./mini0_lex test_string.mini0
./mini0_lex test_errors.mini0
./mini0_lex test_completo.mini0ReintentarClaude puede cometer errores. Por favor, verifique las respuestas. Sonnet 4.5