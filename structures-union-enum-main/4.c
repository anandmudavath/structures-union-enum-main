
/*
 * Student Records and Academic Analytics System
 * Features: Add, Display, Search, Update, Delete,
 * GPA Summary, Sorting, Dynamic Memory Management.
 *
 * Edge cases to test:
 * Empty collection, duplicate roll number, invalid age/GPA,
 * long names, missing records, realloc failure and EOF.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>

#define NAME_LENGTH 50
#define MAX_STUDENTS 100000

typedef struct {
    int roll_no;
    char name[NAME_LENGTH];
    int age;
    float gpa;
} Student;

/* Read a complete line safely */
int read_line(const char *prompt, char *buffer, size_t size) {
    size_t length = 0;
    int ch;
    int too_long = 0;

    printf("%s", prompt);

    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (length + 1 < size) {
            buffer[length++] = (char)ch;
        } else {
            too_long = 1;
        }
    }

    if (ch == EOF && length == 0 && !too_long) {
        return 0;
    }

    buffer[length] = '\0';

    if (too_long) {
        return -1;
    }

    return 1;
}

/* Check whether only whitespace remains */
int only_spaces(const char *text) {
    while (*text != '\0') {
        if (!isspace((unsigned char)*text)) {
            return 0;
        }
        text++;
    }

    return 1;
}

/* Read and validate an integer */
int read_int_range(const char *prompt, int min, int max, int *value) {
    char input[128];
    char *end;
    long number;
    int result;

    while (1) {
        result = read_line(prompt, input, sizeof(input));

        if (result == 0) {
            return 0;
        }

        if (result == -1) {
            printf("Input is too long. Try again.\n");
            continue;
        }

        errno = 0;
        number = strtol(input, &end, 10);

        if (end == input || errno == ERANGE ||
            !only_spaces(end) || number < min || number > max) {
            printf("Invalid input. Enter a value between %d and %d.\n",
                   min, max);
            continue;
        }

        *value = (int)number;
        return 1;
    }
}

/* Read and validate a GPA */
int read_gpa(const char *prompt, float *value) {
    char input[128];
    char *end;
    float number;
    int result;

    while (1) {
        result = read_line(prompt, input, sizeof(input));

        if (result == 0) {
            return 0;
        }

        if (result == -1) {
            printf("Input is too long. Try again.\n");
            continue;
        }

        errno = 0;
        number = strtof(input, &end);

        if (end == input || errno == ERANGE ||
            !only_spaces(end) || !isfinite(number) ||
            number < 0.0f || number > 4.0f) {
            printf("Invalid GPA. Enter a value from 0.00 to 4.00.\n");
            continue;
        }

        *value = number;
        return 1;
    }
}

/* Read a student name */
int read_name(const char *prompt, char *name) {
    char input[NAME_LENGTH];
    char *start;
    size_t length;
    int result;

    while (1) {
        result = read_line(prompt, input, sizeof(input));

        if (result == 0) {
            return 0;
        }

        if (result == -1) {
            printf("Name must not exceed 49 characters.\n");
            continue;
        }

        start = input;

        while (isspace((unsigned char)*start)) {
            start++;
        }

        length = strlen(start);

        while (length > 0 &&
               isspace((unsigned char)start[length - 1])) {
            length--;
        }

        if (length == 0) {
            printf("Name cannot be empty.\n");
            continue;
        }

        memmove(name, start, length);
        name[length] = '\0';

        return 1;
    }
}

/* Allocate initial student array */
Student *create_student_list(size_t capacity) {
    if (capacity == 0 ||
        capacity > SIZE_MAX / sizeof(Student)) {
        return NULL;
    }

    return malloc(capacity * sizeof(Student));
}

/* Find a student using roll number */
int find_student_index(const Student *students,
                       size_t count, int roll_no) {
    size_t i;

    for (i = 0; i < count; i++) {
        if (students[i].roll_no == roll_no) {
            return (int)i;
        }
    }

    return -1;
}

