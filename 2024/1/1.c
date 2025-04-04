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


void printArray(int *array, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        printf("%d ", array[i]);
    }
    printf("\n");
    return;
}


// TODO: figure a way to quickly count lines (binary search?
    // *nthline + 1 != eof  ?  goto *nthline += jump * 2  :  *nthline -= jump / 2)
const size_t countLines(FILE *file)
{
    assert(file != NULL && "`file` variable is `NULL`.");

    char ch;
    assert(ch != EOF && "`ch` variable is already at `EOF`.");

    int count = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    return count;
}


// Given that all lines have the same structure, I could have just done strncpy to make things a lot easier
void separateArrays(FILE *file,  int *array_1, int *array_2, size_t ARRAY_SIZE)
{
    assert(file != NULL);
    assert(array_1 != NULL);
    assert(array_2 != NULL);
    // "weird use of sizeof/sizeof; sizeof array_x is the pointer size"
    // assert(ARRAY_SIZE == ARRAY_LENTH(array_1)); // Should I assert ARRAY_SIZE is array's size?
    // assert(ARRAY_SIZE == ARRAY_LENTH(array_2)); // Should I assert ARRAY_SIZE is array's size?

    char num[NUMSTR_MAX_DIGITS] = { '\0' };
    char ch;
    bool first = true;
    size_t n = 0;
    size_t i = 0;

    while ((ch = fgetc(file)) != EOF) {
        if (isdigit(ch)) {
            num[n] = ch;
            n++;
        } else if (n > 0) {
            if (first) {
                array_1[i] = atoi(num);
            } else {
                array_2[i] = atoi(num);
                i++;
            }
            n = 0;
            memset(num, '\0', sizeof num);
            first = !first;
        }
    }
    return;
}


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
    /* Sort the array from array[a] to array[b] inclusive */
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


int calcDifference(int *value_1, int *value_2)
{
    assert(value_1 != NULL && "`value1` is a null pointer");
    assert(value_2 != NULL && "`value2` is a null pointer");
    if (*value_1 < *value_2)
        return *value_2 - *value_1;
    else
        return *value_1 - *value_2;
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

    int array_1[ARRAY_SIZE];
    int array_2[ARRAY_SIZE];

    separateArrays(input, array_1, array_2, ARRAY_SIZE);

    // void inplace_quick_sort(*int32_t *array, const size_t length, size_t a, size_t b)
    inplaceQuickSort(array_1, ARRAY_SIZE, 0, (ARRAY_SIZE-1));
    inplaceQuickSort(array_2, ARRAY_SIZE, 0, (ARRAY_SIZE-1));

    printArray(array_1, ARRAY_SIZE);
    printArray(array_2, ARRAY_SIZE);
    int result = 0;
    for (size_t i=0; i < ARRAY_SIZE; i++) {
        result += calcDifference(&array_1[i], &array_2[i]);
    }
    printf("RESULT: %d\n", result);

    fclose(input);
    return 0;
}
