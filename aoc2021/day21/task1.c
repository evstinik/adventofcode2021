//
//  task1.c
//  aoc2021
//
//  Created by Nikita Evstigneev on 21.12.2021.
//

#include <stdio.h>

const int PLAYERS = 2;

int throw_dice(int *dice) {
    *dice += 1;
    return *dice - 1;
}

int main(int argc, const char * argv[]) {
    int player_pos[PLAYERS] = { 7, 4 };
    int score[PLAYERS] = { 0, 0 };
    int dice = 1, reached_max = 0, dice_threw = 0;
    
    while ( ! reached_max ) {
        for (int pl = 0; pl < PLAYERS; pl += 1) {
            for (int i = 0; i < 3; i += 1) {
                player_pos[pl] += throw_dice(&dice);
                dice_threw += 1;
            }
            player_pos[pl] = ((player_pos[pl] - 1) % 10) + 1;
            score[pl] += player_pos[pl];
            if (score[pl] >= 1000) {
                reached_max = 1;
                break;
            }
        }
    }
    
    int losers_score = score[0] >= 1000 ? score[1] : score[0];
    printf("%d * %d = %d\n", losers_score, dice_threw, losers_score * dice_threw);
    
    return 0;
}
