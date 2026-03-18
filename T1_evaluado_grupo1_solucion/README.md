# Taller 1 2026-1

```sh
# compilar solución
make

# limpiar compilacion de solución
make clean


# correr test cases:
chmod +x run.sh
./run.sh

```
El script run genera una carpeta outputs, que por cada test, almacena el output de tu codigo junto a un archivo .diff con las diferencias entre tu output y el correcto del test. 

Recordar que para ejecutar un test en particular, se usa:
```sh
./ejecutable archivo_input.txt archivo_output.txt
```

Y en caso de querer usar valgrind para debugear:

```sh
valgrind ./ejecutable archivo_input.txt archivo_output.txt
```
