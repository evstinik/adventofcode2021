#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int MAX = 1000;

int sign(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

int main() {
    int points_count = 0;
    int x1, x2, y1, y2, v_x, v_y, x, y;
    int grid[MAX][MAX];
    
    memset(grid, 0, sizeof(int) * MAX * MAX);
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%d,%d -> %d,%d", &x1, &y1, &x2, &y2) == 4 ) {
        v_x = sign(x2 - x1);
        v_y = sign(y2 - y1);
        if (v_x != 0 && v_y != 0) {
            continue;
        }
        x = x1;
        y = y1;
        while (1) {
            grid[x][y] += 1;
            if (x == x2 && y == y2) {
                break;
            }
            x += v_x;
            y += v_y;
        }
    }
    fclose(f);
    
    for (int y = 0; y < MAX; y += 1) {
        for (int x = 0; x < MAX; x += 1) {
            if (grid[x][y] > 1) {
                points_count += 1;
            }
        }
    }
    
    printf("Points: %d\n", points_count);
}
