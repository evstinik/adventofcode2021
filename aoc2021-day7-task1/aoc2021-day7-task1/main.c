#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX = 4000;

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main() {
    int pos[MAX], len = 0, median = 0;
    long fuel = 0;
    char del;
    
    memset(pos, 0, sizeof(int) * MAX);
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%d", &pos[len]) == 1 ) {
        len += 1;
        if ( fscanf(f, "%c", &del) != 1 ) break;
    }
    fclose(f);
    
    qsort(pos, len, sizeof(int), cmpfunc);
    
    if (len % 2 == 1) {
        median = pos[len / 2];
    } else {
        median = (pos[len / 2 - 1] + pos[len / 2]) / 2;
    }

    for (int i = 0; i < len; i += 1) {
        fuel += abs(pos[i] - median);
    }
    
    printf("%ld\n", fuel);
}