/* Increase array capacity when required */
int expand_capacity(Student **students, size_t *capacity) {
    size_t new_capacity;
    Student *temp;

    if (*capacity >= MAX_STUDENTS) {
        printf("Maximum student capacity reached.\n");
        return 0;
    }

    if (*capacity > MAX_STUDENTS / 2) {
        new_capacity = MAX_STUDENTS;
    } else {
        new_capacity = *capacity * 2;
    }

    if (new_capacity > SIZE_MAX / sizeof(Student)) {
        printf("Memory size overflow.\n");
        return 0;
    }

    temp = realloc(*students, new_capacity * sizeof(Student));

    if (temp == NULL) {
        printf("Memory allocation failed. Existing data preserved.\n");
        return 0;
    }

    *students = temp;
    *capacity = new_capacity;

    printf("Capacity increased to %zu students.\n", *capacity);

    return 1;
}

/* Add a student */
int add_student(Student **students,
                size_t *count, size_t *capacity) {
    Student new_student;
    int result;

    if (!read_int_range("Enter roll number: ",
                       1, 2147483647, &new_student.roll_no)) {
        return 0;
    }

    if (find_student_index(*students, *count,
                           new_student.roll_no) != -1) {
        printf("Error: Roll number already exists.\n");
        return 1;
    }

    if (!read_name("Enter student name: ", new_student.name)) {
        return 0;
    }

    if (!read_int_range("Enter age: ", 16, 100,
                        &new_student.age)) {
        return 0;
    }

    if (!read_gpa("Enter GPA (0.00 - 4.00): ",
                  &new_student.gpa)) {
        return 0;
    }

    if (*count == *capacity) {
        result = expand_capacity(students, capacity);

        if (!result) {
            return 1;
        }
    }

    (*students)[*count] = new_student;
    (*count)++;

    printf("Student added successfully! Roll number: %d\n",
           new_student.roll_no);

    return 1;
}

/* Display one student */
void display_one_student(const Student *student) {
    printf("Roll No: %d | Name: %s | Age: %d | GPA: %.2f\n",
           student->roll_no,
           student->name,
           student->age,
           student->gpa);
}

/* Display all students */
void display_students(const Student *students, size_t count) {
    size_t i;

    if (count == 0) {
        printf("No student records available.\n");
        return;
    }

    printf("\n========== STUDENT RECORDS ==========\n");

    for (i = 0; i < count; i++) {
        display_one_student(&students[i]);
    }

    printf("Total students: %zu\n", count);
}

/* Search student */
int search_student(const Student *students, size_t count) {
    int roll_no;
    int index;

    if (!read_int_range("Enter roll number to search: ",
                        1, 2147483647, &roll_no)) {
        return 0;
    }

    index = find_student_index(students, count, roll_no);

    if (index == -1) {
        printf("Student not found.\n");
    } else {
        printf("\nStudent found:\n");
        display_one_student(&students[index]);
    }

    return 1;
}

/* Update student details */
int update_student(Student *students, size_t count) {
    int roll_no;
    int index;

    if (!read_int_range("Enter roll number to update: ",
                        1, 2147483647, &roll_no)) {
        return 0;
    }

    index = find_student_index(students, count, roll_no);

    if (index == -1) {
        printf("Student not found.\n");
        return 1;
    }

    printf("Enter updated details:\n");

    if (!read_name("Enter new name: ", students[index].name)) {
        return 0;
    }

    if (!read_int_range("Enter new age: ", 16, 100,
                        &students[index].age)) {
        return 0;
    }

    if (!read_gpa("Enter new GPA: ", &students[index].gpa)) {
        return 0;
    }

    printf("Student updated successfully!\n");

    return 1;
}

