#pragma once

#include "magnemite.h"

#include <stdbool.h>

typedef struct magnezone {
    int id;
    Magnemite* head;
} MagneZone;

MagneZone* magnezone_create(int id);
void magnezone_destroy(MagneZone* target);
int magnezone_length(MagneZone* magnezone);
void magnezone_print(MagneZone* magnezone);
void magnezone_insert(MagneZone* magnezone, int magnemite_id, int magnemite_atk, int magnemite_speed);
bool magnezone_remove(MagneZone* magnezone, int magnemite_id);
int magnezone_count_by_atk(MagneZone* magnezone, int target_atk);
Magnemite** magnezone_find_by_atk(MagneZone* magnezone, int target_atk);
void magnezone_invert_1(MagneZone * magnezone);
MagneZone * magnezone_invert_2(MagneZone * magnezone);
void magnezone_shift(MagneZone * magnezone, int k);
Magnemite ** magnezone_twosum(MagneZone * magnezone);