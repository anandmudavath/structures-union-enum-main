
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* =========================================
   1. ENUMERATION AND UNION
   ========================================= */

typedef enum {
    TYPE_EMPTY,
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_STRING
} DataType;

typedef union {
    int integer;
    float decimal;
    char character;
    char *string;
} Data;

typedef struct {
    Data data;
    DataType type;
} Container;


/* =========================================
   2. INITIALIZATION
   ========================================= */

void container_init(Container *c)
{
    if (c == NULL)
        return;

    memset(&c->data, 0, sizeof(c->data));
    c->type = TYPE_EMPTY;
}


/* =========================================
   3. CLEANUP FUNCTION
   ========================================= */

void container_clear(Container *c)
{
    if (c == NULL)
        return;

    if (c->type == TYPE_STRING)
    {
        free(c->data.string);
    }

    memset(&c->data, 0, sizeof(c->data));
    c->type = TYPE_EMPTY;
}


/* =========================================
   4. SETTER FUNCTIONS
   ========================================= */

int container_set_int(Container *c, int value)
{
    if (c == NULL)
        return 0;

    container_clear(c);

    c->data.integer = value;
    c->type = TYPE_INT;

    return 1;
}


int container_set_float(Container *c, float value)
{
    if (c == NULL)
        return 0;

    container_clear(c);

    c->data.decimal = value;
    c->type = TYPE_FLOAT;

    return 1;
}


int container_set_char(Container *c, char value)
{
    if (c == NULL)
        return 0;

    container_clear(c);

    c->data.character = value;
    c->type = TYPE_CHAR;

    return 1;
}


int container_set_string(Container *c, const char *value)
{
    char *new_string;
    size_t length;

    if (c == NULL || value == NULL)
        return 0;

    length = strlen(value);

    if (length == SIZE_MAX)
        return 0;

    new_string = (char *)malloc(length + 1);

    if (new_string == NULL)
    {
        printf("Error: Memory allocation failed.\n");
        return 0;
    }

    memcpy(new_string, value, length + 1);

    /* Allocate first, then release old memory.
       This also supports assigning from the
       container's existing string pointer. */

    container_clear(c);

    c->data.string = new_string;
    c->type = TYPE_STRING;

    return 1;
}


/* =========================================
   5. TYPE-SAFE GETTER FUNCTIONS
   ========================================= */

int container_get_int(const Container *c, int *out_value)
{
    if (c == NULL || out_value == NULL)
        return 0;

    if (c->type != TYPE_INT)
        return 0;

    *out_value = c->data.integer;

    return 1;
}


int container_get_float(const Container *c, float *out_value)
{
    if (c == NULL || out_value == NULL)
        return 0;

    if (c->type != TYPE_FLOAT)
        return 0;

    *out_value = c->data.decimal;

    return 1;
}


int container_get_char(const Container *c, char *out_value)
{
    if (c == NULL || out_value == NULL)
        return 0;

    if (c->type != TYPE_CHAR)
        return 0;

    *out_value = c->data.character;

    return 1;
}


int container_get_string(const Container *c,
                         const char **out_value)
{
    if (c == NULL || out_value == NULL)
        return 0;

    if (c->type != TYPE_STRING || c->data.string == NULL)
        return 0;

    *out_value = c->data.string;

    return 1;
}


/* =========================================
   6. TYPE NAME FUNCTION
   ========================================= */

const char *container_type_name(const Container *c)
{
    if (c == NULL)
        return "INVALID";

    switch (c->type)
    {
        case TYPE_EMPTY:
            return "EMPTY";

        case TYPE_INT:
            return "INTEGER";

        case TYPE_FLOAT:
            return "FLOAT";

        case TYPE_CHAR:
            return "CHARACTER";

        case TYPE_STRING:
            return "STRING";

        default:
            return "INVALID";
    }
}


/* =========================================
   7. PRINT FUNCTION
   ========================================= */

void container_print(const Container *c)
{
    if (c == NULL)
    {
        printf("Invalid container pointer.\n");
        return;
    }

    printf("Type: %-10s | Value: ",
           container_type_name(c));

    switch (c->type)
    {
        case TYPE_EMPTY:
            printf("No value stored");
            break;

        case TYPE_INT:
            printf("%d", c->data.integer);
            break;

        case TYPE_FLOAT:
            printf("%.2f", c->data.decimal);
            break;

        case TYPE_CHAR:
            printf("%c", c->data.character);
            break;

        case TYPE_STRING:
            if (c->data.string != NULL)
                printf("%s", c->data.string);
            else
                printf("(null)");
            break;

        default:
            printf("Invalid type tag");
            break;
    }

    printf("\n");
}


/* =========================================
   8. DEEP COPY FUNCTION
   ========================================= */

int container_copy(Container *destination,
                   const Container *source)
{
    Container temporary;

    if (destination == NULL || source == NULL)
        return 0;

    /* Self-copy is safe. */

    if (destination == source)
        return 1;

    container_init(&temporary);

    switch (source->type)
    {
        case TYPE_EMPTY:
            break;

        case TYPE_INT:
            container_set_int(
                &temporary,
                source->data.integer
            );
            break;

        case TYPE_FLOAT:
            container_set_float(
                &temporary,
                source->data.decimal
            );
            break;

        case TYPE_CHAR:
            container_set_char(
                &temporary,
                source->data.character
            );
            break;

        case TYPE_STRING:
            if (source->data.string == NULL)
                return 0;

            if (!container_set_string(
                    &temporary,
                    source->data.string))
            {
                return 0;
            }
            break;

        default:
            return 0;
    }

    /* Replace destination only after
       successful creation of the copy. */

    container_clear(destination);
    *destination = temporary;

    return 1;
}


/* =========================================
   9. DEMONSTRATION FUNCTION
   ========================================= */

