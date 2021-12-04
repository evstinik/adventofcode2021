#include <stdio.h>
#include <string.h>

const int MAX_SEQ = 100;
const int N = 100;
const int GRID_SIZE = 5;

typedef struct TCell {
    short n;
    short move;
} TCell;

// Complexity: O(N), where N is number of boards
// Memory: O(1)

int main() {
    short input[MAX_SEQ], input_len = 0, n, max_move_per_col[GRID_SIZE], max_move_per_row[GRID_SIZE], min_move_per_grid = N, global_min_move = N;
    char del, exit = 0;
    TCell cells[GRID_SIZE][GRID_SIZE];
    int move_lookup[N], global_result = 0;
    
    memset(cells, 0, sizeof(TCell) * GRID_SIZE * GRID_SIZE);
    memset(move_lookup, 0, sizeof(int) * N);
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%hd", &input[input_len]) == 1 ) {
        // store lookup number -> move # (moves are 1-based)
        move_lookup[ input[input_len] ] = input_len + 1;
        input_len += 1;
        if ( fscanf(f, "%c", &del) != 1 || del != ',') {
            break;
        }
    }
    
    while ( !exit ) {
        memset(max_move_per_row, 0, sizeof(short) * GRID_SIZE);
        memset(max_move_per_col, 0, sizeof(short) * GRID_SIZE);
        
        // read boards with move # to complete for each number
        // and detect max moves per each column / row
        for (int col = 0; col < GRID_SIZE && !exit; col += 1) {
            for (int row = 0; row < GRID_SIZE; row += 1) {
                if ( fscanf(f, "%hd", &n) != 1 ) {
                    exit = 1;
                    break;
                }
                cells[row][col].n = n;
                cells[row][col].move = move_lookup[n];
                if ( cells[row][col].move ) {
                    if ( max_move_per_col[col] < cells[row][col].move ) {
                        max_move_per_col[col] = cells[row][col].move;
                    }
                    if ( max_move_per_row[row] < cells[row][col].move ) {
                        max_move_per_row[row] = cells[row][col].move;
                    }
                }
            }
        }
        if (exit) {
            break;
        }
        
        // detect min moves per grid to finish board
        min_move_per_grid = N;
        for (int i = 0; i < GRID_SIZE; i += 1) {
            if ( min_move_per_grid > max_move_per_col[i] ) {
                min_move_per_grid = max_move_per_col[i];
            }
            if ( min_move_per_grid > max_move_per_row[i] ) {
                min_move_per_grid = max_move_per_row[i];
            }
        }
        
        // detect min moves needed to win the game
        if ( global_min_move > min_move_per_grid ) {
            global_min_move = min_move_per_grid;
            global_result = 0;
            // calculate sum of unfinished numbers
            for (int col = 0; col < GRID_SIZE; col += 1) {
                for (int row = 0; row < GRID_SIZE; row += 1) {
                    if ( cells[row][col].move > min_move_per_grid ) {
                        global_result += cells[row][col].n;
                    }
                }
            }
            global_result *= input[ global_min_move - 1 ];
        }
    }
    fclose(f);
    
    printf("Result: %d\n", global_result);
}