/* Delete student */
int delete_student(Student *students, size_t *count) {
    int roll_no;
    int index;

    if (!read_int_range("Enter roll number to delete: ",
                        1, 2147483647, &roll_no)) {
        return 0;
    }

    index = find_student_index(students, *count, roll_no);

    if (index == -1) {
        printf("Student not found.\n");
        return 1;
    }

    memmove(&students[index],
            &students[index + 1],
            (*count - (size_t)index - 1) * sizeof(Student));

    (*count)--;

    printf("Student deleted successfully!\n");

    return 1;
}

/* Display highest, lowest and average GPA */
void gpa_summary(const Student *students, size_t count) {
    size_t i;
    size_t highest = 0;
    size_t lowest = 0;
    double total = 0.0;

    if (count == 0) {
        printf("No records available for GPA analysis.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        total += students[i].gpa;

        if (students[i].gpa > students[highest].gpa) {
            highest = i;
        }

        if (students[i].gpa < students[lowest].gpa) {
            lowest = i;
        }
    }

    printf("\n========== GPA ANALYTICS ==========\n");

    printf("Highest GPA:\n");
    display_one_student(&students[highest]);

    printf("Lowest GPA:\n");
    display_one_student(&students[lowest]);

    printf("Average GPA: %.2f\n", total / count);
}

/* Compare students for descending GPA sorting */
int compare_gpa(const void *a, const void *b) {
    const Student *first = a;
    const Student *second = b;

    if (first->gpa > second->gpa) {
        return -1;
    }

    if (first->gpa < second->gpa) {
        return 1;
    }

    if (first->roll_no < second->roll_no) {
        return -1;
    }

    if (first->roll_no > second->roll_no) {
        return 1;
    }

    return 0;
}

/* Display students sorted by GPA without changing original array */
void sort_students(const Student *students, size_t count) {
    Student *sorted;
    
    if (count == 0) {
        printf("No records available for sorting.\n");
        return;
    }

    if (count > SIZE_MAX / sizeof(Student)) {
        printf("Memory size overflow.\n");
        return;
    }

    sorted = malloc(count * sizeof(Student));

    if (sorted == NULL) {
        printf("Unable to allocate memory for sorting.\n");
        return;
    }

    memcpy(sorted, students, count * sizeof(Student));

    qsort(sorted, count, sizeof(Student), compare_gpa);

    printf("\n========== STUDENTS SORTED BY GPA ==========\n");

    display_students(sorted, count);

    free(sorted);
}

/* Main function */
int main(void) {
    Student *student_list;
    size_t count = 0;
    size_t capacity;
    int initial_capacity;
    int choice;
    int running = 1;

    printf("========================================\n");
    printf("  STUDENT RECORDS AND ACADEMIC ANALYTICS\n");
    printf("========================================\n");

    if (!read_int_range("Enter initial capacity (1-100000): ",
                        1, MAX_STUDENTS, &initial_capacity)) {
        printf("Input ended. Exiting.\n");
        return 0;
    }

    capacity = (size_t)initial_capacity;

    student_list = create_student_list(capacity);

    if (student_list == NULL) {
        printf("Unable to allocate initial memory.\n");
        return 1;
    }

    while (running) {
        printf("\n========== MAIN MENU ==========\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. GPA Analytics\n");
        printf("7. Sort Students by GPA\n");
        printf("0. Exit\n");

        if (!read_int_range("Enter your choice: ",
                            0, 7, &choice)) {
            printf("\nInput ended. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                running = add_student(&student_list,
                                      &count, &capacity);
                break;

            case 2:
                display_students(student_list, count);
                break;

            case 3:
                running = search_student(student_list, count);
                break;

            case 4:
                running = update_student(student_list, count);
                break;

            case 5:
                running = delete_student(student_list, &count);
                break;

            case 6:
                gpa_summary(student_list, count);
                break;

            case 7:
                sort_students(student_list, count);
                break;

            case 0:
                printf("Exiting Student Management System.\n");
                running = 0;
                break;
        }
    }

    free(student_list);
    student_list = NULL;

    printf("Memory released successfully.\n");
    printf("Thank you for using the system!\n");

    return 0;
}
