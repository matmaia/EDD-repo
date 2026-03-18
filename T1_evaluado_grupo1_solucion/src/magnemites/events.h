#pragma once

#include "magnezone.h"

#include <stdio.h>

void events_enter(FILE* output_file, MagneZone** magnezones, int magnezone_id, int magnemite_id, int magnemite_atk, int magnemite_speed);
void events_find(FILE* output_file, MagneZone** magnezones, int magnezone_id, int target_atk);
void events_remove(FILE* output_file, MagneZone** magnezones, int n_magnezones, int magnemite_id);
void events_info(FILE* output_file, MagneZone** magnezones, int magnezone_id);
void events_inverse_1(FILE * output_file, MagneZone ** magnezones, int magnezone_id);
void events_inverse_2(FILE * output_file, MagneZone ** magnezones, int magnezone_id);
void events_shift(MagneZone ** magnezones, int magnezone_id, int k);
void events_twosum(FILE * output_file, MagneZone ** magnezones, int magnezone_id);