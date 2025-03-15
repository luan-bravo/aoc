#include <assert.h>
#include <stdio.h>
#include <stdint.h>


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
    FILE *input = fopen("input", "r");
    assert(input != NULL && "Error while trying to open `input` file.");
    if (input == NULL) {
        perror("Error while trying to open `input` file.");
        return -1;
    }


    size_t line_size;

    char ch;
    while ((ch = fgetc(input)) != EOF) {
        line_size++;
        if (ch == '\n') {
            line_size++;
            break;
        }
    }
    printf("line size: %u", (unsigned int)line_size);



    /*
    // Get arrays' size to be able to define them more properly (perhaps nonsense to do this pass? No `realloc` though)

    const size_t LIST_SIZE = count_lines(input); // Possibly no way to `pthread` as can't divide & conquer
    rewind(input);
    const static size_t LINE_MAX_SIZE = 14;

    uint32_t list_1[LIST_SIZE];
    uint32_t list_2[LIST_SIZE];

    separate_lists(input, LINE_MAX_SIZE, list_1, list_2, LIST_SIZE);

    quick_sort(list_1, LIST_SIZE);
    quick_sort(list_2, LIST_SIZE);
    */

    // compare_lists

    fclose(input);
    return 0;
}
