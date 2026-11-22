#include <stdio.h>
# include <stdbool.h>
#include <stdlib.h>

#include "crud_functions.h"
#include "structures.h"


int main(void) {
    // Start off with initial size of 25.
    int num_students = 0;

    // Define seperate variable since some students will be inactive.
    int elements_in_arr = 0;

    // Initial capacity.
    int student_arr_capacity = 25;
    Student *student_array = malloc(sizeof(Student) * student_arr_capacity);


    bool logged_in = true;
    FILE *fp;
    if ((fp = fopen("../students.dat", "rb+")) == NULL) {
        printf("Error opening file!\n");
    }
    else {
        // Load in data to struct array if there is any

        // Move to the end to get the number of bytes in file
        fseek(fp, 0, SEEK_END);

        // Tells us how many bytes in the file.
        const long size = (int)ftell(fp);
        if (size > 0) {
            // Total student structs is size divided by the size of student struct
            int num_structs = size / sizeof(Student);

            // Start at the beginning of the file
            rewind(fp);
            for (int i = 0; i < num_structs; i++) {
                // Basic flow: Create new student struct, load file data into struct, put struct in array, increase arr size
                Student student = {0};
                fread(&student, sizeof(Student), 1, fp);
                student_array[i] = student;
                elements_in_arr++;
                if (student.active == 1) {
                    num_students++;
                }

            }

        }
    }



    while (logged_in) {
        int choice;
        printf("Amount of students: %d\n", num_students);
        printf("\nStudent Record Management\n");
        printf("-------------------------\n");
        printf("1. Add Student\n");
        printf("2. Display all Students\n");
        printf("3. Search for a student\n");
        printf("4. Update a student\n");
        printf("5. Delete a student\n");
        printf("0. Exit\n\n");

        printf("Please enter a choice: ");
        int input = scanf("%d", &choice);

        if (input == 0) {
            printf("That is not a valid input. Please try again.\n");
            clear_buffer();
            continue;
        }

        if (choice == 0) {
            fclose(fp);
            free(student_array);
            printf("You are exiting the program\n");
            logged_in = false;
        }

        else {
            switch (choice) {
                case 1:
                    num_students += add_student(fp, student_array, elements_in_arr, student_arr_capacity);
                    break;
                case 2:
                    display_all_students(student_array, elements_in_arr);
                    break;
                case 3:
                    search_by_id(student_array, elements_in_arr);
                    break;
                case 4:
                    update_student(fp, student_array, elements_in_arr);
                    break;
                case 5:
                    num_students -= delete_student(fp, student_array, elements_in_arr);
                    break;
                default:
                    printf("Invalid choice! Please try again.\n");
                    break;
            }

        }


    }

    return 0;
}