void display_array(Container arr[], int size)
{
    int i;

    printf("\n========== CONTAINER ARRAY ==========\n");

    for (i = 0; i < size; i++)
    {
        printf("Index [%d] | ", i);
        container_print(&arr[i]);
    }

    printf("=====================================\n");
}


/* =========================================
   10. MAIN FUNCTION
   ========================================= */

int main(void)
{
    Container items[6];
    Container copied;

    int i;
    int integer_value;
    float float_value;
    char char_value;
    const char *string_value;

    printf("\n");
    printf("========================================\n");
    printf("   SAFE HETEROGENEOUS DATA CONTAINER\n");
    printf("========================================\n");

    /* Initialize all containers */

    for (i = 0; i < 6; i++)
    {
        container_init(&items[i]);
    }

    container_init(&copied);


    /* ---------------------------------
       TEST 1: STORE DIFFERENT DATA TYPES
       --------------------------------- */

    printf("\n--- TEST 1: SET DIFFERENT VALUES ---\n");

    container_set_int(&items[0], 10);
    container_set_float(&items[1], 3.14f);
    container_set_char(&items[2], 'A');
    container_set_string(&items[3], "Hello");
    container_set_int(&items[4], 0);
    container_set_string(&items[5], "Data");

    display_array(items, 6);


    /* ---------------------------------
       TEST 2: TYPE-SAFE GETTERS
       --------------------------------- */

    printf("\n--- TEST 2: TYPE-SAFE GETTERS ---\n");

    if (container_get_int(&items[0], &integer_value))
        printf("Integer value: %d\n", integer_value);

    if (container_get_float(&items[1], &float_value))
        printf("Float value: %.2f\n", float_value);

    if (container_get_char(&items[2], &char_value))
        printf("Character value: %c\n", char_value);

    if (container_get_string(&items[3], &string_value))
        printf("String value: %s\n", string_value);


    /* ---------------------------------
       TEST 3: VALID INTEGER ZERO
       --------------------------------- */

    printf("\n--- TEST 3: INTEGER ZERO ---\n");

    if (container_get_int(&items[4], &integer_value))
        printf("Zero is a valid integer: %d\n", integer_value);


    /* ---------------------------------
       TEST 4: WRONG TYPE GETTER
       --------------------------------- */

    printf("\n--- TEST 4: WRONG TYPE TEST ---\n");

    if (!container_get_int(&items[1], &integer_value))
        printf("Correctly rejected: Float is not an integer.\n");


    /* ---------------------------------
       TEST 5: DEEP COPY
       --------------------------------- */

    printf("\n--- TEST 5: DEEP COPY ---\n");

    if (container_copy(&copied, &items[5]))
    {
        printf("Original string: ");
        container_print(&items[5]);

        printf("Copied string: ");
        container_print(&copied);

        container_set_string(&items[5], "Changed");

        printf("\nAfter modifying original:\n");

        printf("Original: ");
        container_print(&items[5]);

        printf("Deep copy: ");
        container_print(&copied);
    }


    /* ---------------------------------
       TEST 6: TYPE TRANSITIONS
       STRING -> INT -> STRING -> FLOAT
       --------------------------------- */

    printf("\n--- TEST 6: TYPE TRANSITIONS ---\n");

    container_set_string(&items[0], "First");

    container_print(&items[0]);

    container_set_int(&items[0], 100);

    container_print(&items[0]);

    container_set_string(&items[0], "Second");

    container_print(&items[0]);

    container_set_float(&items[0], 9.5f);

    container_print(&items[0]);


    /* ---------------------------------
       TEST 7: EMPTY STRING
       --------------------------------- */

    printf("\n--- TEST 7: EMPTY STRING TEST ---\n");

    if (container_set_string(&items[3], ""))
    {
        printf("Empty string accepted successfully.\n");
        container_print(&items[3]);
    }


    /* ---------------------------------
       TEST 8: SAME SOURCE POINTER
       --------------------------------- */

    printf("\n--- TEST 8: SAME SOURCE POINTER ---\n");

    if (container_set_string(&items[3], "Memory Safe"))
    {
        const char *same_source = items[3].data.string;

        if (container_set_string(&items[3], same_source))
        {
            printf("Same source pointer handled safely.\n");
            container_print(&items[3]);
        }
    }


    /* ---------------------------------
       TEST 9: EMPTY CONTAINER
       --------------------------------- */

    printf("\n--- TEST 9: EMPTY CONTAINER ---\n");

    container_clear(&items[4]);

    container_print(&items[4]);


    /* ---------------------------------
       TEST 10: REPEATED CLEANUP
       --------------------------------- */

    printf("\n--- TEST 10: REPEATED CLEANUP ---\n");

    container_clear(&items[3]);
    container_clear(&items[3]);

    printf("Repeated cleanup completed safely.\n");

    container_print(&items[3]);


    /* ---------------------------------
       TEST 11: SELF COPY
       --------------------------------- */

    printf("\n--- TEST 11: SELF COPY ---\n");

    if (container_copy(&items[2], &items[2]))
        printf("Self-copy handled safely.\n");


    /* ---------------------------------
       TEST 12: FINAL ARRAY
       --------------------------------- */

    display_array(items, 6);


    /* ---------------------------------
       FINAL MEMORY CLEANUP
       --------------------------------- */

    printf("\n--- FINAL MEMORY CLEANUP ---\n");

    for (i = 0; i < 6; i++)
    {
        container_clear(&items[i]);
    }

    container_clear(&copied);

    printf("All containers cleared successfully.\n");
    printf("All allocated string memory released.\n");

    printf("\n========================================\n");
    printf("       PROGRAM COMPLETED SUCCESSFULLY\n");
    printf("========================================\n");

    return 0;
}

