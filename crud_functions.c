//
// Created by elper on 11/20/2026.
//
#include "crud_functions.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "helper_functions.h"
#include "structures.h"

int add_student(FILE *fp, Student *student_array, int size, int max_size) {
        int id;
        char name[50];
        int age;
        int score;
        Student student = {0};

        printf("Please enter an ID for the student\n");
        int id_input = scanf("%d", &id);
        if (id_input == 0) {
            printf("That is not the correct input");
            return 0;
        }

    if (!is_valid_id(id)) {
        printf("That is not a valid ID");
        return 0;
    }


    clear_buffer();
    // Check if ID exists in struct
    if (get_id(student_array, size, id) > -1) {
        printf("Student ID already exists");
        return 0;
    };


    printf("Enter full name: ");
    fgets(name, sizeof name, stdin);          // reads up to 51 chars, always null-terminates
    if (!is_valid_name(name) ) {
        printf("Name too long (max %zu characters)\n", sizeof name - 1);
        return 0;
    }
    name[strcspn(name, "\n")] = '\0';         // strip the trailing newline


    printf("Please enter your age");
    if (scanf("%d", &age) != 1) {
        printf("That is not a valid age: ");
        clear_buffer();
        return 0;
    };

    if (!is_valid_age(age)) {
        printf("That age is not in the permitted range");
        return 0;
    }



    printf("Please enter your score: ");
    if (scanf("%d", &score) != 1) {
        printf("That is not a score");
        clear_buffer();
        return 0;
    }

    if (!is_valid_score(score)) {
        printf("That score is not in the permitted range.");
        return 0;
    }


    student.id = id;
    snprintf(student.name, sizeof student.name, "%s", name);
    student.age = age;
    student.score = score;
    student.active = 1;
    student_array[size -1] = student;
    fseek(fp, 0, SEEK_END);
    fwrite(&student_array[size -1], sizeof(Student), 1, fp);
    return 1;

}



size_t get_id(Student *student_array, int size, int id) {
    for (int i =0; i < size; i++) {
        if (student_array[i].id == id) {
            return i;
        }
    }
    return -1;

}

void display_all_students(Student student_array[], int size) {
    if (size == 0) {
        printf("There are no students to display.\n");
        return;
    }

    printf("\n%-6s %-25s %5s %6s %6s\n", "ID", "Name", "Age", "Score", "Active");
    printf("------ ------------------------- ----- ------\n");

    for (int i = 0; i < size; i++) {
        if (student_array[i].active == 1) {
            printf("%-6d %-25.25s %5d %6d %6d\n",
                   student_array[i].id,
                   student_array[i].name,
                   student_array[i].age,
                   student_array[i].score,
                   student_array[i].active
                   );
        }
    }

}

void search_by_id(Student student_array[], int size) {
    int id;
    printf("Please enter an ID for the student: ");
    int id_input = scanf("%d", &id);

    // This is to check if the ID is in our struct.
    if (id_input == 0) {
        printf("That is not the correct input");
        return;
    }


    bool found = false;
    int i;

    for (i = 0; i < size; i++) {
        if (student_array[i].id == id) {
            if (student_array[i].active == 1) {
                found = true;
                break;
            }
        }
    }

    if (!found) {
        printf("Student ID does not exist");
        return;
    }

    printf("\n%-6s %-25s %5s %6s\n", "ID", "Name", "Age", "Score");
    printf("------ ------------------------- ----- ------\n");
    printf("%-6d %-25.25s %5d %6d\n",
           student_array[i].id,
           student_array[i].name,
           student_array[i].age,
           student_array[i].score);
}

void update_student(FILE *fp, Student *student_array, int size) {
    int id;
    printf("Please enter an ID for the student: ");
    int id_input = scanf("%d", &id);

    // This is to check if the ID is in our struct.
    if (id_input == 0) {
        printf("That is not the correct input");
        return;
    }

    size_t student_index = get_id(student_array, size, id);
    if (student_index == -1) {
        printf("student ID does not exist");
        return;
    }


    bool in_progress = true;
    int input;
    while (in_progress) {

        printf("What would you like to change?\n");
        printf("1. Change name\n");
        printf("2. Change age\n");
        printf("3. Change score\n");

        if (scanf("%d", &input) > 0) {
            if (input < 1 || input > 3) {
                printf("Please enter a number from one to three\n");
                clear_buffer();
                continue;
            }
            clear_buffer();
            switch (input) {
                case 1: {
                    char name[50];
                    printf("Please enter a new name: ");
                    fgets(name, sizeof name, stdin);
                    if (!is_valid_name(name) ) {
                        printf("Name too long (max %zu characters)\n", sizeof name - 1);
                        continue;
                    }

                    name[strcspn(name, "\n")] = '\0';         // strip the trailing newline
                    snprintf(student_array[student_index].name, sizeof student_array[student_index].name, "%s", name);
                    fseek(fp, (student_index * sizeof(Student)), SEEK_SET);
                    fwrite(&student_array[student_index], sizeof(Student), 1, fp);
                    fflush(fp);
                    in_progress = false;
                    printf("Student name updated successfully!\n");
                    break;
                }

                case 2: {
                    int age;
                    printf("Please enter your age");
                    if (scanf("%d", &age) != 1) {
                        printf("That is not a valid age");
                        clear_buffer();
                    };
                    break;
                }

                case 3: {
                    int score;
                    printf("Please enter your score");
                    if (scanf("%d", &score) != 1) {
                        printf("That is not a score");
                        clear_buffer();
                    }
                    break;
                }
            }


        }
        else {
            printf("That is not a valid input. Please try again\n");
            clear_buffer();
        }

    }

}


int delete_student(FILE *fp, Student *student_array, int size) {
    int id;
    bool in_progress = true;

    while (in_progress) {


    printf("Please enter the ID of student you would like to delete: ");
    int id_input = scanf("%d", &id);

    if (id_input == 0) {
        printf("That is not the correct input");
        continue;
    }


    size_t student_index = get_id(student_array, size, id);

    if (student_index == -1) {
        printf("Student ID does not exist");
    }
    else {
        printf("\n%-6s %-25s %5s %6s\n", "ID", "Name", "Age", "Score");
        printf("------ ------------------------- ----- ------\n");
        printf("%-6d %-25.25s %5d %6d\n",
               student_array[student_index].id,
               student_array[student_index].name,
               student_array[student_index].age,
               student_array[student_index].score);


        bool valid = false;

        while (!valid) {
            int confirm_value;
            printf("Are you sure you want to delete the student?\n");
            printf("0. No\n");
            printf("1. Yes\n");
            int value = scanf("%d", &confirm_value);

            if (value == 0) {
                printf("That is not the correct input\n");
                clear_buffer();
                continue;
            }

            if (confirm_value == 1) {
                student_array[student_index].active = 0;
                fseek(fp, student_index * sizeof(Student), SEEK_SET);
                fwrite(&student_array[student_index], sizeof(Student), 1, fp);
                return 1;
            }

            printf("Nothing has been changed");

            valid = true;
            in_progress = false;
            clear_buffer();

        }

    }


    }
    return 0;


}








