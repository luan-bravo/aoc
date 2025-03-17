#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#define LENGTH 10

void xorSwap(uint32_t* num_1, uint32_t* num_2)
{
    if (*num_1 == *num_2) return;
    *num_1 = *num_1 ^ *num_2;
    *num_2 = *num_1 ^ *num_2;
    *num_1 = *num_1 ^ *num_2;
    return;
}

void printArray(uint32_t* array, size_t length)
{
    for (size_t i = 1; i <= length; i++, array++) {
        printf("%d ", *array);
    }
    printf("\n");
    return;
}

// Sort from array[a] to array[b] inclusive
void inplaceQuickSort(uint32_t *array, size_t length, size_t a, size_t b) // TODO: check if length is necessary at any point
{
    assert(array != NULL && "`array` is a null pointer");

    if (a >= b) {
        return;
    }

    uint32_t pivot = array[b];
    size_t left = a;
    size_t right = b-1;
    while (left <= right) {
        printArray(array, length);
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

    // set pivot to left
    xorSwap(&array[left], &array[b]);

    inplaceQuickSort(array, length, a, left-1);
    inplaceQuickSort(array, length, left+1, b);
    return;
}

int main() {
    uint32_t array[LENGTH] = { 3, 6, 2, 9, 1, 0, 4, 7, 5, 8 };

    printArray(array, LENGTH);
    inplaceQuickSort(array, LENGTH, 0, (LENGTH-1));
    printArray(array, LENGTH);

    return 0;
}
