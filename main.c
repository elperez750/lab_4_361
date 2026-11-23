#include <stdio.h>
# include <stdbool.h>
#include <stdlib.h>

#include "crud_functions.h"
#include "structures.h"


int main(void) {
    // This will count the number of active students
    int num_students = 0;

    // This will count all students, active or inactive
    int elements_in_arr = 0;

    // Initial capacity.
    int student_arr_capacity = 25;
    Student *student_array = malloc(sizeof(Student) * student_arr_capacity);



    bool logged_in = true;
    FILE *fp= fopen("../students.dat", "rb+");

    if (fp == NULL) {
        fp = fopen("students.dat", "wb");
    }
    else {
        // Move to the end to get the number of bytes in file
        fseek(fp, 0, SEEK_END);

        // Tells us how many bytes in the file.
        const long size = (int)ftell(fp);

        if (size > 0) {
            // Total student structs in the file
            int num_structs = size / sizeof(Student);

            // Case where the structs in the file exceed the initial capacity
            if (num_structs > student_arr_capacity) {
                Student *tmp = realloc(student_array, (num_structs * 2) * sizeof(Student));

                // Create tmp first, since reallocating could fail
                if (tmp == NULL) {
                    printf("Out of memory\n");
                    return 1;
                }

                // If space allocated, make student array the reallocated array and change capacity
                student_array = tmp;
                student_arr_capacity = num_structs * 2;
            }


            // Start at the beginning of the file
            rewind(fp);

            for (int i = 0; i < num_structs; i++) {
                // Basic flow: Create new student struct, load student data into struct, put struct in array, increase arr size
                Student student = {0};
                fread(&student, sizeof(Student), 1, fp);
                student_array[i] = student;
                elements_in_arr++;

                // Only count active students towards number of students
                if (student.active == 1) {
                    num_students++;
                }

            }

        }


    }


    // Displays the interface while the user does not exit
    while (logged_in) {
        int choice;
        printf("\nStudent Record Management\n");
        printf("-------------------------\n");
        printf("1. Add Student\n");
        printf("2. Display all Students\n");
        printf("3. Search for a student\n");
        printf("4. Update a student\n");
        printf("5. Delete a student\n");
        printf("0. Exit\n\n");

        printf("Please enter a choice: ");
        // Choice must be number from 0-5
        int input = scanf("%d", &choice);


        // Means we got something other than an int. Clear buffer and reprompt
        if (input == 0) {
            printf("That is not a valid input. Please try again.\n");
            clear_buffer();
            continue;
        }

        // User chooses to exit. Free the array and close file and set logged_in to false
        if (choice == 0) {
            fclose(fp);
            free(student_array);
            printf("You are exiting the program\n");
            logged_in = false;
        }


        // if choice was valid.
        else {
            switch (choice) {
                case 1: {
                    // Add student returns 1 if student was added, 0 if there was any issues.
                    int added = add_student(fp, &student_array, elements_in_arr, &student_arr_capacity);
                    num_students += added;
                    elements_in_arr += added;
                    break;
                }
                case 2: {
                    display_all_students(student_array, elements_in_arr);
                    break;
                }
                case 3: {
                    search_by_id(student_array, elements_in_arr);
                    break;
                }
                case 4: {
                    update_student(fp, student_array, elements_in_arr);
                    break;
                }
                case 5: {
                    // Same as add_students. Returns 0 if there was an issue or 1 if the student was deleted.
                    // Note. We only decrement num_students and not elements_in_arr. Since it is a logical delete, we will keep object in struct and file
                    num_students -= delete_student(fp, student_array, elements_in_arr);
                    break;
                }
                // If user types in int that is not 0-6
                default:
                    printf("Invalid choice! Please try again.\n");
                    break;
            }

        }


    }

    return 0;
}
