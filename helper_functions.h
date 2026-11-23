//
// Created by elper on 11/21/2026.
//

#ifndef LAB_4_CS361_HELPER_FUNCTIONS_H
#define LAB_4_CS361_HELPER_FUNCTIONS_H
#include <stdbool.h>

#include "structures.h"


bool is_valid_age(int age);
bool is_valid_id(int id);
bool is_valid_score(int score);
bool is_valid_name(char *name);
void clear_buffer(void);
int get_student_index(Student student_array[], int size, int id);
int scanf_int(int *out);



#endif //LAB_4_CS361_HELPER_FUNCTIONS_H
