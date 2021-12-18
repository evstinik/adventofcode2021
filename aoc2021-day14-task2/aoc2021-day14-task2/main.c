//
//  main.c
//  aoc2021-day14-task1
//
//  Created by Nikita Evstigneev on 14.12.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int UNIQUE_ELEMENTS = 10;

const char elements[UNIQUE_ELEMENTS] = { 'B', 'C', 'F', 'H', 'K', 'N', 'O', 'P', 'S', 'V' };
int get_elem_idx(char a) {
    for (int i = 0; i < UNIQUE_ELEMENTS; i += 1) {
        if (a == elements[i]) {
            return i;
        }
    }
    return -1;
}

int get_pair_idx(char a, char b) {
    return get_elem_idx(a) * UNIQUE_ELEMENTS + get_elem_idx(b);
}

void print_pair_counts(long long *pair_counts) {
    for (int a = 0; a < UNIQUE_ELEMENTS; a += 1) {
        for (int b = 0; b < UNIQUE_ELEMENTS; b += 1) {
            if (pair_counts[a * UNIQUE_ELEMENTS + b] > 0) {
                printf("%c%c: %lld\n", elements[a], elements[b], pair_counts[a * UNIQUE_ELEMENTS + b]);
            }
        }
    }
}

int main(int argc, const char * argv[]) {
    char c, a, b, min_c = 0, max_c = 0, prev = 0, first_el = 0, last_el = 0;
    short next_pairs[UNIQUE_ELEMENTS * UNIQUE_ELEMENTS][2], next_pairs_count[UNIQUE_ELEMENTS * UNIQUE_ELEMENTS];
    long long pair_counts[UNIQUE_ELEMENTS * UNIQUE_ELEMENTS], tmp_pair_counts[UNIQUE_ELEMENTS * UNIQUE_ELEMENTS];
    long long el_counts[UNIQUE_ELEMENTS], min = -1, max = -1;
    
    memset(next_pairs, 0, sizeof(short) * UNIQUE_ELEMENTS * UNIQUE_ELEMENTS * 2);
    memset(next_pairs_count, 0, sizeof(short) * UNIQUE_ELEMENTS * UNIQUE_ELEMENTS);
    memset(pair_counts, 0, sizeof(long long) * UNIQUE_ELEMENTS * UNIQUE_ELEMENTS);
    memset(el_counts, 0, sizeof(long long) * UNIQUE_ELEMENTS);
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%c", &c) == 1 && c != '\n') {
        if (prev > 0) {
            pair_counts[get_pair_idx(prev, c)] += 1;
        }
        if (first_el == 0) {
            first_el = c;
        }
        prev = c;
        last_el = c;
    }
    
    fscanf(f, "%c", &a);
    while ( fscanf(f, "%c%c -> %c\n", &a, &b, &c) == 3 ) {
        int pair_idx = get_pair_idx(a, b);
        next_pairs_count[pair_idx] = 2;
        next_pairs[ pair_idx ][0] = get_pair_idx(a, c);
        next_pairs[ pair_idx ][1] = get_pair_idx(c, b);
    }
    fclose(f);
    
    printf("Pair counts:\n");
    print_pair_counts(pair_counts);
    
    for (int step = 0; step < 40; step += 1) {
        memset(tmp_pair_counts, 0, sizeof(long long) * UNIQUE_ELEMENTS * UNIQUE_ELEMENTS);
        for (int pair_idx = 0; pair_idx < UNIQUE_ELEMENTS * UNIQUE_ELEMENTS; pair_idx += 1) {
            if ( next_pairs_count[pair_idx] == 0 ) {
                tmp_pair_counts[ pair_idx ] = pair_counts[pair_idx];
            } else {
                tmp_pair_counts[ next_pairs[pair_idx][0] ] += pair_counts[pair_idx];
                tmp_pair_counts[ next_pairs[pair_idx][1] ] += pair_counts[pair_idx];
            }
        }
        memcpy(pair_counts, tmp_pair_counts, sizeof(long long) * UNIQUE_ELEMENTS * UNIQUE_ELEMENTS);
        printf("Pair counts:\n");
        print_pair_counts(pair_counts);
    }
    
    for (int a = 0; a < UNIQUE_ELEMENTS; a += 1) {
        for (int b = 0; b < UNIQUE_ELEMENTS; b += 1) {
            el_counts[a] += pair_counts[ a * UNIQUE_ELEMENTS + b ];
            el_counts[b] += pair_counts[ a * UNIQUE_ELEMENTS + b ];
        }
    }
    
    el_counts[ get_elem_idx(first_el) ] -= 1;
    el_counts[ get_elem_idx(last_el) ] -= 1;
    for (int i = 0; i < UNIQUE_ELEMENTS; i += 1) {
        el_counts[i] /= 2;
    }
    el_counts[ get_elem_idx(first_el) ] += 1;
    el_counts[ get_elem_idx(last_el) ] += 1;
    
    for (int i = 0; i < UNIQUE_ELEMENTS; i += 1) {
        if (el_counts[i] > 0 && (min == -1 || el_counts[i] < min)) {
            min = el_counts[i];
            min_c = elements[i];
        }
        if (max == -1 || el_counts[i] > max) {
            max = el_counts[i];
            max_c = elements[i];
        }
    }
    printf("Max: %c %lldx\n", max_c, max);
    printf("Min: %c %lldx\n", min_c, min);
    printf("Result: %lldx\n", max - min);
    
    return 0;
}
