//
//  main.c
//  aoc2021-day20-task1
//
//  Created by Nikita Evstigneev on 20.12.2021.
//

#include <stdio.h>
#include <string.h>

const int MAX_SIZE = 200;
const int DEC_SIZE = 512;

char algo[DEC_SIZE + 2];
char input[MAX_SIZE][MAX_SIZE], c, outside = 0;
int w = 100, h = 100;
//int w = 5, h = 5;

int run_algo(void) {
    char result[MAX_SIZE][MAX_SIZE];
    int count = 0, new_x, new_y, new_w, new_h;
    
    for (int y = -1; y <= h; y += 1) {
        for (int x = -1; x <= w; x += 1) {
            int algo_idx = 0;
            // top row
            if (y > 0) {
                algo_idx = algo_idx * 2 + ((x > 0) ? input[x-1][y-1] : outside);
                algo_idx = algo_idx * 2 + ((x >= 0 && x < w) ? input[x][y-1] : outside);
                algo_idx = algo_idx * 2 + ((x < (w - 1)) ? input[x+1][y-1] : outside);
            } else {
                algo_idx = algo_idx * 2 + outside;
                algo_idx = algo_idx * 2 + outside;
                algo_idx = algo_idx * 2 + outside;
            }
            // current row
            if (y >= 0 && y < h) {
                algo_idx = algo_idx * 2 + ((x > 0) ? input[x-1][y] : outside);
                algo_idx = algo_idx * 2 + ((x >= 0 && x < w) ? input[x][y] : outside);
                algo_idx = algo_idx * 2 + ((x < (w - 1)) ? input[x+1][y] : outside);
            } else {
                algo_idx = algo_idx * 2 + outside;
                algo_idx = algo_idx * 2 + outside;
                algo_idx = algo_idx * 2 + outside;
            }
            // bottom row
            if (y < (h - 1)) {
                algo_idx = algo_idx * 2 + ((x > 0) ? input[x-1][y+1] : outside);
                algo_idx = algo_idx * 2 + ((x >= 0 && x < w) ? input[x][y+1] : outside);
                algo_idx = algo_idx * 2 + ((x < (w - 1)) ? input[x+1][y+1] : outside);
            } else {
                algo_idx = algo_idx * 2 + outside;
                algo_idx = algo_idx * 2 + outside;
                algo_idx = algo_idx * 2 + outside;
            }
            
            result[x+1][y+1] = algo[algo_idx] == '#';
            if (result[x+1][y+1]) count += 1;
        }
    }
    
    outside = algo[outside ? 511 : 0] == '#';
    
//    new_x = 0;
//    new_w = w + 2;
//    new_y = 0;
//    new_h = h + 2;
    
    new_w = w;
    new_h = h + 2;
    new_x = 1;
    for (int i = 0; i < new_h; i += 1) {
        if (result[0][i]) {
            new_w = w + 1;
            new_x = 0;
            break;
        }
    }
    for (int i = 0; i < new_h; i += 1) {
        if (result[w + 1][i]) {
            new_w += 1;
            break;
        }
    }
    new_h = h;
    new_y = 1;
    for (int i = new_x; i < new_w; i += 1) {
        if (result[i][0]) {
            new_h = h + 1;
            new_y = 0;
            break;
        }
    }
    for (int i = new_x; i < new_w; i += 1) {
        if (result[i][h+1]) {
            new_h += 1;
            break;
        }
    }

    for (int x = new_x; x < new_x + new_w; x += 1) {
        for (int y = new_y; y < new_y + new_h; y += 1) {
            input[x - new_x][y - new_y] = result[x][y];
        }
    }
    w = new_w;
    h = new_h;
    
    return count;
}

void print_input(void) {
    for (int y = 0; y < h; y += 1) {
        for (int x = 0; x < w; x += 1) {
            printf("%c", input[x][y] ? '#' : '.');
        }
        printf("\n");
    }
}

int main(int argc, const char * argv[]) {
    FILE *f = fopen("input.txt", "r");
    fscanf(f, "%512s", algo);
    fscanf(f, "%c", &c);
    fscanf(f, "%c", &c);
    for (int y = 0; y < h; y += 1) {
        for (int x = 0; x < w; x += 1) {
            fscanf(f, "%c", &c);
            input[x][y] = c == '#';
        }
        fscanf(f, "%c", &c);
    }
    fclose(f);
    
    print_input();
    printf("\n");
    
    int result = 0;
    for (int i = 0; i < 50; i += 1) {
        result = run_algo();
    }
    
    printf("%d\n", result);
    
    return 0;
}
