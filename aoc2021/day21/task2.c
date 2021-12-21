//
//  task2.c
//  day21
//
//  Created by Nikita Evstigneev on 21.12.2021.
//

#include <stdio.h>
#include <string.h>

const int PLAYERS = 2;
const int MAX_SCORE = 21;
const int MAX_MOVES = 21;
const int MAX_POSITION = 10;

int main(int argc, const char * argv[]) {
    int player_pos[PLAYERS] = { 7, 4 };
    long long pl1[MAX_MOVES][MAX_POSITION][MAX_SCORE + 1];
    long long pl2[MAX_MOVES][MAX_POSITION][MAX_SCORE + 1];
    
    memset(pl1, 0, sizeof(long long) * MAX_MOVES * MAX_POSITION * (MAX_SCORE + 1));
    memset(pl2, 0, sizeof(long long) * MAX_MOVES * MAX_POSITION * (MAX_SCORE + 1));
    pl1[0][player_pos[0] - 1][0] = 1;
    pl2[0][player_pos[1] - 1][0] = 1;
    
    for (int move = 1; move < MAX_MOVES; move += 1) {
        for (int dice1 = 1; dice1 <= 3; dice1 += 1) {
            for (int dice2 = 1; dice2 <= 3; dice2 += 1) {
                for (int dice3 = 1; dice3 <= 3; dice3 += 1) {
                    for (int prev_pos = 0; prev_pos < MAX_POSITION; prev_pos += 1) {
                        int next_pos = (prev_pos + (dice1 + dice2 + dice3)) % MAX_POSITION;
                        for (int prev_score = 0; prev_score < MAX_SCORE; prev_score += 1) {
                            int next_score = prev_score + next_pos + 1;
                            if (next_score > MAX_SCORE) {
                                next_score = MAX_SCORE;
                            }
                            pl1[move][next_pos][next_score] += pl1[move-1][prev_pos][prev_score];
                            pl2[move][next_pos][next_score] += pl2[move-1][prev_pos][prev_score];
                        }
                    }
                }
            }
        }
    }
    
    long long pl1_win = 0, pl2_win = 0;
    for (int move = 1; move < MAX_MOVES; move += 1) {
        long long sum_win_pl1 = 0;
        long long sum_win_pl2 = 0;
        for (int pos = 0; pos < MAX_POSITION; pos += 1) {
            sum_win_pl1 += pl1[move][pos][MAX_SCORE];
            sum_win_pl2 += pl2[move][pos][MAX_SCORE];
        }
        
        long long sum_loose_pl1 = 0;
        long long sum_loose_pl2 = 0;
        for (int pos = 0; pos < MAX_POSITION; pos += 1) {
            for (int score = 0; score < MAX_SCORE; score += 1) {
                sum_loose_pl1 += pl1[move][pos][score];
                sum_loose_pl2 += pl2[move][pos][score];
            }
        }
        
        long long prev_not_win_pl2 = 0;
        for (int pos = 0; pos < MAX_POSITION; pos += 1) {
            for (int score = 0; score < MAX_SCORE; score += 1) {
                prev_not_win_pl2 += pl2[move - 1][pos][score];
            }
        }
        
        printf("At move %d player 1 can win: %lld\n", move, sum_win_pl1);
        printf("At move %d player 2 can loose: %lld\n", move, sum_loose_pl2);
        
        pl1_win += sum_win_pl1 * prev_not_win_pl2;
        pl2_win += sum_win_pl2 * sum_loose_pl1;
    }
    
    printf("Player 1 wins: %lld\n", pl1_win);
    printf("Player 2 wins: %lld\n", pl2_win);
    
    return 0;
}
