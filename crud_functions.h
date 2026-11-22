//
// Created by elper on 11/20/2026.
//

#ifndef LAB_4_CS361_FUNCTIONS_H
#define LAB_4_CS361_FUNCTIONS_H
#include <stdbool.h>
#include <stdio.h>

#include "structures.h"


int add_student(FILE *fp, Student *student_array, int size, int max_size);
void display_all_students(Student student_array[], int size);
size_t get_id(Student *student_array, int size, int id);
void update_student(FILE *fp, Student *student_array, int size);
int delete_student(FILE *fp, Student *student_array, int size);
void clear_buffer(void);
void search_by_id(Student student_array[], int size);








#endif //LAB_4_CS361_FUNCTIONS_H
