//
//  main.c
//  aoc2021-day9-task1
//
//  Created by Nikita Evstigneev on 09.12.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SIZE_X = 100;
const int SIZE_Y = 100;

//const int SIZE_X = 10;
//const int SIZE_Y = 5;

char map[SIZE_Y][SIZE_X];
int map_basins[SIZE_Y][SIZE_X];

int dfs(char x, char y, int basin_idx) {
    int sum = 1;
    map_basins[y][x] = basin_idx;
    if ( x > 0 && map[y][x-1] < 9 && map_basins[y][x-1] != basin_idx ) {
        sum += dfs(x-1, y, basin_idx);
    }
    if ( x < SIZE_X - 1 && map[y][x+1] < 9 && map_basins[y][x+1] != basin_idx ) {
        sum += dfs(x+1, y, basin_idx);
    }
    if ( y > 0 && map[y-1][x] < 9 && map_basins[y-1][x] != basin_idx ) {
        sum += dfs(x, y-1, basin_idx);
    }
    if ( y < SIZE_Y - 1 && map[y+1][x] < 9 && map_basins[y+1][x] != basin_idx ) {
        sum += dfs(x, y+1, basin_idx);
    }
    return sum;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)b - *(int*)a );
}

int main() {
    
    char del;
    int sum = 0, basin_area[SIZE_X * SIZE_Y], basin_idx = 0;
    
    memset(basin_area, 0, sizeof(int) * SIZE_X * SIZE_Y);
    
    FILE *f = fopen("input.txt", "r");
    for (int y = 0; y < SIZE_Y; y += 1) {
        for (int x = 0; x < SIZE_X; x += 1) {
            fscanf(f, "%c", &map[y][x]);
            map_basins[y][x] = -1;
            map[y][x] -= '0';
        }
        fscanf(f, "%c", &del);
    }
    fclose(f);
    
    for (int y = 0; y < SIZE_Y; y += 1) {
        for (int x = 0; x < SIZE_X; x += 1) {
            if (
                (x == 0 || map[y][x] < map[y][x-1])
                && (x == SIZE_X - 1 || map[y][x] < map[y][x+1])
                && (y == 0 || map[y][x] < map[y-1][x])
                && (y == SIZE_Y - 1 || map[y][x] < map[y+1][x])
                ) {
                    sum += map[y][x] + 1;
                    basin_area[basin_idx] = dfs(x, y, basin_idx);
                    basin_idx += 1;
//                    printf("%d ", map[y][x]);
                } else {
//                    printf(". ");
                }
        }
//        printf("\n");
    }
    
    qsort(basin_area, basin_idx, sizeof(int), cmpfunc);
    
    printf("%dx: %d * %d * %d = %d\n", basin_idx, basin_area[0], basin_area[1], basin_area[2], basin_area[0] * basin_area[1] * basin_area[2]);
    
    return 0;
}
