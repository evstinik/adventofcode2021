//
//  main.c
//  aoc2021-day13-task1
//
//  Created by Nikita Evstigneev on 13.12.2021.
//

#include <stdio.h>
#include <string.h>

const int MAX = 2000;
const int LINE_LEN = 256;

void print_letter(char map[MAX][MAX], int max_x, int max_y, int letter_idx, int letter_counts) {
    int letter_w = (max_x / letter_counts);
    for (int y = 0; y <= max_y; y += 1) {
        for (int x = letter_w * letter_idx; x <= letter_w * (letter_idx + 1); x += 1) {
            printf("%c", map[x][y] ? '#' : '.');
        }
        printf("\n");
    }
}

void print_map(char map[MAX][MAX], int max_x, int max_y) {
    for (int y = 0; y <= max_y; y += 1) {
        for (int x = 0; x <= max_x; x += 1) {
            printf("%c", map[x][y] ? '#' : '.');
        }
        printf("\n");
    }
}

int main(int argc, const char * argv[]) {
    char map[MAX][MAX], line[LINE_LEN], line2[LINE_LEN], axis;
    int value = 0, x, y, max_x = 0, max_y = 0, sum = 0;
    
    memset(map, 0, sizeof(char) * MAX * MAX);
    
    FILE *f = fopen("input.txt", "r");
    while ( fgets(line, LINE_LEN, f) ) {
        if (strcmp(line, "\n") == 0) {
            break;
        }
        if (sscanf(line, "%d,%d", &x, &y) != 2) {
            break;
        }
        map[x][y] = 1;
        if (x > max_x) {
            max_x = x;
        }
        if (y > max_y) {
            max_y = y;
        }
    }
    printf("max x = %d, max y = %d\n", max_x, max_y);
    
    while ( fscanf(f, "%s %s %c=%d", line, line2, &axis, &value) == 4) {
        printf("max x = %d, max y = %d\n", max_x, max_y);
        printf("%c %d\n", axis, value);
        if (axis == 'x') {
            if (value < max_x/2) {
                printf("Folding across x=%d, when max is %d\n", value, max_x);
                
            }
            for (int dx = 1; dx <= (max_x - value); dx += 1) {
                for (int y = 0; y <= max_y; y += 1) {
                    map[value - dx][y] = map[value - dx][y] || map[value + dx][y];
                }
            }
            max_x = value - 1;// > max_x/2 ? value : max_x - value;
        } else if (axis == 'y') {
            if (value < max_y/2) {
                printf("Folding across y=%d, when max is %d\n", value, max_x);
            }
            for (int x = 0; x <= max_x; x += 1) {
                for (int dy = 1; dy <= (max_y - value); dy += 1) {
                    map[x][value - dy] = map[x][value - dy] || map[x][value + dy];
                }
            }
            max_y = value - 1;// > max_y/2 ? value : max_y - value;
        }
    }
    fclose(f);
    
    printf("max x = %d, max y = %d\n", max_x, max_y);
    print_map(map, max_x, max_y);
    for (char c = 'A'; c <= 'Z'; c += 1) {
        printf("%c ", c);
    }
    printf("\n");
//    print_letter(map, max_x, max_y, 0, 8);
    
//    for (int x = 0; x <= max_x; x += 1) {
//        for (int y = 0; y <= max_y; y += 1) {
//            sum += map[x][y];
//        }
//    }
//    printf("Sum: %d\n", sum);
    
    return 0;
}
