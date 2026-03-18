#include "magnemite.h"

#include <stdlib.h>

Magnemite* magnemite_create(int id, int atk, int speed) {
    Magnemite* new_magnemite = calloc(1, sizeof(Magnemite));

    new_magnemite->id = id;
    new_magnemite->atk = atk;
    new_magnemite->speed = speed;
    new_magnemite->next = NULL;
    //SOLUCION: agregado puntero prev
    new_magnemite->prev = NULL;

    return new_magnemite;
}

void magnemite_destroy(Magnemite* target) {
    free(target);
}
