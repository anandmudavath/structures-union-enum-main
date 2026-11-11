
/*
 * Rectangle Inventory and Geometry Analysis
 * Structures, Dynamic Memory and Geometry
 *
 * Maximum rectangles: 1000
 * Square comparison tolerance: 1e-6 (relative tolerance)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_RECTANGLES 1000
#define NAME_BUFFER 128
#define SQUARE_TOLERANCE 1e-6f

typedef struct {
    int id;
    float width;
    float height;
    float area;
    float perimeter;
} Rectangle;

/* Read a complete line */
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

    if (ch == EOF && length == 0 && !too_long)
        return 0;

    buffer[length] = '\0';

    if (too_long)
        return -1;

    return 1;
}

/* Check trailing characters */
int only_spaces(const char *text) {
    while (*text) {
        if (!isspace((unsigned char)*text))
            return 0;
        text++;
    }
    return 1;
}

/* Read an integer within a range */
int read_integer(const char *prompt, int min, int max, int *value) {
    char input[NAME_BUFFER];
    char *end;
    long number;
    int result;

    while (1) {
        result = read_line(prompt, input, sizeof(input));

        if (result == 0)
            return 0;

        if (result == -1) {
            printf("Input too long. Try again.\n");
            continue;
        }

        errno = 0;
        number = strtol(input, &end, 10);

        if (end == input || errno == ERANGE ||
            !only_spaces(end) || number < min || number > max) {
            printf("Invalid input. Enter a value from %d to %d.\n",
                   min, max);
            continue;
        }

        *value = (int)number;
        return 1;
    }
}

/* Read a finite positive dimension */
int read_dimension(const char *prompt, float *value) {
    char input[NAME_BUFFER];
    char *end;
    float number;
    int result;

    while (1) {
        result = read_line(prompt, input, sizeof(input));

        if (result == 0)
            return 0;

        if (result == -1) {
            printf("Input too long. Try again.\n");
            continue;
        }

        errno = 0;
        number = strtof(input, &end);

        if (end == input || errno == ERANGE ||
            !only_spaces(end) || !isfinite(number) ||
            number <= 0.0f) {
            printf("Invalid dimension. Enter a finite positive number.\n");
            continue;
        }

        *value = number;
        return 1;
    }
}

/* Read a finite non-negative area threshold */
int read_threshold(float *value) {
    char input[NAME_BUFFER];
    char *end;
    float number;
    int result;

    while (1) {
        result = read_line("Enter area threshold: ",
                           input, sizeof(input));

        if (result == 0)
            return 0;

        if (result == -1) {
            printf("Input too long. Try again.\n");
            continue;
        }

        errno = 0;
        number = strtof(input, &end);

        if (end == input || errno == ERANGE ||
            !only_spaces(end) || !isfinite(number) ||
            number < 0.0f) {
            printf("Enter a finite non-negative threshold.\n");
            continue;
        }

        *value = number;
        return 1;
    }
}

/* Calculate area and perimeter safely */
int calculate_geometry(Rectangle *r) {
    float area;
    float perimeter;

    if (!isfinite(r->width) || !isfinite(r->height) ||
        r->width <= 0.0f || r->height <= 0.0f)
        return 0;

    area = r->width * r->height;
    perimeter = 2.0f * (r->width + r->height);

    if (!isfinite(area) || !isfinite(perimeter))
        return 0;

    r->area = area;
    r->perimeter = perimeter;

    return 1;
}

/* Create a dynamically allocated rectangle array */
Rectangle *create_rectangle_list(size_t capacity) {
    if (capacity == 0 ||
        capacity > SIZE_MAX / sizeof(Rectangle))
        return NULL;

    return malloc(capacity * sizeof(Rectangle));
}

/* Find rectangle by ID */
int find_rectangle(const Rectangle *rectangles,
                   size_t count, int id) {
    size_t i;

    for (i = 0; i < count; i++) {
        if (rectangles[i].id == id)
            return (int)i;
    }

    return -1;
}

