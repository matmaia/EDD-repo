#pragma once

struct magnemite {
    int id;
    int atk;
    int speed;
    struct magnemite* next;
    //SOLUCION: agregado puntero prev
    struct magnemite* prev;
    
};

typedef struct magnemite Magnemite;

Magnemite* magnemite_create(int id, int atk, int speed);
void magnemite_destroy(Magnemite* target);
