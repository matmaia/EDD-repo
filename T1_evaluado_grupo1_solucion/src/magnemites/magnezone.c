#include "magnezone.h"

#include <stdio.h>
#include <stdlib.h>

MagneZone* magnezone_create(int id) {
    MagneZone* new_magnezone = calloc(1, sizeof(MagneZone));

    new_magnezone->id = id;
    new_magnezone->head = NULL;

    return new_magnezone;
}

void magnezone_destroy(MagneZone* target) {
    Magnemite* current_magnemite = target->head;
    while (current_magnemite != NULL) {
        Magnemite* next_magnemite = current_magnemite->next;
        magnemite_destroy(current_magnemite);
        current_magnemite = next_magnemite;
    }

    free(target);
}

int magnezone_length(MagneZone* magnezone) {
    int length = 0;
    Magnemite* current_magnemite = magnezone->head;
    while (current_magnemite != NULL) {
        length++;
        current_magnemite = current_magnemite->next;
    }

    return length;
}

void magnezone_print(MagneZone* magnezone) {
    printf("(%p) MagneZone %d (length: %d):\n", magnezone, magnezone->id, magnezone_length(magnezone));

    Magnemite* current_magnemite = magnezone->head;
    while (current_magnemite != NULL) {
        printf("|-(%p)-> Magnemite %d >> atk: %d | speed: %d\n", current_magnemite, current_magnemite->id, current_magnemite->atk, current_magnemite->speed);
        current_magnemite = current_magnemite->next;
    }
}

void magnezone_insert(MagneZone* magnezone, int magnemite_id, int magnemite_atk, int magnemite_speed) {
    Magnemite* new_magnemite = magnemite_create(magnemite_id, magnemite_atk, magnemite_speed);

    Magnemite* current_magnemite = magnezone->head;
    if (current_magnemite == NULL) {
        magnezone->head = new_magnemite;
        return;
    }

    while (current_magnemite->next != NULL) {
        current_magnemite = current_magnemite->next;
    }

    current_magnemite->next = new_magnemite;
    //SOLUCION: agregado puntero prev
    new_magnemite->prev = current_magnemite;
}

bool magnezone_remove(MagneZone* magnezone, int magnemite_id) {
    if (magnezone->head == NULL) return false;

    bool found = false;
    Magnemite* previous_magnemite = NULL;
    Magnemite* current_magnemite = magnezone->head;
    while (current_magnemite != NULL) {
        if (current_magnemite->id == magnemite_id) {
            found = true;
            break;
        }
        previous_magnemite = current_magnemite;
        current_magnemite = current_magnemite->next;
    }

    if (!found) return found;

    if (previous_magnemite == NULL) {
        magnezone->head = magnezone->head->next;
        //SOLUCION: agregado puntero prev
        
        if (magnezone->head != NULL){

            magnezone->head->prev = NULL;

        }

    } else {
        previous_magnemite->next = current_magnemite->next;
        //SOLUCION: agregado puntero prev
        if (current_magnemite->next != NULL) {
            current_magnemite->next->prev = previous_magnemite;
        }
    }

    magnemite_destroy(current_magnemite);
    return found;
}

int magnezone_count_by_atk(MagneZone* magnezone, int target_atk) {
    int count = 0;
    Magnemite* current_magnemite = magnezone->head;
    while (current_magnemite != NULL) {
        if (current_magnemite->atk == target_atk) {
            count++;
        }

        current_magnemite = current_magnemite->next;
    }

    return count;
}

Magnemite** magnezone_find_by_atk(MagneZone* magnezone, int target_atk) {
    int count = magnezone_count_by_atk(magnezone, target_atk);
    if (count == 0) return NULL;

    Magnemite** matches = calloc(count, sizeof(Magnemite*));

    int i = 0;
    Magnemite* current_magnemite = magnezone->head;
    while (current_magnemite != NULL) {
        if (current_magnemite->atk == target_atk) {
            matches[i] = current_magnemite;
            i++;
        }
        current_magnemite = current_magnemite->next;
    }

    return matches;
}

