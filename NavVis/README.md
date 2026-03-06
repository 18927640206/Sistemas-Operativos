# Practica Navegador y Visor de Archivos

## Datos

Sistemas Operativos
Joahan Andres Gonzalez Padron
Matricula: 2223073570

## Archivos

hexEditor.c -> visor hexadecimal interactivo  
checkSum.c -> calcula checksum del archivo  

## Compilación

Facil:
make

o manualmente:

gcc -o hexEditor hexEditor.c -lncurses  
gcc -o checkSum checkSum.c

## Uso

### Visor hexadecimal

./hexEditor archivo

Controles:

↑ ↓ → moverse  
PgUp / PgDn → avanzar páginas  
g → ir a offset  
i → inicio del archivo  
f → final del archivo  
Ctrl+X → salir

### Checksum

./checkSum archivo

Muestra la suma hexadecimal de todos los bytes del archivo.

## Características

 * Uso de memoria virtual con mmap
 * Visualización hexadecimal
 * Navegacion por offsets
 * Interfaz en terminal con ncurses