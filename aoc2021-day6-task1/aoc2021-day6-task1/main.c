#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int DAYS = 256;
const int MAX_TIMER = 9;

int main() {
    long long count_per_times[MAX_TIMER], timer, new_fishes = 0, reset_fishes = 0, sum = 0;
    char del;
    
    memset(count_per_times, 0, sizeof(long long) * MAX_TIMER);
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%lld", &timer) == 1 ) {
        sum += 1;
        count_per_times[timer] += 1;
        if ( fscanf(f, "%c", &del) != 1 ) break;
    }
    fclose(f);
    
    
    sum = 0;
    for (int day = 0; day < DAYS; day += 1) {
        new_fishes = count_per_times[0];
        reset_fishes = count_per_times[0];
        for (int timer_idx = 0; timer_idx < MAX_TIMER - 1; timer_idx += 1) {
            count_per_times[timer_idx] = count_per_times[timer_idx + 1];
        }
        count_per_times[6] += reset_fishes;
        count_per_times[8] = new_fishes;
    }
    
    for (int timer_idx = 0; timer_idx < MAX_TIMER; timer_idx += 1) {
        sum += count_per_times[timer_idx];
    }
    printf("%lld\n", sum);
}
