
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdalign.h>

#define TEXT_SIZE 20
#define RECORD_COUNT 5

/* Structure declaration */
typedef struct {
    int integer;
    float decimal;
    char text[TEXT_SIZE];
} StructData;

/* Union declaration */
typedef union {
    int integer;
    float decimal;
    char text[TEXT_SIZE];
} UnionData;

/* Enumeration declaration */
typedef enum {
    VALUE_INT,
    VALUE_FLOAT,
    VALUE_TEXT
} ValueType;

/* Tagged union declaration */
typedef struct {
    UnionData value;
    ValueType tag;
} TaggedRecord;

/* Set integer value */
int set_integer(TaggedRecord *record, int value) {
    if (record == NULL) {
        return 0;
    }

    record->value.integer = value;
    record->tag = VALUE_INT;
    return 1;
}

/* Set float value */
int set_float(TaggedRecord *record, float value) {
    if (record == NULL) {
        return 0;
    }

    record->value.decimal = value;
    record->tag = VALUE_FLOAT;
    return 1;
}

/* Set text value */
int set_text(TaggedRecord *record, const char *text) {
    if (record == NULL || text == NULL) {
        return 0;
    }

    size_t length = strlen(text);

    if (length >= TEXT_SIZE) {
        printf("Error: Text must contain at most 19 characters.\n");
        return 0;
    }

    strcpy(record->value.text, text);
    record->tag = VALUE_TEXT;
    return 1;
}

/* Display only the active union member */
void display_record(const TaggedRecord *record, int number) {
    if (record == NULL) {
        return;
    }

    printf("\nRecord %d: ", number);

    switch (record->tag) {
        case VALUE_INT:
            printf("Integer = %d\n", record->value.integer);
            break;

        case VALUE_FLOAT:
            printf("Float = %.2f\n", record->value.decimal);
            break;

        case VALUE_TEXT:
            printf("Text = %s\n", record->value.text);
            break;

        default:
            printf("Invalid tag. No value displayed.\n");
            break;
    }
}

/* Display structure and union memory information */
void memory_analysis(void) {
    StructData s = {10, 220.5f, "Union Programming"};
    UnionData u;

    memset(&u, 0, sizeof(u));

    printf("\n====================================\n");
    printf(" STRUCTURE AND UNION MEMORY ANALYSIS\n");
    printf("====================================\n");

    printf("\n--- STRUCTURE INFORMATION ---\n");

    printf("Size of StructData: %zu bytes\n", sizeof(StructData));
    printf("Alignment of StructData: %zu bytes\n",
           alignof(StructData));

    printf("Structure address: %p\n", (void *)&s);

    printf("Integer address: %p\n", (void *)&s.integer);
    printf("Float address: %p\n", (void *)&s.decimal);
    printf("Text address: %p\n", (void *)s.text);

    printf("\nStructure field offsets:\n");

    printf("Integer offset: %zu bytes\n",
           offsetof(StructData, integer));

    printf("Float offset: %zu bytes\n",
           offsetof(StructData, decimal));

    printf("Text offset: %zu bytes\n",
           offsetof(StructData, text));

    printf("\n--- UNION INFORMATION ---\n");

    printf("Size of UnionData: %zu bytes\n", sizeof(UnionData));
    printf("Alignment of UnionData: %zu bytes\n",
           alignof(UnionData));

    printf("Union address: %p\n", (void *)&u);

    printf("Integer address: %p\n", (void *)&u.integer);
    printf("Float address: %p\n", (void *)&u.decimal);
    printf("Text address: %p\n", (void *)u.text);

    printf("\nAll union members start at the same address.\n");

    printf("\n--- MEMORY COMPARISON ---\n");

    printf("Memory for 100 structures: %zu bytes\n",
           100 * sizeof(StructData));

    printf("Memory for 100 unions: %zu bytes\n",
           100 * sizeof(UnionData));

    printf("Memory saved using unions: %zu bytes\n",
           100 * (sizeof(StructData) - sizeof(UnionData)));

    printf("\n--- OBSERVATION ---\n");

    printf("Structure members occupy separate storage locations.\n");
    printf("Union members share the same memory location.\n");
    printf("Structure size may include padding for alignment.\n");
    printf("Union size is sufficient for its largest member,\n");
    printf("with any required alignment padding.\n");
}

/* Demonstrate tagged union functionality */
void tagged_union_demo(void) {
    TaggedRecord records[RECORD_COUNT] = {0};

    printf("\n====================================\n");
    printf(" TAGGED UNION RECORD SYSTEM\n");
    printf("====================================\n");

    /* Create five records */
    set_integer(&records[0], 10);
    set_float(&records[1], 220.5f);
    set_text(&records[2], "Union Programming");
    set_integer(&records[3], 500);
    set_text(&records[4], "C Language");

    printf("\n--- FIVE TAGGED RECORDS ---\n");

    for (int i = 0; i < RECORD_COUNT; i++) {
        display_record(&records[i], i + 1);
    }

    printf("\n--- TAGGED RECORD MEMORY ---\n");

    printf("Size of TaggedRecord: %zu bytes\n",
           sizeof(TaggedRecord));

    printf("Alignment of TaggedRecord: %zu bytes\n",
           alignof(TaggedRecord));

    printf("Size of five tagged records: %zu bytes\n",
           RECORD_COUNT * sizeof(TaggedRecord));

    printf("\n--- TYPE SWITCHING TEST ---\n");

    printf("Initially:\n");
    display_record(&records[0], 1);

    set_text(&records[0], "Changed to Text");
    printf("After changing to text:\n");
    display_record(&records[0], 1);

    set_float(&records[0], 99.75f);
    printf("After changing to float:\n");
    display_record(&records[0], 1);

    printf("\n--- STRING VALIDATION TEST ---\n");

    printf("Testing maximum valid string:\n");
    set_text(&records[0], "1234567890123456789");
    display_record(&records[0], 1);

    printf("\nTesting overlength string:\n");
    set_text(&records[0], "12345678901234567890");

    printf("\n--- INVALID TAG TEST ---\n");

    records[0].tag = (ValueType)99;
    display_record(&records[0], 1);
}

/* Main function */
int main(void) {
    printf("====================================\n");
    printf(" STRUCTURES AND UNIONS MEMORY UTILITY\n");
    printf("====================================\n");

    memory_analysis();

    tagged_union_demo();

    printf("\n====================================\n");
    printf(" PROGRAM COMPLETED SUCCESSFULLY\n");
    printf("====================================\n");

    return 0;
}



