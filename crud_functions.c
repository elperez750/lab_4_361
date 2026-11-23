//
// Created by elper on 11/20/2026.
//
#include "crud_functions.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper_functions.h"
#include "structures.h"


// Adds student to file and array
int add_student(FILE *fp, Student **student_array, int size, int *max_size) {
    // Declaring our student struct fields
    int id;
    char name[50];
    int age;
    int score;
    Student student = {0};

    // For reprompting user if they enter bad data.
    bool valid_id_input = false;
    bool valid_name_input = false;
    bool valid_age_input = false;
    bool valid_score_input = false;


    while (!valid_id_input) {
        printf("Please enter an ID for the student: ");
        int id_input = scanf_int(&id);
        // If we do not get an integer
        if (id_input <= 0) {
            printf("That is not the correct input");
            clear_buffer();
            continue;
        }

        // Check validity of ID by checking it is greater than 0.
        if (!is_valid_id(id)) {
            printf("That is not a valid ID");
            clear_buffer();
            continue;
        }

        clear_buffer();

        // Check if ID exists in struct
        if (get_student_index(*student_array, size, id) > -1) {
            printf("Student ID already exists. Please try again\n");
            continue;
        };

        valid_id_input = true;
    }


    // will run until we get a proper name
    while (!valid_name_input) {
        printf("Enter full name: ");
        // reads up to 51 chars, always null-terminates
        fgets(name, sizeof name, stdin);
        if (!is_valid_name(name)) {
            printf("Name too long (max %zu characters)\n", sizeof name - 1);
            clear_buffer();
            continue;
        }
        // strip the trailing newline
        name[strcspn(name, "\n")] = '\0';
        valid_name_input = true;
    }

    // Will run until we get a proper age
    while (!valid_age_input) {
        printf("Please enter your age: ");
        // If we get anything other than int
        if (scanf_int(&age) != 1) {
            printf("That is not a valid age\n");
            clear_buffer();
            continue;
        };

        // Checks if age is between 1 and 120 inclusive
        if (!is_valid_age(age)) {
            printf("That age is not in the permitted range\n");
            clear_buffer();
            continue;
        }

        valid_age_input = true;
        clear_buffer();
    }

    // Will run until we get valid score
    while (!valid_score_input) {
        printf("Please enter your score: ");
        // If score is not integer
        if (scanf_int(&score) != 1) {
            printf("That is not a score\n");
            clear_buffer();
            continue;
        }

        // Checks if score between 0 and 100 inclusive
        if (!is_valid_score(score)) {
            printf("That score is not in the permitted range.\n");
            clear_buffer();
            continue;
        }
        valid_score_input = true;
        clear_buffer();
    }

    // Set struct fields with the valid data.
    student.id = id;
    snprintf(student.name, sizeof student.name, "%s", name);
    student.age = age;
    student.score = score;
    student.active = 1;

    // if we have reached capacity, reallocate memory.
    // We pass in pointer for max_size to modify it incase we have to resize
    // The same is true for the array. We pass pointer to its address incase we
    // reassign it.
    if (size == *max_size) {
        Student *tmp = realloc(*student_array, *max_size * 2 * sizeof(Student));
        if (tmp == NULL) {
            printf("Out of memory\n");
            return 0;
        }
        *student_array = tmp;
        *max_size *= 2;
    }

    // insert new student in the last position
    (*student_array)[size] = student;
    // Go to end of the file
    fseek(fp, 0, SEEK_END);
    // Write the struct we just inserted
    fwrite(&(*student_array)[size], sizeof(Student), 1, fp);
    // Update changes immediately
    fflush(fp);
    return 1;
}

// Prints all students currently in the struct
void display_all_students(Student student_array[], int size) {
    if (size == 0) {
        printf("There are no students to display.\n");
        return;
    }

    printf("\n%-6s %-25s %5s %6s %6s\n", "ID", "Name", "Age", "Score",
           "Active");
    printf("------ ------------------------- ----- ------\n");

    for (int i = 0; i < size; i++) {
        if (student_array[i].active == 1) {
            printf("%-6d %-25.25s %5d %6d %6d\n", student_array[i].id,
                   student_array[i].name, student_array[i].age,
                   student_array[i].score, student_array[i].active);
        }
    }
}

void search_by_id(Student student_array[], int size) {
    int id;
    bool has_valid_input = false;

    // Prompts user until valid ID is given
    while (!has_valid_input) {
        printf("Please enter an ID for the student: ");
        int id_input = scanf_int(&id);

        // Will check to see if we do not get the right type
        if (id_input <= 0) {
            printf("That is not a valid input for ID\n");
            clear_buffer();
            continue;
        }

        // If we get an int, we check to see if the ID is valid
        if (!is_valid_id(id)) {
            printf("This is not a valid ID!");
            clear_buffer();
            continue;
        }
        has_valid_input = true;
    }

    const int i = get_student_index(student_array, size, id);
    if (i == -1) {
        printf("Student ID does not exist\n");
        return;
    }

    printf("\n%-6s %-25s %5s %6s\n", "ID", "Name", "Age", "Score");
    printf("------ ------------------------- ----- ------\n");
    printf("%-6d %-25.25s %5d %6d\n", student_array[i].id,
           student_array[i].name, student_array[i].age, student_array[i].score);
}