/* Expand capacity using realloc */
int expand_capacity(Rectangle **rectangles,
                    size_t *capacity) {
    size_t new_capacity;
    Rectangle *temp;

    if (*capacity >= MAX_RECTANGLES) {
        printf("Maximum rectangle limit reached.\n");
        return 0;
    }

    if (*capacity > MAX_RECTANGLES / 2)
        new_capacity = MAX_RECTANGLES;
    else
        new_capacity = *capacity * 2;

    if (new_capacity > SIZE_MAX / sizeof(Rectangle)) {
        printf("Memory size overflow.\n");
        return 0;
    }

    temp = realloc(*rectangles,
                   new_capacity * sizeof(Rectangle));

    if (temp == NULL) {
        printf("Memory reallocation failed. Existing data preserved.\n");
        return 0;
    }

    *rectangles = temp;
    *capacity = new_capacity;

    printf("Capacity increased to %zu rectangles.\n", *capacity);

    return 1;
}

/* Read a complete rectangle */
int read_rectangle(Rectangle *r) {
    if (!read_integer("Enter rectangle ID: ",
                      1, 2147483647, &r->id))
        return 0;

    if (!read_dimension("Enter width: ", &r->width))
        return 0;

    if (!read_dimension("Enter height: ", &r->height))
        return 0;

    if (!calculate_geometry(r)) {
        printf("Dimensions produce an invalid area or perimeter.\n");
        return -1;
    }

    return 1;
}

/* Add a rectangle */
int add_rectangle(Rectangle **rectangles,
                  size_t *count, size_t *capacity) {
    Rectangle new_rectangle;
    int result;

    if (*count >= MAX_RECTANGLES) {
        printf("Maximum rectangle limit reached.\n");
        return 1;
    }

    result = read_rectangle(&new_rectangle);

    if (result == 0)
        return 0;

    if (result == -1)
        return 1;

    if (find_rectangle(*rectangles, *count,
                       new_rectangle.id) != -1) {
        printf("Error: Duplicate rectangle ID.\n");
        return 1;
    }

    if (*count == *capacity) {
        if (!expand_capacity(rectangles, capacity))
            return 1;
    }

    (*rectangles)[*count] = new_rectangle;
    (*count)++;

    printf("Rectangle added successfully!\n");
    return 1;
}

/* Display one rectangle */
void display_one(const Rectangle *r) {
    printf("ID: %d | Width: %.2f | Height: %.2f"
           " | Area: %.2f | Perimeter: %.2f\n",
           r->id, r->width, r->height,
           r->area, r->perimeter);
}

/* Display all rectangles */
void display_rectangles(const Rectangle *rectangles,
                        size_t count) {
    size_t i;

    if (count == 0) {
        printf("No rectangles available.\n");
        return;
    }

    printf("\n========== RECTANGLE INVENTORY ==========\n");

    for (i = 0; i < count; i++)
        display_one(&rectangles[i]);

    printf("Total rectangles: %zu\n", count);
}

/* Search rectangle by ID */
int search_rectangle(const Rectangle *rectangles,
                     size_t count) {
    int id;
    int index;

    if (!read_integer("Enter rectangle ID to search: ",
                      1, 2147483647, &id))
        return 0;

    index = find_rectangle(rectangles, count, id);

    if (index == -1) {
        printf("Rectangle not found.\n");
    } else {
        printf("Rectangle found:\n");
        display_one(&rectangles[index]);
    }

    return 1;
}

/* Update rectangle dimensions */
int update_rectangle(Rectangle *rectangles, size_t count) {
    int id;
    int index;
    float width, height;
    Rectangle updated;

    if (!read_integer("Enter rectangle ID to update: ",
                      1, 2147483647, &id))
        return 0;

    index = find_rectangle(rectangles, count, id);

    if (index == -1) {
        printf("Rectangle not found.\n");
        return 1;
    }

    updated = rectangles[index];

    if (!read_dimension("Enter new width: ", &width))
        return 0;

    if (!read_dimension("Enter new height: ", &height))
        return 0;

    updated.width = width;
    updated.height = height;

    if (!calculate_geometry(&updated)) {
        printf("Invalid dimensions or geometry overflow.\n");
        return 1;
    }

    rectangles[index] = updated;

    printf("Rectangle updated successfully!\n");
    return 1;
}

