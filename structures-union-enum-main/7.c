
/*
 * 7.c - Interactive Command-Driven Task Manager
 *
 * Concepts:
 * Structures, Enumerations, Dynamic Memory Allocation,
 * realloc, String Handling, Input Validation and Switch.
 *
 * Commands:
 * ADD, DELETE, VIEW, UPDATE, COMPLETE, FIND, HELP, EXIT
 *
 * Edge cases:
 * Duplicate IDs, invalid IDs, empty descriptions,
 * unknown commands, overlong input, EOF and allocation failure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

#define DESCRIPTION_SIZE 100
#define COMMAND_SIZE 32
#define INITIAL_CAPACITY 2

typedef enum {
    CMD_ADD,
    CMD_DELETE,
    CMD_VIEW,
    CMD_UPDATE,
    CMD_COMPLETE,
    CMD_FIND,
    CMD_HELP,
    CMD_EXIT,
    CMD_UNKNOWN
} CommandType;

typedef enum {
    PENDING,
    DONE
} TaskStatus;

typedef struct {
    int id;
    char description[DESCRIPTION_SIZE];
    TaskStatus status;
} Task;

typedef struct {
    Task *tasks;
    size_t count;
    size_t capacity;
} TaskList;

/* Read a bounded line from the user.
   Returns 1 on success, 0 on EOF, -1 on invalid input. */

int read_line(const char *prompt, char *buffer, size_t size)
{
    size_t length;
    int ch;

    printf("%s", prompt);
    fflush(stdout);

    if (fgets(buffer, (int)size, stdin) == NULL) {
        return 0;
    }

    length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';

        if (length > 1 && buffer[length - 2] == '\r') {
            buffer[length - 2] = '\0';
        }

        return 1;
    }

    if (!feof(stdin)) {
        ch = getchar();

        if (ch != '\n' && ch != EOF) {
            while (ch != '\n' && ch != EOF) {
                ch = getchar();
            }

            printf("Error: Input is too long.\n");
            return -1;
        }
    }

    return 1;
}

/* Convert command to uppercase. */

void convert_to_uppercase(char *text)
{
    size_t i;

    for (i = 0; text[i] != '\0'; i++) {
        text[i] = (char)toupper((unsigned char)text[i]);
    }
}

/* Convert command string into enumeration. */

CommandType parse_command(char *command)
{
    convert_to_uppercase(command);

    if (strcmp(command, "ADD") == 0) {
        return CMD_ADD;
    }

    if (strcmp(command, "DELETE") == 0) {
        return CMD_DELETE;
    }

    if (strcmp(command, "VIEW") == 0) {
        return CMD_VIEW;
    }

    if (strcmp(command, "UPDATE") == 0) {
        return CMD_UPDATE;
    }

    if (strcmp(command, "COMPLETE") == 0) {
        return CMD_COMPLETE;
    }

    if (strcmp(command, "FIND") == 0) {
        return CMD_FIND;
    }

    if (strcmp(command, "HELP") == 0) {
        return CMD_HELP;
    }

    if (strcmp(command, "EXIT") == 0) {
        return CMD_EXIT;
    }

    return CMD_UNKNOWN;
}

/* Read and validate a positive integer. */

int read_positive_id(const char *prompt, int *id)
{
    char buffer[64];
    char *end;
    long value;
    int result;

    result = read_line(prompt, buffer, sizeof(buffer));

    if (result != 1) {
        return result;
    }

    errno = 0;
    value = strtol(buffer, &end, 10);

    while (isspace((unsigned char)*end)) {
        end++;
    }

    if (buffer[0] == '\0' ||
        end == buffer ||
        *end != '\0' ||
        errno == ERANGE ||
        value <= 0 ||
        value > 2147483647L) {

        printf("Error: Enter a valid positive task ID.\n");
        return -1;
    }

    *id = (int)value;

    return 1;
}

/* Find a task by ID. */

int find_task_index(const TaskList *list, int id)
{
    size_t i;

    for (i = 0; i < list->count; i++) {
        if (list->tasks[i].id == id) {
            return (int)i;
        }
    }

    return -1;
}

