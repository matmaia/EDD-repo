#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Magnemite {
    int id;
    int atk;
    int speed;
    struct Magnemite* next;
} Magnemite;

typedef struct {
    int id;
    Magnemite* head;
    int count;
} MagneZone;

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

// Crear un nuevo magnemite
Magnemite* create_magnemite(int id, int atk, int speed) {
    Magnemite* new = (Magnemite*)malloc(sizeof(Magnemite));
    new->id = id;
    new->atk = atk;
    new->speed = speed;
    new->next = NULL;
    return new;
}

// Buscar un magnemite por ID en una manada específica
Magnemite* find_magnemite_by_id(MagneZone* zone, int id) {
    Magnemite* current = zone->head;
    while (current != NULL) {
        if (current->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Eliminar un magnemite de una manada
bool remove_magnemite(MagneZone* zone, int id) {
    Magnemite* current = zone->head;
    Magnemite* prev = NULL;
    
    while (current != NULL) {
        if (current->id == id) {
            if (prev == NULL) {
                zone->head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            zone->count--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

int main(int argc, char **argv) {
    check_arguments(argc, argv);
    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");
    
    if (input_file == NULL || output_file == NULL) {
        printf("Error opening files\n");
        return 1;
    }
    
    int S, E;
    
    if (fscanf(input_file, "%d", &S) != 1) {
        printf("Error reading maximum number of magnezones\n");
        return 1;
    }
    
    if (fscanf(input_file, "%d", &E) != 1) {
        printf("Error reading number of events\n");
        return 1;
    }
    
    // Inicializar los MagneZones
    MagneZone** zones = (MagneZone**)malloc(S * sizeof(MagneZone*));
    for (int i = 0; i < S; i++) {
        zones[i] = (MagneZone*)malloc(sizeof(MagneZone));
        zones[i]->id = i;
        zones[i]->head = NULL;
        zones[i]->count = 0;
    }
    
    char command[32];
    
    for (int i = 0; i < E; i++) {
        if (fscanf(input_file, "%s", command) != 1) {
            printf("Error reading event\n");
            break;
        }
        
        if (string_equals(command, "ENTER")) {
            int zone_id, magnemite_id, atk, speed;
            fscanf(input_file, "%d %d %d %d", &zone_id, &magnemite_id, &atk, &speed);
            
            Magnemite* new = create_magnemite(magnemite_id, atk, speed);
            new->next = zones[zone_id]->head;
            zones[zone_id]->head = new;
            zones[zone_id]->count++;
            
            fprintf(output_file, "Magnemite %d ingreso a la manada %d.\n", 
                    magnemite_id, zone_id);
        }
        else if (string_equals(command, "FIND")) {
            int zone_id, atk;
            fscanf(input_file, "%d %d", &zone_id, &atk);
            
            int count = 0;
            Magnemite* current = zones[zone_id]->head;
            while (current != NULL) {
                if (current->atk == atk) count++;
                current = current->next;
            }
            
            fprintf(output_file, "MagneZone %d encontrados %d:\n", zone_id, count);
            
            current = zones[zone_id]->head;
            while (current != NULL) {
                if (current->atk == atk) {
                    fprintf(output_file, "Magnemite %d\n", current->id);
                }
                current = current->next;
            }
        }
        else if (string_equals(command, "REMOVE")) {
            int magnemite_id;
            fscanf(input_file, "%d", &magnemite_id);
            
            int zone_id = -1;
            for (int j = 0; j < S; j++) {
                if (find_magnemite_by_id(zones[j], magnemite_id) != NULL) {
                    zone_id = j;
                    break;
                }
            }
            
            if (zone_id != -1) {
                remove_magnemite(zones[zone_id], magnemite_id);
                fprintf(output_file, "Magnemite %d abandono a su magneZone.\n", magnemite_id);
            }
        }
        else if (string_equals(command, "INFO")) {
            int zone_id;
            fscanf(input_file, "%d", &zone_id);
            
            fprintf(output_file, "MagneZone con id %d tiene %d magnemites.\n", 
                    zone_id, zones[zone_id]->count);
        }
    }
    
    // Liberar memoria
    for (int i = 0; i < S; i++) {
        Magnemite* current = zones[i]->head;
        while (current != NULL) {
            Magnemite* temp = current;
            current = current->next;
            free(temp);
        }
        free(zones[i]);
    }
    free(zones);
    
    fclose(input_file);
    fclose(output_file);
    
    return 0;
}