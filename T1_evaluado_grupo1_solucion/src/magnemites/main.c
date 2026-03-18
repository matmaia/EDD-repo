#include <stdlib.h>
#include <string.h>
#include "events.h"

/* Retorna true si ambos strings son iguales */
static bool string_equals(char *string1, char *string2) {
    return !strcmp(string1, string2);
}

static bool check_arguments(int argc, char **argv) {
    if (argc != 3) {
        printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
        printf("Donde:\n");
        printf("\tINPUT es la ruta del archivo de input\n");
        printf("\tOUTPUT es la ruta del archivo de output\n");
        exit(1);
    }
    return true;
}

int main(int argc, char **argv){
    /* Definicion de argumentos */
    check_arguments(argc, argv);
    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");

    int S; // Cantidad maxima de MagneZones
    int E; // Cantidad de eventos

    //La funcion fscanf lee desde el archivo de input segun los especificadores de formato en el string
    //del segundo argumento. Lo que lea, se guarda en la dirección de memoria especificada en el tercer argumento
    //Se guarda en buffer un int de status para confirmar si todo salió bien. 
    int buffer = fscanf(input_file, "%d", &S);

    if (buffer != 1) {
        printf("Error reading maximum number of sectors\n");
        return 1;
    }

    buffer = fscanf(input_file, "%d", &E);

    if (buffer != 1) {
        printf("Error reading number of events\n");
        return 1;
    }

    /* Eventos */

    char command[32];
    MagneZone* magnezones[S];

    for (int i = 0; i < S; i++) {
        magnezones[i] = magnezone_create(i);
    }

    for (int i = 0; i < E; i++){
        buffer = fscanf(input_file, "%s", command);
        if (buffer != 1) {
            printf("Error reading event\n");
            return 1;
        }

        if (string_equals(command, "ENTER")) {
            int magnezone_id;
            int magnemite_id;
            int magnemite_atk;
            int magnemite_speed;
            fscanf(input_file, " %d %d %d %d", &magnezone_id, &magnemite_id, &magnemite_atk, &magnemite_speed);

            events_enter(output_file, magnezones, magnezone_id, magnemite_id, magnemite_atk, magnemite_speed);
        } else if (string_equals(command, "FIND")) {
            int magnezone_id;
            int target_atk;
            fscanf(input_file, " %d %d", &magnezone_id, &target_atk);

            events_find(output_file, magnezones, magnezone_id, target_atk);
        } else if (string_equals(command, "REMOVE")) {
            int magnemite_id;
            fscanf(input_file, " %d", &magnemite_id);

            events_remove(output_file, magnezones, S, magnemite_id);
        } else if (string_equals(command, "INFO")) {
            int magnezone_id;
            fscanf(input_file, " %d", &magnezone_id);

            events_info(output_file, magnezones, magnezone_id);
        } else if (string_equals(command, "INVERSE")){


            int magnezone_id;
            fscanf(input_file, " %d", &magnezone_id);

            /*Para la funcion inverse se proponen 2 soluciones, la primera es mas eficiente en terminos de complejidad
              pero ambas validas a efectos del taller
              
              1. Usar una lista doblemente ligada: En vez de tener solo un puntero next en cada nodo, almacenamos tambien
                 un puntero prev. Para hacer este cambio, debemos actualizar las funciones anteriores que alteraban los punteros
                 para ahora actualizar correctamente el puntero prev, estas funciones son: insert, create_magnemite y remove.
                 Una vez hecho esto, el inverse consiste en simplemente para cada nodo, intercambiar el puntero next y prev.
                 
              2. Crear una copia: Un poco mas ineficiente que la anterior, pero es posible crear un nuevo magnezone y mover 
                 los magnemites pero ahora en orden inverso
                 
              
               Se entrega el codigo para ambas soluciones
               
               NOTA: Tambien se puede resolver solo actualizando el puntero next, sin hacer copias ni usar listas doblemente ligadas y 
               en una sola pasada por la lista. Esta solución es la mejor de todas pero esto requiere llevar una logica de triple 
               punteros, lo cual puede ser dificil de manejar sino sabes lo que haces */

            events_inverse_1(output_file, magnezones, magnezone_id);

            //events_inverse_2(output_file, magnezones, magnezone_id);
        }else if (string_equals(command, "SHIFT")){

            /*Para hacer el shift necesitamos encontrar 2 nodos importantes
            
            1. La cola actual
            2. La nueva cola despues del shift. 
            
            Con esto, podemos pensar que hacer shift en k posiciones es simplemente
            mover los ultimos k nodos al inicio. Con un ejemplo se ve mas facil (recomiendo dibujarlo):
            
            Con k = 2
            
            Nodo1(cabeza) ->  Nodo2(nueva_cola)-> Nodo3 -> Nodo4(cola_actual)
            
            
            ahora fijense que si hacemos la siguiente secuencia de pasos, completamos el shift 
            

            1. Creamos una lista ligada circular. Para esto Nodo4->next = cabeza. Asi queda la lista:

            Nodo1(cabeza) ->  Nodo2(nueva_cola)-> Nodo3 -> Nodo4(cola_actual) ->Nodo1(cabeza) ...
            

            2. Ahora, la nueva cabeza es el siguiente a la nueva cola. Para esto
            cabeza = Nueva_cola->next. Asi queda la lista:
            
            Nodo1 ->  Nodo2(nueva_cola) ->  Nodo3(cabeza) -> Nodo4(cola_actual)->Nodo1 ...  
            
            3. Finalmente, eliminamos el puntero entre la nueva cola y la cabeza.
            Para esto nueva_cola->next = NULL. Asi queda la lista:

            Nodo1 ->  Nodo2(nueva_cola)  Nodo3(cabeza) -> Nodo4 ->Nodo1 ... 

            y de esta forma, ahora la lista se movió en k posiciones con solo actualizar 3 punteros
            */

            


            int magnezone_id;
            int k;
            fscanf(input_file, " %d %d", &magnezone_id, &k);

            events_shift(magnezones, magnezone_id, k);
        }else if (string_equals(command, "TWOSUM")){

            /*La forma mas simple de resolver esto es simplemente recorrer la lista 2 veces
            y encontrar los 2 maximos, pero una forma mas eficiente es en una sola pasada.
            
            Lo unico dificil de hacerlo en una sola pasada es que hay que hacer una triple comparación
            en cada paso. Entre el Maximo 1 actual, el Maximo 2 actual y el nodo que estamos revisando.
            Pueden pensarlo como que estamos ordenando este trio en cada paso*/

            int magnezone_id;
            fscanf(input_file, " %d", &magnezone_id);
            
            events_twosum(output_file, magnezones, magnezone_id);
        }
    }

    /* Liberamos memoria y cerramos archivos */

    for (int i = 0; i < S; i++) {
        magnezone_destroy(magnezones[i]);
    }

    fclose(input_file);
    fclose(output_file);

    return 0;
}
