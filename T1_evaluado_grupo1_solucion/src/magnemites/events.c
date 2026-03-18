#include "events.h"
#include "magnezone.h"

#include <stdlib.h>

//Las funciones del events solo llaman a las funciones definidas en magnezone y magnemite y hacen prints

void events_enter(FILE* output_file, MagneZone** magnezones, int magnezone_id, int magnemite_id, int magnemite_atk, int magnemite_speed) {
    MagneZone* magnezone = magnezones[magnezone_id];
    magnezone_insert(magnezone, magnemite_id, magnemite_atk, magnemite_speed);
    fprintf(output_file, "Magnemite %d ingreso a la manada %d.\n", magnemite_id, magnezone_id);
}

void events_find(FILE* output_file, MagneZone** magnezones, int magnezone_id, int target_atk) {
    MagneZone* magnezone = magnezones[magnezone_id];
    int count = magnezone_count_by_atk(magnezone, target_atk);

    fprintf(output_file, "MagneZone %d encontrados %d:\n", magnezone->id, count);
    if (count == 0) return;

    Magnemite** matches = magnezone_find_by_atk(magnezone, target_atk);
    for (int i = 0; i < count; i++) {
        fprintf(output_file, "    Magnemite %d\n", matches[i]->id);
    }

    free(matches);
}

void events_remove(FILE* output_file, MagneZone** magnezones, int n_magnezones, int magnemite_id) {
    bool removed = false;
    MagneZone* current_magnezone = NULL;
    for (int i = 0; i < n_magnezones; i++) {
        current_magnezone = magnezones[i];
        removed = magnezone_remove(current_magnezone, magnemite_id);
        if (removed) break;
    }

    fprintf(output_file, "Magnemite %d abandono a su magneZone.\n", magnemite_id);
}

void events_info(FILE* output_file, MagneZone** magnezones, int magnezone_id) {
    MagneZone* magnezone = magnezones[magnezone_id];
    int count = magnezone_length(magnezone);
    fprintf(output_file, "MagneZone %d tiene %d magnemites.\n", magnezone_id, count);
}

void events_inverse_1(FILE * output_file, MagneZone ** magnezones, int magnezone_id){

    MagneZone * magnezone = magnezones[magnezone_id];
    
    magnezone_invert_1(magnezone);
    fprintf(output_file, "MagneZone %d:\n", magnezone_id);

    Magnemite * current = magnezone->head;

    while (current != NULL){

        fprintf(output_file, "    Magnemite %d\n", current->id);
        current = current->next;
    }   
}

void events_inverse_2(FILE * output_file, MagneZone ** magnezones, int magnezone_id){

    MagneZone * magnezone = magnezones[magnezone_id];
    
    MagneZone * copy = magnezone_invert_2(magnezone);

    //no se nos puede olvidar actualizar el array de magnezones con la copia

    magnezones[magnezone_id] = copy;

    fprintf(output_file, "MagneZone %d:\n", magnezone_id);

    Magnemite * current = copy->head;


    while (current != NULL){

        fprintf(output_file, "    Magnemite %d\n", current->id);
        current = current->next;
    }   
}


void events_shift(MagneZone ** magnezones, int magnezone_id, int k){

    MagneZone * magnezone = magnezones[magnezone_id];

    magnezone_shift(magnezone, k);
}

void events_twosum(FILE * output_file, MagneZone ** magnezones, int magnezone_id){


    MagneZone * magnezone = magnezones[magnezone_id];

    Magnemite ** result = magnezone_twosum(magnezone);

    fprintf(output_file, "Magnemites %d y %d suman %d de velocidad.\n", result[0]->id, result[1]->id, 
                                                                        result[0]->speed + result[1]->speed);

    free(result);
}