//
//  main.c
//  day22
//
//  Created by Nikita Evstigneev on 22.12.2021.
//

#include <stdio.h>
#include <string.h>

const char MIN = -50;
const char MAX = 50;

int between(int t, int a, int b) {
    if (t < a) return a;
    if (t > b) return b;
    return t;
}

int main(int argc, const char * argv[]) {
    char state[MAX - MIN][MAX - MIN][MAX - MIN], action[8];
    int min_x, max_x, min_y, max_y, min_z, max_z;
    long long on_count = 0;
    memset(state, 0, sizeof(char) * (MAX - MIN) * (MAX - MIN) * (MAX - MIN));
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%s x=%d..%d,y=%d..%d,z=%d..%d", action, &min_x, &max_x, &min_y, &max_y, &min_z, &max_z) == 7 ) {
        min_x = between(min_x, -50, 50);
        max_x = between(max_x, -50, 50);
        if (max_x - min_x <= 0) continue;
        min_y = between(min_y, -50, 50);
        max_y = between(max_y, -50, 50);
        if (max_y - min_y <= 0) continue;
        min_z = between(min_z, -50, 50);
        max_z = between(max_z, -50, 50);
        if (max_z - min_z <= 0) continue;
        char on = strcmp(action, "on") == 0;
        for (int x = min_x; x <= max_x; x += 1) {
            for (int y = min_y; y <= max_y; y += 1) {
                for (int z = min_z; z <= max_z; z += 1) {
                    if (state[x - MIN][y - MIN][z - MIN] != on) {
                        on_count += on ? 1 : -1;
                    }
                    state[x - MIN][y - MIN][z - MIN] = on;
                }
            }
        }
    }
    fclose(f);
    
    printf("%lld\n", on_count);
    
    return 0;
}
