//
// Created by elper on 11/21/2026.
//

#include "helper_functions.h"

#include <string.h>

#include "crud_functions.h"

// Checks if ID greater than 0
bool is_valid_id(int id) {
    if (id < 0) return false;
    return true;
}


// if last char is \0, the name is not valid
bool is_valid_name(char *name) {
    // position of the newline, or the end of the string
    size_t len = strcspn(name, "\n");
    if (name[len] == '\0') {
        return false;

    }
    return true;
}


// Checks to see if user between 1 and 120 inclusive
bool is_valid_age(int age) {
    if (age < 1 || age > 120) return false;
    return true;
}

// Checks to see if score is 0 to 100 inclusive
bool is_valid_score(int score) {
    if (score < 0 || score > 100) return false;
    return true;
}

// Clears the buffer with inputs that are not valid.
void clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int scanf_int(int *out) {
    int value;
    char after;
    int count = scanf("%d%c", &value, &after);

    if (count == 2 && after == '\n') {
        ungetc(after, stdin);       // put the newline back
        *out = value;
        return 1;
    }
    if (count == 1) {               // number, then end of input with no newline
        *out = value;
        return 1;
    }
    return -1;                      // letters, or text after the number, or end of input
}



// This will return the index of the student if it is found and -1 otherwise
int get_student_index(Student student_array[], int size, int id) {
    for (int i =0; i < size; i++) {
        if (student_array[i].id == id) {
            // Will only return student if they are active.
            if (student_array[i].active == 1) {
                return i;
            }
        }
    }
    return -1;
}


// Like scanf("%d", out), but also rejects anything typed after the number
// (for example 21.5 or 123abc).
// Returns 1 on success and -1 on bad input or end of input, so the existing
// checks (<= 0, != 1, < 0) all treat it as a failure. *out is only changed on success.
// On success the newline stays in the buffer, exactly like scanf("%d") leaves it.


