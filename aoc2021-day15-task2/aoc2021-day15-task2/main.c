//
//  main.c
//  aoc2021-day15-task1
//
//  Created by Nikita Evstigneev on 15.12.2021.
//

#include <stdio.h>
#include <string.h>

const int MAP_SIZE = 500;

short map[MAP_SIZE][MAP_SIZE];
short prev[MAP_SIZE][MAP_SIZE];
int min[MAP_SIZE][MAP_SIZE];
short visited[MAP_SIZE][MAP_SIZE];

int main(int argc, const char * argv[]) {
    char c;
    int count = 0, x, y;
    short _min;
    
    memset(visited, 0, sizeof(short) * MAP_SIZE * MAP_SIZE);
    
    FILE *f = fopen("input.txt", "r");
    for (int y = 0; y < MAP_SIZE / 5; y += 1) {
        for (int x = 0; x < MAP_SIZE / 5; x += 1) {
            prev[x][y] = -1;
            fscanf(f, "%c", &c);
            min[x][y] = 9 * MAP_SIZE * MAP_SIZE;
            map[x][y] = c - '0';
//            printf("Setting %d, %d\n", x, y);
            int step = MAP_SIZE / 5;
            for (int dy = y; dy < MAP_SIZE; dy += step) {
                for (int dx = x; dx < MAP_SIZE; dx += step) {
                    if (dx > x) {
//                        printf("Copying %d, %d -> %d, %d\n", dx - step, dy, dx, dy);
                        min[dx][dy] = 10 * MAP_SIZE * MAP_SIZE;
                        map[dx][dy] = map[dx - step][dy] + 1;
                    } else if (dy > y) {
//                        printf("Copying %d, %d -> %d, %d\n", dx, dy - step, dx, dy);
                        min[dx][dy] = 9 * MAP_SIZE * MAP_SIZE;
                        map[dx][dy] = map[dx][dy - step] + 1;
                    }
                    if (map[dx][dy] >= 10) {
                        map[dx][dy] -= 9;
                    }
                }
            }
        }
        fscanf(f, "%c", &c); // read newline
    }
    fclose(f);
    
//    for (int y = 0; y < MAP_SIZE; y += 1) {
//        for (int x = 0; x < MAP_SIZE; x += 1) {
//            printf("%d", map[x][y]);
//        }
//        printf("\n");
//    }
    
    min[0][0] = 0;
    count = 1;
    while (count < MAP_SIZE * MAP_SIZE - 1) {
        _min = MAP_SIZE * MAP_SIZE * 10;
        for (int _y = 0; _y < MAP_SIZE; _y += 1) {
            for (int _x = 0; _x < MAP_SIZE; _x += 1){
                if (min[_x][_y] < _min && !visited[_x][_y]) {
                    _min = min[_x][_y];
                    x = _x;
                    y = _y;
                }
            }
        }
        visited[x][y] = 1;
        if (x > 0 && !visited[x-1][y] && min[x - 1][y] > min[x][y] + map[x - 1][y]) {
            min[x - 1][y] = min[x][y] + map[x - 1][y];
            prev[x - 1][y] = 0;
        }
        if (y > 0 && !visited[x][y-1] && min[x][y - 1] > min[x][y] + map[x][y - 1]) {
            min[x][y - 1] = min[x][y] + map[x][y - 1];
            prev[x][y - 1] = 1;
        }
        if (x < MAP_SIZE - 1 && !visited[x+1][y] && min[x + 1][y] > min[x][y] + map[x + 1][y]) {
            min[x + 1][y] = min[x][y] + map[x + 1][y];
            prev[x + 1][y] = 2;
        }
        if (y < MAP_SIZE - 1 && !visited[x][y+1] && min[x][y + 1] > min[x][y] + map[x][y + 1]) {
            min[x][y + 1] = min[x][y] + map[x][y + 1];
            prev[x][y + 1] = 3;
        }
        count += 1;
    }
    
    printf("%d\n", min[MAP_SIZE - 1][MAP_SIZE - 1]);
    
    return 0;
}