// This will update either name, age, or score in the struct and the file.
void update_student(FILE *fp, Student *student_array, int size) {
    int id;
    bool has_valid_input = false;

    while (!has_valid_input) {
        printf("Please enter an ID for the student: ");
        int id_input = scanf_int(&id);
        // Checks if ID is int.
        if (id_input <= 0) {
            printf("That is not the correct input");
            clear_buffer();
            continue;
        }

        if (!is_valid_id(id)) {
            printf("This is not a valid ID!");
            clear_buffer();
            continue;
        }
        has_valid_input = true;
    }

    clear_buffer();


    // Checks if ID in the struct
    int student_index = get_student_index(student_array, size, id);
    if (student_index == -1) {
        printf("student ID does not exist");
        return;
    }


    bool in_progress = true;
    while (in_progress) {
        char name[50];
        int age;
        int score;
        bool has_valid_name = false;

        while (!has_valid_name) {
            printf("Please enter a new name: ");
            fgets(name, sizeof name, stdin);
            if (!is_valid_name(name)) {
                printf("Name too long (max %zu characters)\n", sizeof name - 1);
                clear_buffer();
                continue;
            }

            // strip the trailing newline
            name[strcspn(name, "\n")] = '\0';
            has_valid_name = true;
        }

        // Updating age.
        bool has_valid_age = false;
        while (!has_valid_age) {
            printf("Please enter your age: ");
            if (scanf_int(&age) != 1) {
                printf("That is not a valid age\n");
                clear_buffer();
                continue;
            }

            if (!is_valid_age(age)) {
                printf("That is not a valid age\n");
                clear_buffer();
                continue;
            }
            has_valid_age = true;
        }

        bool has_valid_score = false;
        while (!has_valid_score) {
            printf("Please enter your score: ");
            if (scanf_int(&score) != 1) {
                printf("That is not a score\n");
                clear_buffer();
            } else {
                if (!is_valid_score(score)) {
                    printf("That is not a valid score");
                    clear_buffer();
                    continue;
                }
                has_valid_score = true;
            }
        }

        // Update name, age, and score in the struct
        snprintf(student_array[student_index].name,sizeof student_array[student_index].name, "%s", name);
        student_array[student_index].age = age;
        student_array[student_index].score = score;

        // Find the position in the file by multiplying index by size of the struct
        fseek(fp, (student_index * sizeof(Student)), SEEK_SET);
        // Write the new updated struct in that position
        fwrite(&student_array[student_index], sizeof(Student), 1, fp);
        fflush(fp);

        printf("Student has been successfully updated!\n");
        in_progress = false;
    }

}



// Deletes a student from struct and file
int delete_student(FILE *fp, Student *student_array, int size) {
    int id;
    bool in_progress = true;

    while (in_progress) {
        printf("Please enter the ID of student you would like to delete: ");
        int id_input = scanf_int(&id);

        // If it gets something other than int
        if (id_input <= 0) {
            printf("That is not the correct input\n");
            clear_buffer();
            continue;
        }

        // Checks if ID is valid. (Non negative)
        if (!is_valid_id(id)) {
            printf("That is not a valid id\n");
            clear_buffer();
            continue;
        }

        // Checks to see if we have the index
        int student_index = get_student_index(student_array, size, id);

        if (student_index == -1) {
            printf("Student ID does not exist\n");
            break;
        }

        // Prints the student information to the screen
        else {
            printf("\n%-6s %-25s %5s %6s\n", "ID", "Name", "Age", "Score");
            printf("------ ------------------------- ----- ------\n");
            printf("%-6d %-25.25s %5d %6d\n", student_array[student_index].id,
                   student_array[student_index].name,
                   student_array[student_index].age,
                   student_array[student_index].score);

            bool valid = false;
            while (!valid) {
                int confirm_value;
                printf("\nAre you sure you want to delete the student?\n");
                printf("0. No\n");
                printf("1. Yes\n");
                int value = scanf_int(&confirm_value);

                // Did not get integer
                if (value < 0) {
                    printf("That is not the correct input\n");
                    clear_buffer();
                    continue;
                }

                // Did not get value in range
                if (confirm_value > 1) {
                    printf("That is not a valid number. Please try again\n");
                    clear_buffer();
                    continue;
                }

                // User confirms deletion
                if (confirm_value == 1) {

                    // Since it is a logical deletion, simply make active = 0
                    student_array[student_index].active = 0;
                    fseek(fp, student_index * sizeof(Student), SEEK_SET);
                    fwrite(&student_array[student_index], sizeof(Student), 1,
                           fp);
                    fflush(fp);
                    return 1;
                }

                // If user selects 0, do not delete user.
                printf("Nothing has been done\n");

                valid = true;
                in_progress = false;
                clear_buffer();
            }
        }
    }
    return 0;
}