/* Increase capacity when required. */

int reserve_capacity(TaskList *list)
{
    size_t new_capacity;
    Task *temp;

    if (list->count < list->capacity) {
        return 1;
    }

    if (list->capacity == 0) {
        new_capacity = INITIAL_CAPACITY;
    } else {
        if (list->capacity > SIZE_MAX / 2) {
            printf("Error: Capacity overflow.\n");
            return 0;
        }

        new_capacity = list->capacity * 2;
    }

    if (new_capacity > SIZE_MAX / sizeof(Task)) {
        printf("Error: Memory size overflow.\n");
        return 0;
    }

    temp = realloc(list->tasks, new_capacity * sizeof(Task));

    if (temp == NULL) {
        printf("Error: Unable to allocate memory.\n");
        return 0;
    }

    list->tasks = temp;
    list->capacity = new_capacity;

    printf("Capacity increased to %zu tasks.\n",
           list->capacity);

    return 1;
}

/* Add a new task. */

int add_task(TaskList *list)
{
    int id;
    int result;
    char description[DESCRIPTION_SIZE];

    printf("\n========== ADD TASK ==========\n");

    result = read_positive_id("Enter Task ID: ", &id);

    if (result != 1) {
        return result == 0 ? 0 : 1;
    }

    if (find_task_index(list, id) != -1) {
        printf("Error: Task ID already exists.\n");
        return 1;
    }

    result = read_line("Enter Task Description: ",
                       description, sizeof(description));

    if (result != 1) {
        return result == 0 ? 0 : 1;
    }

    if (description[0] == '\0') {
        printf("Error: Description cannot be empty.\n");
        return 1;
    }

    if (!reserve_capacity(list)) {
        return 1;
    }

    list->tasks[list->count].id = id;

    strcpy(list->tasks[list->count].description,
           description);

    list->tasks[list->count].status = PENDING;

    list->count++;

    printf("\nTask added successfully! ID: %d\n", id);

    return 1;
}

/* Display all tasks. */

void view_tasks(const TaskList *list)
{
    size_t i;

    printf("\n========== TASK LIST ==========\n");

    if (list->count == 0) {
        printf("No tasks available.\n");
        return;
    }

    printf("%-10s %-35s %-12s\n",
           "ID", "DESCRIPTION", "STATUS");

    printf("------------------------------------------------------------\n");

    for (i = 0; i < list->count; i++) {
        printf("%-10d %-35s %-12s\n",
               list->tasks[i].id,
               list->tasks[i].description,
               list->tasks[i].status == DONE
                   ? "DONE" : "PENDING");
    }

    printf("\nTotal tasks: %zu\n", list->count);
}

/* Search for a task. */

int find_task(TaskList *list)
{
    int id;
    int index;
    int result;

    printf("\n========== FIND TASK ==========\n");

    result = read_positive_id("Enter Task ID: ", &id);

    if (result != 1) {
        return result == 0 ? 0 : 1;
    }

    index = find_task_index(list, id);

    if (index == -1) {
        printf("Error: Task not found.\n");
        return 1;
    }

    printf("\nTask Found!\n");
    printf("ID: %d\n", list->tasks[index].id);
    printf("Description: %s\n",
           list->tasks[index].description);

    printf("Status: %s\n",
           list->tasks[index].status == DONE
               ? "DONE" : "PENDING");

    return 1;
}

/* Update task description. */

int update_task(TaskList *list)
{
    int id;
    int index;
    int result;
    char description[DESCRIPTION_SIZE];

    printf("\n========== UPDATE TASK ==========\n");

    result = read_positive_id("Enter Task ID: ", &id);

    if (result != 1) {
        return result == 0 ? 0 : 1;
    }

    index = find_task_index(list, id);

    if (index == -1) {
        printf("Error: Task not found.\n");
        return 1;
    }

    result = read_line("Enter New Description: ",
                       description, sizeof(description));

    if (result != 1) {
        return result == 0 ? 0 : 1;
    }

    if (description[0] == '\0') {
        printf("Error: Description cannot be empty.\n");
        return 1;
    }

    strcpy(list->tasks[index].description, description);

    printf("Task %d updated successfully!\n", id);

    return 1;
}

