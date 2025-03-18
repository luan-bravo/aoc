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


typedef struct {
    unsigned int key;
    unsigned int appearances;
} item;


void printArray(item *array, size_t length)
{
    size_t i = 0;
    for (; i < length && (array[i].key != 0 && array[i].appearances != 0); i++) {
        // printf("num: %d\nappears: %d\n", array[i].key, array[i].appearances);
    }
    size_t j = i;
    for (; j < length; j++);
    printf("%lu populated items with %lu left\n", i, j-i);
    printf("\n");
    return;
}


// TODO: figure a way to quickly count lines (binary search?
    // *nthline + 1 != eof  ?  goto *nthline += jump * 2  :  *nthline -= jump / 2)
size_t countLines(FILE *file)
{
    assert(file != NULL && "`file` variable is `NULL`.");
    char ch = '\0';
    assert(ch != EOF && "`ch` variable is already at `EOF`.");
    unsigned int count = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    return count;
}

// returns item index or first empty zeroed item index
size_t findItemIndex(item *array, size_t length, unsigned int num)
{
    size_t i;
    for (i=0; array[i].key != num && (array[i].key != 0 && array[i].appearances != 0) && i <= length; i++);
    assert(i < length && "Item not found and array is full");
    return i;
}

void addAppearence(item *array, size_t length, unsigned int num)
{
    size_t i = findItemIndex(array, length, num);
    // find num index of array or empty index in array
    // if num found in array, mark its appearance. Else, add it to the end of the list
    if (array[i].key == num && array[i].appearances > 0) {
        array[i].appearances += 1;
    } else {
        array[i].key = num;
        array[i].appearances += 1;
    }
    return;
}


// Given that all lines have the same structure, I could have just done strncpy to make things a lot easier
void separateArrays(FILE *file, item *array_1, item *array_2, size_t length)
{
    assert(file != NULL);
    assert(array_1 != NULL);
    assert(array_2 != NULL);
    // "weird use of sizeof/sizeof; sizeof array_x is the pointer size"
    // assert(length == ARRAY_LENTH(array_1)); // Should I assert ARRAY_SIZE is array's size?
    // assert(length == ARRAY_LENTH(array_2)); // Should I assert ARRAY_SIZE is array's size?
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
                addAppearence(array_1, length, atoi(num));
                // array_1[i] = atoi(num);
            } else {
                addAppearence(array_2, length, atoi(num));
                // array_2[i] = atoi(num);
                // i++;
            }
            n = 0;
            memset(num, '\0', sizeof num);
            first = !first;
        }
    }
    return;
}


/*
void xorSwap(int *num_1, int *num_2)
{
    if (*num_1 == *num_2) return;
    *num_1 = *num_1 ^ *num_2;
    *num_2 = *num_1 ^ *num_2;
    *num_1 = *num_1 ^ *num_2;
    return;
}

// Median-of-three to improve pivot choice
int medianOfThree(int *array, int low, int high) {
    int mid = low + (high - low) / 2;
    if (array[mid] < array[low]) {
        xorSwap(&array[mid], &array[low]);
    }
    if (array[high] < array[low]) {
        xorSwap(&array[high], &array[low]);
    }
    if (array[high] < array[mid]) {
        xorSwap(&array[high], &array[mid]);
    }
    // Place the median at the high position to use it as a pivot
    xorSwap(&array[mid], &array[high]);
    return array[high];
}

// Algorithm `inplace_quick_sort` taken from code fragment 12.6 of Data Structures and Algorithms in Python by Goldwasser, et al. Improved by adding median of three
void inplaceQuickSort(int *array, size_t length, size_t low, size_t high)
{
    // Sort the array from array[a] to array[b] inclusive
    assert(array != NULL && "`array` is a null pointer");
    if (low >= high) {
        return;
    }
    int pivot = medianOfThree(array, low, high);
    size_t left = low;
    size_t right = high-1;
    while (left <= right) {
        // printArray(array, length);
        while (left <= right && array[left] < pivot) {
            left += 1;
        }
        while (left <= right && pivot < array[right]) {
            right -= 1;
        }
        if (left <= right) {
            xorSwap(&array[left], &array[right]);
            left++;
            right--;
        }
    }
    xorSwap(&array[left], &array[high]);
    inplaceQuickSort(array, length, low, left-1);
    inplaceQuickSort(array, length, left+1, high);
    return;
}
*/


void zeroItems(item *array, size_t length)
{
    item zeroed_item = {
        .key = 0,
        .appearances = 0
    };
    for (size_t i=0; i <= length; i++) {
        array[i] = zeroed_item;
        // printf("Num: %d\nAppearances: %d\n", array[i].key, array[i].appearances);
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

    // Get arrays' size to be able to define them more properly (perhaps nonsense to do this pass? No `realloc` or overflow though)
    const size_t ARRAY_SIZE = countLines(input);
    rewind(input);

    item array_1[ARRAY_SIZE];
    item array_2[ARRAY_SIZE];
    zeroItems(array_1, ARRAY_SIZE);
    zeroItems(array_2, ARRAY_SIZE);

    separateArrays(input, array_1, array_2, ARRAY_SIZE);

    // inplaceQuickSort(array_1, ARRAY_SIZE, 0, (ARRAY_SIZE-1));
    // inplaceQuickSort(array_2, ARRAY_SIZE, 0, (ARRAY_SIZE-1));

    printArray(array_1, ARRAY_SIZE);
    printArray(array_2, ARRAY_SIZE);
    int result = 0;
    // for (size_t i=0; i < ARRAY_SIZE; i++) {
    //     result += calcDifference(&array_1[i], &array_2[i]);
    // }

    size_t i = 0;
    size_t j = 0;
    unsigned int similarities = 0;
    // Get similarity
    // Iterate over every populated item in array_1
    for (i=0; i < ARRAY_SIZE && (array_1[i].key != 0 && array_1[i].appearances != 0); i++) {
        // try find current item from array_1 in array_2
        for (j=0;
            (j < ARRAY_SIZE) && // don't go over the ARRAY_SIZE limit
            (array_2[j].key != array_1[i].key) && // try to find the item
            (array_2[j].key != 0 && array_2[j].appearances != 0); // don't look further into non populated items
            j++);
        // if actually found a corresponding number, add calculate similarity and add to result
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