void magnezone_invert_1(MagneZone * magnezone){

    Magnemite * current = magnezone->head;

    while(current != NULL){

        //guardo el next antes de cambiar los punteros para no perder la referencia
        Magnemite * next = current->next;


        //intercambiamos los punteros
        Magnemite * temp = current->prev;
        current->prev = current->next;
        current->next = temp;

        if (next == NULL){
            //estamos en la cola actual, que al invertir es la nueva head
            magnezone->head = current;
        }

        current = next;
    }

}



Magnemite * magnezone_get_penultimate(MagneZone * magnezone){

    Magnemite * current = magnezone->head;

    while (current->next->next != NULL){
        current = current->next;
    }

    return current;
}

MagneZone * magnezone_invert_2(MagneZone * magnezone){

    if (magnezone->head->next == NULL){
       //la lista solo tiene un nodo, no hay nada que invertir, retornamos tal cual
        return magnezone;
    }
    //la copia tiene el mismo id
    MagneZone * copy = magnezone_create(magnezone->id);
    Magnemite * penultimate = magnezone_get_penultimate(magnezone);

    //No es lo mas eficiente pero por simplicidad, voy a usar la funcion insert que ya existe
    //esta va a crear un nuevo nodo magnemite, por lo que debemos borrar el anterior
    while (true){

        Magnemite * last = penultimate->next;
        
        //insertamos el ultimo creando una copia de este
        magnezone_insert(copy, last->id, last->atk, last->speed);

        //borramos el nodo anterior
        magnemite_destroy(last);
        penultimate->next = NULL;

        if (penultimate == magnezone->head){
            //el ultimo nodo que copie fue el siguiente a la cabeza,
            //solo me falta copiar la cabeza
            break;
        }
        penultimate = magnezone_get_penultimate(magnezone);

        

    }

    magnezone_insert(copy, penultimate->id, penultimate->atk, penultimate->speed);

    //Terminamos de copiar, destruimos lo que queda del magnezone anterior
    magnemite_destroy(penultimate);
    free(magnezone);

    return copy;

}

void magnezone_shift(MagneZone * magnezone, int k){

    if(magnezone->head->next == NULL) return; //solo hay un nodo

    int len = magnezone_length(magnezone);

    //esto lo hacemos ya que si k > len, la lista dara vueltas sobre si misma
    //con el % calculamos la cantidad de espacios que se mueve realmente la lista
    k = k % len;

    if (k == 0) return; //el shift deja igual a la lista

    //cola actual

    Magnemite * tail = magnezone->head;

    while(tail->next != NULL){

        tail = tail->next;
    }

    //la nueva cola se encuentra en la posicion len - k -1 partiendo desde la head

    Magnemite * new_tail = magnezone->head;

    for (int i = 0; i < len - k - 1; i++){

        new_tail = new_tail->next;
    }

    //ya que usé antes listas doblemente ligadas debo actualizar correctamente los punteros prev
    //ignorar esta parte si no usaron listas doblemente ligadas en su solucion
    magnezone->head->prev = tail;
    new_tail->next->prev = NULL;

    
    //ahora hacemos los pasos antes mencionados en el main

    tail->next = magnezone->head;  
    magnezone->head = new_tail->next;
    new_tail->next = NULL;

}

Magnemite ** magnezone_twosum(MagneZone * magnezone){

    //inicializamos
    Magnemite** result = calloc(2, sizeof(Magnemite*));
    Magnemite* max1 = magnezone->head;
    Magnemite* max2 = magnezone->head->next;

    // Ajustamos el orden inicial
    if (max2->speed > max1->speed) {
        max1 = magnezone->head->next;
        max2 = magnezone->head;
    }

    // Empezamos a evaluar desde el tercer nodo en adelante
    Magnemite* current = magnezone->head->next->next;

    while (current != NULL) {
        // Le gana estrictamente a max1?
        if (current->speed > max1->speed) {
            // El campeón antiguo baja a subcampeón
            max2 = max1;
            max1 = current;
        } 
        // No le ganó al max1, pero le gana a max2
        else if (current->speed > max2->speed) {
            max2 = current;
        }

        current = current->next;
    }

    result[0] = max1;
    result[1] = max2;

    return result;
}