//
//  task2.c
//  day22
//
//  Created by Nikita Evstigneev on 22.12.2021.
//

#include <stdio.h>
#include <vector>
#include <math.h>

using namespace std;

const int BITMAP_SZ = 7; // ceil( 420 (max cubes) / ( 8 (bits per byte) * 8 (sizeof long long) ) )

struct Cube {
    int min_x, max_x, min_y, max_y, min_z, max_z;
    bool is_on;
    Cube(int min_x, int max_x, int min_y, int max_y, int min_z, int max_z, bool is_on): min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y), min_z(min_z), max_z(max_z), is_on(is_on) {}
};

struct Interval {
    /** interval goes from this value includingly to the next from of next interval in array (not including) */
    int from;
    /** bitmap of cubes presented in interval */
    unsigned long long cubes[BITMAP_SZ];
    Interval() {
        memset(cubes, 0, sizeof(long long) * BITMAP_SZ);
    }
};

void add_to_intervals(vector<Interval> & intervals, int min_x, int max_x, int cube_idx) {
    int x_idx, min_x_idx = 0, max_x_idx = 0;
    
    // Insert left (min_x)
    Interval left;
    left.from = min_x;
    x_idx = (int)(upper_bound(intervals.begin(), intervals.end(), left, [](const Interval& a, const Interval& b){return a.from < b.from;}) - intervals.begin());
    
    if (x_idx < intervals.size() && left.from < intervals[x_idx].from) {
        // found
        if (x_idx > 0 && intervals[x_idx - 1].from == left.from) {
            // case when it exactly matches previous
            min_x_idx = x_idx - 1;
        } else {
            // not matched, simple insert
            if (x_idx > 0) {
                memcpy(left.cubes, intervals[x_idx - 1].cubes, sizeof(long long) * BITMAP_SZ);
            }
            intervals.insert(intervals.begin() + x_idx, left);
            min_x_idx = x_idx;
        }
    } else {
        // not found
        if (x_idx > 0 && intervals[x_idx - 1].from == left.from) {
            // case when it exactly matches previous
            min_x_idx = x_idx - 1;
        } else {
            intervals.insert(intervals.end(), left);
            min_x_idx = (int)intervals.size() - 1;
        }
    }
    
    // Insert right (max x)
    Interval right;
    right.from = max_x + 1;
    
    x_idx = (int)(upper_bound(intervals.begin(), intervals.end(), right, [](const Interval& a, const Interval& b){return a.from < b.from;}) - intervals.begin());
    
    if (x_idx < intervals.size() && right.from < intervals[x_idx].from) {
        // found
        if (x_idx > 0 && intervals[x_idx - 1].from == right.from) {
            // case when its matched with previous
            max_x_idx = x_idx - 1;
        } else {
            // normal insert
            if (x_idx > 0) {
                memcpy(right.cubes, intervals[x_idx - 1].cubes, sizeof(long long) * BITMAP_SZ);
            }
            intervals.insert(intervals.begin() + x_idx, right);
            max_x_idx = x_idx;
        }
    } else {
        // not found
        if (x_idx > 0 && intervals[x_idx - 1].from == right.from) {
            // case when its matched with previous
            max_x_idx = x_idx - 1;
        } else {
            intervals.insert(intervals.end(), right);
            max_x_idx = (int)intervals.size() - 1;
        }
    }
    
    // precalculate place in bitmap for cube with index `cube_idx`
    int cubes_dst_idx = (cube_idx / (8 * sizeof(long long))); // big steps
    // small steps
    unsigned long long cube_present = 1;
    for (int i = 1; i <= cube_idx % (8 * sizeof(long long)); i += 1) cube_present <<= 1;
    
    // mark for each intervals from min_x to max_x cube as presented
    for (int idx = min_x_idx; idx < max_x_idx; idx += 1) {
        intervals[idx].cubes[cubes_dst_idx] = intervals[idx].cubes[cubes_dst_idx] | cube_present;
    }
}

int main() {
    vector<Cube> cubes;
    vector<Interval> x_intervals, y_intervals, z_intervals;
    int min_x, max_x, min_y, max_y, min_z, max_z;
    char action[8];
    unsigned long long result = 0;
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%s x=%d..%d,y=%d..%d,z=%d..%d", action, &min_x, &max_x, &min_y, &max_y, &min_z, &max_z) == 7 ) {
        bool on = strcmp(action, "on") == 0;
        Cube c(min_x, max_x, min_y, max_y, min_z, max_z, on);
        cubes.push_back(c);
        add_to_intervals(x_intervals, min_x, max_x, (int)cubes.size() - 1);
        add_to_intervals(y_intervals, min_y, max_y, (int)cubes.size() - 1);
        add_to_intervals(z_intervals, min_z, max_z, (int)cubes.size() - 1);
    }
    fclose(f);
    
    for (int x_int = 0; x_int < (int)x_intervals.size() - 1; x_int += 1) {
        printf("%d/%lu\n", x_int + 1, x_intervals.size());
        
        int min_x = x_intervals[x_int].from;
        int max_x = x_intervals[x_int + 1].from;
        
        for (int y_int = 0; y_int < (int)y_intervals.size() - 1; y_int += 1) {
            
            int min_y = y_intervals[y_int].from;
            int max_y = y_intervals[y_int + 1].from;
            
            for (int z_int = 0; z_int < (int)z_intervals.size() - 1; z_int += 1) {
                
                // detect latest common cube for current X,Y,Z intervals
                // by applying bitwise AND to cube present bitmaps
                unsigned long long common_cubes = 0;
                int last_common_cube = sizeof(long long) * 8 * (BITMAP_SZ - 1);
                // detect big step
                for (int cubes_dst_idx = BITMAP_SZ - 1; cubes_dst_idx >= 0; cubes_dst_idx -= 1) {
                    common_cubes = x_intervals[x_int].cubes[cubes_dst_idx] & y_intervals[y_int].cubes[cubes_dst_idx] & z_intervals[z_int].cubes[cubes_dst_idx];
                    if (common_cubes) {
                        // detect small step
                        while (common_cubes >>= 1) {
                            last_common_cube += 1;
                        }
                        break;
                    }
                    last_common_cube -= sizeof(long long) * 8;
                }
                
                if (last_common_cube < 0) {
                    continue;
                }
                
                int min_z = z_intervals[z_int].from;
                int max_z = z_intervals[z_int + 1].from;
                bool is_on = cubes[last_common_cube].is_on;
                if (is_on) {
                    unsigned long long l = (max_x - min_x), w = (max_y - min_y), h = (max_z - min_z);
                    result += w * h * l;
                }
            }
        }
    }
    
    printf("%llu\n", result);
    
    return 0;
}
