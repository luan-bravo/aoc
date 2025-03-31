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
#define GOLDEN_RATIO 2654435761U


typedef struct {
    unsigned int key;
    unsigned int appearances;
} item;


void printArray(item *array, size_t length)
{
    unsigned int count = 0;
    unsigned int available = 0;

    for (size_t i = 0; i < length; i++) {
        if (array[i].key != 0 && array[i].appearances != 0) {
            count += 1;
        } else {
            available += 1;
        }
        printf("num: %d\tappears: %d\n", array[i].key, array[i].appearances);
    }
    printf("%u populated items with %u left\n", count, available);
    printf("\n");
    return;
}


// TODO: figure a way to quickly count lines (binary search?
    // *nthline + 1 != eof  ?  goto *nthline += jump * 2  :  *nthline -= jump / 2)
size_t countLines(FILE *file)
{
    assert(file != NULL && "'file' is 'NULL'.");
    char ch = ' ';
    unsigned int count = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n')
            count++;
    }
    return count;
}


unsigned int hash(unsigned int key, size_t length)
{
    // Hash item by multiplying by the 32b golden ratio...
    // ...and doing a bit op with the Fs to ensure that hash is within 32b.
    // return mod of length to insert into array
    return ((key * GOLDEN_RATIO) & 0xFFFFFFFF) % length;
}

void addAppearence(item *array, size_t length, unsigned int num)
{
    // size_t i = findItemIndex(array, length, num);
    static unsigned int usedAtI[length];
    size_t i = hash(num, length);
    usedAtI[i]++;
    // find num index of array or empty index in array
    // if num found in array, mark its appearance. Else, add it to the end of the list
    if (!(array[i].key == num && array[i].appearances > 0))
        array[i].key = num;
    array[i].appearances += 1;
    return;
}


// Given that all lines have the same structure, I could have just done strncpy to make things a lot easier
void separateArrays(FILE *file, item *array_1, item *array_2, size_t length)
{
    assert(file != NULL && "'file' is NULL");
    assert(array_1 != NULL && "'array_1' is NULL");
    assert(array_2 != NULL && "'array_2' is NULL");
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
            if (first)
                addAppearence(array_1, length, atoi(num));
            else
                addAppearence(array_2, length, atoi(num));
            n = 0;
            memset(num, '\0', sizeof num);
            first = !first;
        }
    }
    return;
}


void zeroItems(item *array, size_t length)
{
    item zeroed_item = {
        .key = 0,
        .appearances = 0
    };
    for (size_t i=0; i <= length; i++) {
        array[i] = zeroed_item;
    }
    return;
}



int main()
{
    FILE *input = fopen("example_input", "r");
    assert(input != NULL && "Error while trying to open 'input' file.");
    if (input == NULL) {
        perror("Error while trying to open 'input' file.");
        return -1;
    }

    // Get arrays' size to be able to define them more properly (perhaps nonsense to do this pass? No 'realloc' or overflow though)
    const size_t ARRAY_SIZE = countLines(input);
    rewind(input);

    item array_1[ARRAY_SIZE];
    item array_2[ARRAY_SIZE];
    zeroItems(array_1, ARRAY_SIZE);
    zeroItems(array_2, ARRAY_SIZE);
    assert(ARRAY_SIZE == ARRAY_LENGTH(array_1) && "Provided length to separate arrays is not the actual length of the array at comptime");
    assert(ARRAY_SIZE == ARRAY_LENGTH(array_2) && "Provided length to separate arrays is not the actual length of the array at comptime");

    separateArrays(input, array_1, array_2, ARRAY_SIZE);

    printArray(array_1, ARRAY_SIZE);
    printArray(array_2, ARRAY_SIZE);

    unsigned int result = 0;
    size_t i = 0;
    size_t j = 0;
    unsigned int similarities = 0;
    // Get similarity
    // Iterate over every populated item in array_1
    for (i=0; i < ARRAY_SIZE && (array_1[i].key != 0 && array_1[i].appearances != 0); i++) {
        // Try find current item from array_1 in array_2
        for (j=0;
            (j < ARRAY_SIZE) && // don't go over the ARRAY_SIZE limit
            (array_2[j].key != array_1[i].key) && // try to find the item
            (array_2[j].key != 0 && array_2[j].appearances != 0); // don't look further into non populated items
            j++);
        // If actually found a corresponding number, add calculate similarity and add to result
        if (array_1[i].key == array_2[j].key && j < ARRAY_SIZE) {
            result += array_1[i].key * array_1[i].appearances * array_2[j].appearances; // update result
            similarities++;
            printf("Similarity found!\n");
            printf("%d appeared %d times in array_1 and %d in array_2(%d)\n", array_1[i].key, array_1[i].appearances, array_2[j].appearances, array_2[j].key);
            printf("result: %u\n", result);
            printf("similarities: %u\n\n", similarities);
        }
    }

    printf("RESULT: %u\n", result);
    printf("SIMILARITIES: %u\n", similarities);
    fclose(input);
    return 0;
}
