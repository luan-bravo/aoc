#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#define ARRAY_LENGTH(arr) (sizeof(arr) / sizeof((arr)[0]))
#define NUMSTR_MAX_DIGITS 10
#define TABLE_SIZE 1000


typedef struct {
    unsigned int value;
    unsigned int appearances;
} item;


void printArray(item *table)
{
    size_t i = 0;
    for (; i < TABLE_SIZE && (table[i].value != 0 && table[i].appearances != 0); i++) {
        // printf("num: %d\nappears: %d\n", table[i].key, table[i].appearances);
    }
    size_t j = i;
    for (; j < TABLE_SIZE; j++);
    printf("%lu populated items with %lu left\n", i, j-i);
    printf("\n");
    return;
}

int prime(int num)
{

    // All prime numbers are odd except two
    if (num & 1) {
        num -= 2;
    } else {
        num--;
    }

    int i, j;
    for (i = num; i >= 2; i -= 2) {
        if (i % 2 == 0) {
            continue;
        }
        for (j = 3; j <= sqrt(i); j += 2) {
            if (i % j == 0) {
                break;
            }
        }
        if (j > sqrt(i)) {
            return i;
        }
    }
    // It will only be executed when n is 3
    return 2;
}

unsigned int hash(item *table, int num)
{
    assert(table != NULL);
    return num * sqrt(TABLE_SIZE) + num % TABLE_SIZE;
}

item findItem(item* table, int num)
{

}

void addAppearence(item *table, int num)
{
    int key = hash(table, num);
    // find num index of table or empty index in table
    // if num found in table, mark its appearance. Else, add it to the end of the table
    if (table[key].value == num && table[key].appearances > 0) {
        table[key].appearances += 1;
    } else {
        table[key].value = num;
        table[key].appearances += 1;
    }
    return;
}


// Given that all lines have the same structure, I could have just done strncpy to make things a lot easier
void separateArrays(FILE *file, item *table_1, item *table_2)
{
    assert(file != NULL);
    assert(table_1 != NULL);
    assert(table_2 != NULL);
    // "weird use of sizeof/sizeof; sizeof table_x is the pointer size"
    // assert(TABLE_SIZE == ARRAY_LENTH(table_1)); // Should I assert ARRAY_SIZE is table's size?
    // assert(TABLE_SIZE == ARRAY_LENTH(table_2)); // Should I assert ARRAY_SIZE is table's size?
    char num[NUMSTR_MAX_DIGITS] = { '\0' };
    char ch;
    bool first = true;
    size_t n = 0;
    //size_t i = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (isdigit(ch)) {
            num[n] = ch;
            n++;
        } else if (n > 0) {
            if (first) {
                addAppearence(table_1, atoi(num));
            } else {
                addAppearence(table_2, atoi(num));
            }
            n = 0;
            memset(num, '\0', sizeof num);
            first = !first;
        }
    }
    return;
}


void zeroItems(item *table)
{
    item zeroed_item = {
        .value = 0,
        .appearances = 0
    };
    for (size_t i=0; i <= TABLE_SIZE; i++) {
        table[i] = zeroed_item;
        // printf("Num: %d\nAppearances: %d\n", table[i].key, table[i].appearances);
    }
    return;
}

int main()
{
    // Open file
    FILE *input = fopen("input", "r");
    assert(input != NULL && "Error while trying to open `input` file.");
    if (input == NULL) {
        perror("Error while trying to open `input` file.");
        return -1;
    }

    // Get tables' size to be able to define them more properly (perhaps nonsense to do this pass? No `realloc` or overflow though)
    rewind(input);

    item table_1[TABLE_SIZE]; item table_2[TABLE_SIZE]; zeroItems(table_1);
    zeroItems(table_2);

    separateArrays(input, table_1, table_2);

    printArray(table_1);
    printArray(table_2);
    int result = 0;

    size_t i = 0;
    size_t j = 0;
    unsigned int similarities = 0;
    // Get similarity
    // Iterate over every populated item in table_1
    for (i=0; i < ARRAY_SIZE && (table_1[i].value != 0 && table_1[i].appearances != 0); i++) {
        // try find current item from table_1 in table_2
        for (j=0;
            (j < ARRAY_SIZE) && // don't go over the ARRAY_SIZE limit
            (table_2[j].value != table_1[i].value) && // try to find the item
            (table_2[j].value != 0 && table_2[j].appearances != 0); // don't look further into non populated items
            j++);
        // if actually found a corresponding number, add calculate similarity and add to result
        if (table_1[i].value == table_2[j].value && j < ARRAY_SIZE) {
            result += table_1[i].value * table_1[i].appearances * table_2[j].appearances; // update result
            similarities++;
            printf("Similarity found!\n");
            printf("%d appeared %d times in table_1 and %d in table_2(%d)\n", table_1[i].value, table_1[i].appearances, table_2[j].appearances, table_2[j].value);
            printf("result: %u\n", result);
            printf("similarities: %u\n\n", similarities);
        }
    }

    printf("RESULT: %u\n", result);
    printf("SIMILARITIES: %u\n", similarities);
    fclose(input);
    return 0;
}
