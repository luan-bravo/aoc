#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define REPORT_SIZE 16384
#define LINE_SIZE 256
#define STRLEVEL_SIZE 256

bool is_within_limits(unsigned int *a, unsigned int *b)
{
    const int minDiff = 1;
    const int maxDiff = 3;
    const int diff = *b - *a;
    return ((diff >= minDiff && diff <= maxDiff) || (diff >= -minDiff && diff <= -maxDiff));
}

int main()
{
    FILE* input = fopen("input", "r");
    assert(input != NULL && "Error while trying to open `input` file.");
    char report[LINE_SIZE] = curLine;
    char adjacentLevels[2][REPORT_SIZE] = { report[i], report[i+1*(non-space)] };
    char* c = '\0';
    int i = 0;
    while (report[i] != EOF && i < REPORT_SIZE) {
        char strLevel[STRLEVEL_SIZE];
        char* nc = '\0';
        int ni = 0;
        for (; strLevel[ni] != ' ' && strLevel[ni] != '\n' && strLevel[ni] != EOF && ni < STRLEVEL_SIZE; ni++, i++) {
            strLevel[ni] = report;
        }
    }
}