/* Mark task as completed. */

int complete_task(TaskList *list)
{
    int id;
    int index;
    int result;

    printf("\n========== COMPLETE TASK ==========\n");

    result = read_positive_id("Enter Task ID: ", &id);

    if (result != 1) {
        return result == 0 ? 0 : 1;
    }

    index = find_task_index(list, id);

    if (index == -1) {
        printf("Error: Task not found.\n");
        return 1;
    }

    if (list->tasks[index].status == DONE) {
        printf("Task %d is already completed.\n", id);
        return 1;
    }

    list->tasks[index].status = DONE;

    printf("Task %d marked as DONE!\n", id);

    return 1;
}

/* Delete a task and close the gap. */

int delete_task(TaskList *list)
{
    int id;
    int index;
    int result;
    size_t i;

    printf("\n========== DELETE TASK ==========\n");

    result = read_positive_id("Enter Task ID: ", &id);

    if (result != 1) {
        return result == 0 ? 0 : 1;
    }

    index = find_task_index(list, id);

    if (index == -1) {
        printf("Error: Task not found.\n");
        return 1;
    }

    for (i = (size_t)index; i + 1 < list->count; i++) {
        list->tasks[i] = list->tasks[i + 1];
    }

    list->count--;

    printf("Task %d deleted successfully!\n", id);

    return 1;
}

/* Display available commands. */

void show_help(void)
{
    printf("\n========== AVAILABLE COMMANDS ==========\n");

    printf("ADD       - Add a new task\n");
    printf("DELETE    - Delete a task by ID\n");
    printf("VIEW      - Display all tasks\n");
    printf("UPDATE    - Update task description\n");
    printf("COMPLETE  - Mark a task as completed\n");
    printf("FIND      - Search for a task by ID\n");
    printf("HELP      - Display available commands\n");
    printf("EXIT      - Exit the application\n");

    printf("========================================\n");
}

/* Main function. */

int main(void)
{
    TaskList list = {NULL, 0, 0};

    char command[COMMAND_SIZE];
    CommandType choice;
    int running = 1;
    int result;

    printf("\n");
    printf("========================================\n");
    printf("     INTERACTIVE TASK MANAGER\n");
    printf("========================================\n");

    printf("Dynamic Memory + Structures + Enums\n");

    show_help();

    while (running) {

        printf("\n----------------------------------------\n");

        result = read_line("Enter command: ",
                           command, sizeof(command));

        if (result == 0) {
            printf("\nInput ended. Exiting safely.\n");
            break;
        }

        if (result == -1) {
            continue;
        }

        choice = parse_command(command);

        switch (choice) {

            case CMD_ADD:
                if (!add_task(&list)) {
                    running = 0;
                }
                break;

            case CMD_DELETE:
                if (!delete_task(&list)) {
                    running = 0;
                }
                break;

            case CMD_VIEW:
                view_tasks(&list);
                break;

            case CMD_UPDATE:
                if (!update_task(&list)) {
                    running = 0;
                }
                break;

            case CMD_COMPLETE:
                if (!complete_task(&list)) {
                    running = 0;
                }
                break;

            case CMD_FIND:
                if (!find_task(&list)) {
                    running = 0;
                }
                break;

            case CMD_HELP:
                show_help();
                break;

            case CMD_EXIT:
                printf("\nExiting Task Manager...\n");
                running = 0;
                break;

            case CMD_UNKNOWN:
            default:
                printf("Error: Unknown command. Type HELP.\n");
                break;
        }
    }

    /* Release allocated memory. */

    free(list.tasks);
    list.tasks = NULL;
    list.count = 0;
    list.capacity = 0;

    printf("\nAllocated memory released successfully.\n");
    printf("Thank you for using Task Manager!\n");

    return 0;
}
