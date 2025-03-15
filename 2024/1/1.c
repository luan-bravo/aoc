#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>
#include <ctype.h>

#define ARRAY_LENTH(arr) (sizeof(arr) / sizeof((arr)[0]))


// Given that all lines have the same structure, I could have just done strncpy to make things a lot easier
void separate_lists(FILE *file, size_t LINE_MAX_SIZE, uint32_t *list_1, uint32_t *list_2, size_t LIST_SIZE)
{
    assert(file != NULL);
    assert(list_1 != NULL);
    assert(list_2 != NULL);
    // "weird use of sizeof/sizeof; sizeof list_x is the pointer size"
    assert(LIST_SIZE == ARRAY_LENTH(list_1)); // Should I assert LIST_SIZE is list's size?
    assert(LIST_SIZE == ARRAY_LENTH(list_2)); // Should I assert LIST_SIZE is list's size?

    char ch;
    const size_t NUMSTR_MAX_SIZE = 5;
    char num[NUMSTR_MAX_SIZE];
    bool is_first = true;

    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            continue;
        }
        if (isdigit(ch)){

        }


    }

    return;
}


// Algorithm `inplace_quick_sort` taken from code fragment 12.6 of Data Structures and Algorithms in Python by Goldwasser, et al.
/* Sort the list from list[a] to list[b] */
void inplace_quick_sort(uint32_t *list, size_t length, size_t a, size_t b) // TODO: check if length is necessary at any point
{
    assert(a < b && "Trying to sort list backwars"); // Would be possible to just swap a and b instead
    assert(list != NULL && "`list` is a null pointer");
    uint32_t pivot = list[b];
    size_t left = a;
    size_t right = b-1;

    while (left <= right) {
        while (left <= right && list[left] < pivot) {
            left += 1;
        }
        while (left <= right && list[right] < pivot) {
            right -= 1;
        }
        if (left <= right) {
            // XOR swap
            list[left] = list[left] ^ list[right];
            list[right] = list[left] ^ list[right];
            list[left] = list[left] ^ list[right];

            left++;
            right--;
        }
    }
    inplace_quick_sort(list, length, a, left-1);
    inplace_quick_sort(list, length, left+1, b);
    return;
}


uint32_t difference(uint32_t *value_1, uint32_t *value_2)
{
    assert(value_1 != NULL && "`value1` is a null pointer");
    assert(value_2 != NULL && "`value2` is a null pointer");
    return (*value_1 - *value_2);
}


// TODO: figure a way to quickly count lines (binary search?
    // *nthline + 1 != eof  ?  goto *nthline += jump * 2  :  *nthline -= jump / 2)
const size_t count_lines(FILE* file)
{
    assert(file != NULL && "`file` variable is `NULL`.");

    char ch;
    assert(ch != EOF && "`file` variable is already at `EOF`.");

    uint32_t count = 0;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    return count;
}



int main()
{
    // Open file
    FILE *input = fopen("example_input", "r");
    assert(input != NULL && "Error while trying to open `input` file.");
    if (input == NULL) {
        perror("Error while trying to open `input` file.");
        return -1;
    }

    // Get arrays' size to be able to define them more properly (perhaps nonsense to do this pass? No `realloc` though)

    size_t LIST_SIZE = count_lines(input); // Possibly no way to `pthread` as can't divide & conquer
    rewind(input);
    size_t LINE_MAX_SIZE = 14;

    uint32_t list_1[LIST_SIZE];
    uint32_t list_2[LIST_SIZE];

    separate_lists(input, LINE_MAX_SIZE, list_1, list_2, LIST_SIZE);

    // void inplace_quick_sort(*int32_t *list, const size_t length, size_t a, size_t b)
    inplace_quick_sort(list_1, LIST_SIZE, 0, LIST_SIZE);
    inplace_quick_sort(list_2, LIST_SIZE, 0, LIST_SIZE);
    uint32_t restult[LIST_SIZE];
    for (size_t i; *list_1 == NULL || *list_2 == NULL; list_1++, list_2++) {

    }



    fclose(input);
    return 0;
}