/* Delete rectangle by ID */
int delete_rectangle(Rectangle *rectangles, size_t *count) {
    int id;
    int index;

    if (!read_integer("Enter rectangle ID to delete: ",
                      1, 2147483647, &id))
        return 0;

    index = find_rectangle(rectangles, *count, id);

    if (index == -1) {
        printf("Rectangle not found.\n");
        return 1;
    }

    memmove(&rectangles[index],
            &rectangles[index + 1],
            (*count - (size_t)index - 1) * sizeof(Rectangle));

    (*count)--;

    printf("Rectangle deleted successfully!\n");
    return 1;
}

/* Find largest, smallest, total and average area */
void area_summary(const Rectangle *rectangles, size_t count) {
    size_t i;
    size_t largest = 0;
    size_t smallest = 0;
    double total = 0.0;

    if (count == 0) {
        printf("Collection is empty. No area analysis possible.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        total += rectangles[i].area;

        if (rectangles[i].area > rectangles[largest].area ||
            (rectangles[i].area == rectangles[largest].area &&
             rectangles[i].id < rectangles[largest].id))
            largest = i;

        if (rectangles[i].area < rectangles[smallest].area ||
            (rectangles[i].area == rectangles[smallest].area &&
             rectangles[i].id < rectangles[smallest].id))
            smallest = i;
    }

    printf("\n========== AREA SUMMARY ==========\n");

    printf("Largest area rectangle:\n");
    display_one(&rectangles[largest]);

    printf("Smallest area rectangle:\n");
    display_one(&rectangles[smallest]);

    printf("Total area: %.2f\n", total);
    printf("Average area: %.2f\n", total / count);
}

/* Check whether rectangle is a square */
int is_square(const Rectangle *r) {
    float scale = fmaxf(1.0f,
                        fmaxf(r->width, r->height));

    return fabsf(r->width - r->height)
           <= SQUARE_TOLERANCE * scale;
}

/* Classify all rectangles */
void classify_rectangles(const Rectangle *rectangles,
                         size_t count) {
    size_t i;

    if (count == 0) {
        printf("No rectangles available for classification.\n");
        return;
    }

    printf("\n========== SQUARE CLASSIFICATION ==========\n");

    for (i = 0; i < count; i++) {
        printf("ID: %d | Width: %.2f | Height: %.2f | ",
               rectangles[i].id,
               rectangles[i].width,
               rectangles[i].height);

        if (is_square(&rectangles[i]))
            printf("Square\n");
        else
            printf("Non-square\n");
    }
}

/* Compare rectangles by ascending area, then ID */
int compare_area(const void *a, const void *b) {
    const Rectangle *first = a;
    const Rectangle *second = b;

    if (first->area < second->area)
        return -1;

    if (first->area > second->area)
        return 1;

    if (first->id < second->id)
        return -1;

    if (first->id > second->id)
        return 1;

    return 0;
}

/* Display sorted rectangles without changing original order */
void sort_rectangles(const Rectangle *rectangles,
                     size_t count) {
    Rectangle *sorted;

    if (count == 0) {
        printf("No rectangles available for sorting.\n");
        return;
    }

    if (count > SIZE_MAX / sizeof(Rectangle)) {
        printf("Memory size overflow.\n");
        return;
    }

    sorted = malloc(count * sizeof(Rectangle));

    if (sorted == NULL) {
        printf("Unable to allocate sorting memory.\n");
        return;
    }

    memcpy(sorted, rectangles, count * sizeof(Rectangle));

    qsort(sorted, count, sizeof(Rectangle), compare_area);

    printf("\n========== SORTED RECTANGLES (ASCENDING AREA) ==========\n");

    display_rectangles(sorted, count);

    free(sorted);
}

/* Filter rectangles by area threshold */
int filter_rectangles(const Rectangle *rectangles,
                      size_t count) {
    float threshold;
    size_t i;
    int found = 0;

    if (!read_threshold(&threshold))
        return 0;

    printf("\n========== FILTERED RECTANGLES ==========\n");

    for (i = 0; i < count; i++) {
        if (rectangles[i].area > threshold) {
            display_one(&rectangles[i]);
            found = 1;
        }
    }

    if (!found)
        printf("No rectangle has area greater than %.2f.\n",
               threshold);

    return 1;
}

/* Display main menu */
void display_menu(void) {
    printf("\n========== RECTANGLE MANAGEMENT ==========\n");
    printf("1. Add Rectangle\n");
    printf("2. Display All Rectangles\n");
    printf("3. Search Rectangle by ID\n");
    printf("4. Update Rectangle Dimensions\n");
    printf("5. Delete Rectangle\n");
    printf("6. Area Summary\n");
    printf("7. Square / Non-square Classification\n");
    printf("8. Sort by Area (Ascending)\n");
    printf("9. Filter by Area Threshold\n");
    printf("0. Exit\n");
}

/* Main function */
int main(void) {
    Rectangle *rectangles;
    size_t count;
    size_t capacity;
    int initial_count;
    int choice;
    int i;
    int result;

    printf("============================================\n");
    printf(" RECTANGLE INVENTORY AND GEOMETRY ANALYSIS\n");
    printf("============================================\n");
    printf("Maximum allowed rectangles: %d\n",
           MAX_RECTANGLES);

    if (!read_integer("Enter initial number of rectangles: ",
                      1, MAX_RECTANGLES, &initial_count)) {
        printf("Input ended. Exiting.\n");
        return 0;
    }

    capacity = (size_t)initial_count;
    count = 0;

    rectangles = create_rectangle_list(capacity);

    if (rectangles == NULL) {
        printf("Initial memory allocation failed.\n");
        return 1;
    }

    /* Read initial rectangle records */
    for (i = 0; i < initial_count; i++) {
        Rectangle new_rectangle;

        printf("\nEnter details for rectangle %d:\n", i + 1);

        while (1) {
            result = read_rectangle(&new_rectangle);

            if (result == 0) {
                printf("Input ended. Releasing memory.\n");
                free(rectangles);
                return 0;
            }

            if (result == -1)
                continue;

            if (find_rectangle(rectangles, count,
                               new_rectangle.id) != -1) {
                printf("Duplicate ID. Enter a unique ID.\n");
                continue;
            }

            break;
        }

        rectangles[count++] = new_rectangle;
        printf("Rectangle stored successfully.\n");
    }

    /* Interactive menu */
    while (1) {
        display_menu();

        if (!read_integer("Enter your choice: ",
                          0, 9, &choice)) {
            printf("\nInput ended. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                if (!add_rectangle(&rectangles,
                                   &count, &capacity))
                    goto cleanup;
                break;

            case 2:
                display_rectangles(rectangles, count);
                break;

            case 3:
                if (!search_rectangle(rectangles, count))
                    goto cleanup;
                break;

            case 4:
                if (!update_rectangle(rectangles, count))
                    goto cleanup;
                break;

            case 5:
                if (!delete_rectangle(rectangles, &count))
                    goto cleanup;
                break;

            case 6:
                area_summary(rectangles, count);
                break;

            case 7:
                classify_rectangles(rectangles, count);
                break;

            case 8:
                sort_rectangles(rectangles, count);
                break;

            case 9:
                if (!filter_rectangles(rectangles, count))
                    goto cleanup;
                break;

            case 0:
                printf("Exiting Rectangle Management System.\n");
                goto cleanup;
        }
    }

cleanup:
    free(rectangles);
    rectangles = NULL;

    printf("Allocated memory released successfully.\n");
    printf("Thank you for using the system!\n");

    return 0;
}
