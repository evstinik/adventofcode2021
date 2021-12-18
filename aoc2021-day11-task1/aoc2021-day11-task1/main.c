//
//  main.c
//  aoc2021-day11-task1
//
//  Created by Nikita Evstigneev on 11.12.2021.
//

#include <stdio.h>
#include <string.h>

const int GRID_SIZE = 10;
short map[GRID_SIZE][GRID_SIZE];
short flashed[GRID_SIZE][GRID_SIZE];

void print_map() {
    for (short y = 0; y < GRID_SIZE; y += 1) {
        for (short x = 0; x < GRID_SIZE; x += 1) {
            printf("%d", map[x][y]);
        }
        printf("\n");
    }
}

long long increase(short x, short y) {
    long long flashes = 0;
    if (flashed[x][y]) {
        return flashes;
    }
    map[x][y] += 1;
    if (map[x][y] > 9) {
        map[x][y] = 0;
        flashed[x][y] = 1;
        flashes += 1;
        if (x > 0) {
            flashes += increase(x - 1, y);
        }
        if (x > 0 && y > 0) {
            flashes += increase(x - 1, y - 1);
        }
        if (y > 0) {
            flashes += increase(x, y - 1);
        }
        if (x < GRID_SIZE - 1 && y > 0) {
            flashes += increase(x + 1, y - 1);
        }
        if (x < GRID_SIZE - 1) {
            flashes += increase(x + 1, y);
        }
        if (x < GRID_SIZE - 1 && y < GRID_SIZE - 1) {
            flashes += increase(x + 1, y + 1);
        }
        if (y < GRID_SIZE - 1) {
            flashes += increase(x, y + 1);
        }
        if (x > 0 && y < GRID_SIZE - 1) {
            flashes += increase(x - 1, y + 1);
        }
    }
    return flashes;
}

int main(int argc, const char * argv[]) {
    char level;
    long long sum = 0;
    
    memset(map, 0, sizeof(short) * GRID_SIZE * GRID_SIZE);
    
    FILE *f = fopen("input.txt", "r");
    for (short y = 0; y < GRID_SIZE; y += 1) {
        for (short x = 0; x < GRID_SIZE; x += 1) {
            fscanf(f, "%c", &level);
            map[x][y] = level - '0';
        }
        fscanf(f, "%c", &level);
    }
    fclose(f);
    
    printf("Before any step:\n");
    print_map();
    printf("\n");
    
    sum = 0;
    for (int step = 0; step < 100; step += 1) {
        memset(flashed, 0, sizeof(short) * GRID_SIZE * GRID_SIZE);
        for (short x = 0; x < GRID_SIZE; x += 1) {
            for (short y = 0; y < GRID_SIZE; y += 1) {
                sum += increase(x, y);
            }
        }
    }
    
    printf("%lld\n", sum);
    
    return 0;
}
