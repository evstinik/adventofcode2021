//
//  main.c
//  aoc2021-day15-task1
//
//  Created by Nikita Evstigneev on 15.12.2021.
//

#include <stdio.h>
#include <string.h>

const int MAP_SIZE = 100;

short map[MAP_SIZE][MAP_SIZE];
short prev[MAP_SIZE][MAP_SIZE];
int min[MAP_SIZE][MAP_SIZE];

void search(int x, int y) {
    if (x > 0 && min[x - 1][y] > min[x][y] + map[x - 1][y]) {
        min[x - 1][y] = min[x][y] + map[x - 1][y];
        prev[x - 1][y] = 0;
        search(x - 1, y);
    }
    if (y > 0 && min[x][y - 1] > min[x][y] + map[x][y - 1]) {
        min[x][y - 1] = min[x][y] + map[x][y - 1];
        prev[x][y - 1] = 1;
        search(x, y - 1);
    }
    if (x < MAP_SIZE - 1 && min[x + 1][y] > min[x][y] + map[x + 1][y]) {
        min[x + 1][y] = min[x][y] + map[x + 1][y];
        prev[x + 1][y] = 2;
        search(x + 1, y);
    }
    if (y < MAP_SIZE - 1 && min[x][y + 1] > min[x][y] + map[x][y + 1]) {
        min[x][y + 1] = min[x][y] + map[x][y + 1];
        prev[x][y + 1] = 3;
        search(x, y + 1);
    }
}

int main(int argc, const char * argv[]) {
    char c;
    
    FILE *f = fopen("input.txt", "r");
    for (int y = 0; y < MAP_SIZE; y += 1) {
        for (int x = 0; x < MAP_SIZE; x += 1) {
            prev[x][y] = -1;
            min[x][y] = 9 * MAP_SIZE * MAP_SIZE;
            fscanf(f, "%c", &c);
            map[x][y] = c - '0';
        }
        fscanf(f, "%c", &c); // read newline
    }
    fclose(f);
    
    min[0][0] = 0;
    search(0, 0);
    
    printf("%d\n", min[MAP_SIZE - 1][MAP_SIZE - 1]);
    
    return 0;
}
