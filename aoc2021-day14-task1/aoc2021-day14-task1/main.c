//
//  main.c
//  aoc2021-day14-task1
//
//  Created by Nikita Evstigneev on 14.12.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX = 'Z' - 'A' + 1;

char presented[MAX];
long long counts[MAX];
char next[MAX][MAX];

typedef struct TListElem {
    char c;
    struct TListElem *next, *prev;
} TListElem;

TListElem* init_elem(char c, TListElem *prev, TListElem *next) {
    TListElem *e = (TListElem *)malloc(sizeof(TListElem));
    e->c = c;
    e->prev = prev;
    e->next = next;
    if (next) {
        next->prev = e;
    }
    if (prev) {
        prev->next = e;
    }
    return e;
}

void print_list(TListElem *e) {
    if (e->c != 0) {
        printf("%c", e->c);
    }
    if (e->next) {
        print_list(e->next);
    }
}

void free_list(TListElem *e) {
    if (e->next) {
        free_list(e->next);
    }
    free(e);
}

int main(int argc, const char * argv[]) {
    char c, a, b, min_c, max_c;
    TListElem *start = init_elem(0, NULL, NULL), *end = init_elem(0, start, NULL), *cur = start, *tmp;
    
    memset(presented, 0, sizeof(char) * MAX);
    memset(counts, 0, sizeof(long long) * MAX);
    memset(next, 0, sizeof(char) * MAX * MAX);
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%c", &c) == 1 && c != '\n') {
        cur = init_elem(c, cur, end);
        presented[c - 'A'] = 1;
        counts[c - 'A'] += 1;
    }
    printf("Template: ");
    print_list(start);
    printf("\n");
    
    fscanf(f, "%c", &a);
    while ( fscanf(f, "%c%c -> %c\n", &a, &b, &c) == 3 ) {
        next[a - 'A'][b - 'A'] = c;
    }
    fclose(f);
    
    for (int step = 0; step < 10; step += 1) {
        cur = start->next;
        while (cur && cur->next && cur->next != end) {
            a = next[cur->c - 'A'][cur->next->c - 'A'];
            if (a) {
                presented[a - 'A'] = 1;
                counts[a - 'A'] += 1;
                tmp = cur->next;
                init_elem(a, cur, cur->next);
                cur = tmp;
            } else {
                cur = cur->next;
            }
        }
        printf("After step %d: ", step + 1);
        print_list(start);
        printf("\n");
    }
    
    free_list(start);
    
    min_c = -1;
    max_c = -1;
    for (char c = 'A'; c <= 'Z'; c += 1) {
        if (!presented[c - 'A']) {
            continue;
        }
        if (min_c == -1 || counts[c - 'A'] < counts[min_c - 'A']) {
            min_c = c;
        }
        if (max_c == -1 || counts[c - 'A'] > counts[max_c - 'A']) {
            max_c = c;
        }
    }
    printf("Max: %c, %lldx\n", max_c, counts[max_c - 'A']);
    printf("Min: %c, %lldx\n", min_c, counts[min_c - 'A']);
    printf("Result: %lldx\n", counts[max_c - 'A'] - counts[min_c - 'A']);
    
    return 0;
}
