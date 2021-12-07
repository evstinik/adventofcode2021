#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const int MAX = 4000;

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

long fuel_needed(int a, int b) {
    // sum of first N members of arithmetic sequence
    long N = abs(a - b + 1);
    return N * (N + 1) / 2;
}

int main() {
    int pos[MAX], len = 0;
    long fuel = 0, fuel_min = 10000000000;
    char del;
    
    memset(pos, 0, sizeof(int) * MAX);
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%d", &pos[len]) == 1 ) {
        len += 1;
        if ( fscanf(f, "%c", &del) != 1 ) break;
    }
    fclose(f);
    
    qsort(pos, len, sizeof(int), cmpfunc);
    
    for (int i = pos[0]; i <= pos[len - 1]; i += 1) {
        fuel = 0;
        for (int j = 0; j < len; j += 1) {
            fuel += fuel_needed(pos[j], i);
        }
        if (fuel_min > fuel) {
            fuel_min = fuel;
        }
    }
    printf("%ld\n", fuel_min);
}
