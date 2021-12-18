//
//  main.c
//  aoc2021-day17-task1
//
//  Created by Nikita Evstigneev on 17.12.2021.
//

#include <stdio.h>
#include <math.h>
#include <set>
#include <map>
#include <vector>

using namespace std;

int main(int argc, const char * argv[]) {
//    int x1 = 20, x2 = 30, y1 = -10, y2 = -5;
    int x1 = 230, x2 = 283, y1 = -107, y2 = -57;
    int r_vx0 = 0, r_vy0 = 0;
    
    int best_steps = 0;
    set<pair<int, int>> v;
    
    for (int steps = 1; steps < 1000; steps += 1) {
        for (int vy0 = 1000; vy0 >= -1000; vy0 -= 1) {
            for (int vx0 = 0; vx0 <= 400; vx0 += 1) {
                int x = ((vx0 + vx0 - fminf(steps, vx0 + 1) + 1) * fminf(steps, vx0 + 1)) / 2;
                int y = ((vy0 + vy0 - steps + 1) * steps) / 2;
                if ( x1 <= x && x <= x2 && y1 <= y && y <= y2 ) {
                    v.insert(pair<int, int>(vx0, vy0));
                    if (r_vy0 < vy0) {
                        r_vx0 = vx0;
                        r_vy0 = vy0;
                        best_steps = steps;
                    }
                }
            }
        }
    }
    int maxy = (int)((float)(r_vy0 * fminf(best_steps, r_vy0 + 1)) * 0.5f);
    
    printf("%d,%d, max y = %d (steps = %d)\n", r_vx0, r_vy0, maxy, best_steps);
    printf("%lu\n", v.size());
    
//    vector<pair<int, int>> _v(v.begin(), v.end());
//    for (int i = 0; i < _v.size(); i += 1) {
//        printf("%d, %d\n", _v.at(i).first, _v.at(i).second);
//    }
    
    return 0;
}
