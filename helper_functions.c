//
// Created by elper on 11/21/2026.
//

#include "helper_functions.h"

#include <string.h>

#include "crud_functions.h"

bool is_valid_id(int id) {
    if (id < 0) return false;
    return true;
}

bool is_valid_name(char *name) {
    // position of the newline, or the end of the string
    size_t len = strcspn(name, "\n");
    if (name[len] == '\0') {
        clear_buffer();
        return false;

    }
    return true;
}

bool is_valid_age(int age) {
    if (age < 1 || age > 120) return false;
    return true;
}

bool is_valid_score(int score) {
    if (score < 0 || score > 100) return false;
    return true;
}

void clear_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}
