//
//  main.c
//  aoc2021-day19-task1
//
//  Created by Nikita Evstigneev on 19.12.2021.
//

#include <stdio.h>
#include <string.h>
#include <vector>
#include <math.h>
#include <map>
#include <set>

using namespace std;

struct Point {
    int x, y, z;
    Point(int x, int y, int z): x(x), y(y), z(z) {}
    float distance(Point & a) {
        return sqrtf(powf(x - a.x, 2) + powf(y - a.y, 2) + powf(z - a.z, 2));
    }
    friend bool operator==(const Point& l, const Point& r)
        {
            return l.x == r.x && l.y == r.y && l.z == r.z;
        }
    bool operator<(const Point& rhs) const
    {
        if (x < rhs.x) return true;
        if (x > rhs.x) return false;
        if (y < rhs.y) return true;
        if (y > rhs.y) return false;
        if (z < rhs.z) return true;
        if (z > rhs.z) return false;
        return false;
    }
};

vector<Point> build_from_permutation(vector<Point> & scanner, int perm[3]) {
    vector<Point> result;
    for (int i = 0; i < scanner.size(); i += 1) {
        int orig_p[3] = { scanner[i].x, scanner[i].y, scanner[i].z };
        Point p( orig_p[ abs(perm[0]) - 1 ], orig_p[ abs(perm[1]) - 1 ], orig_p[ abs(perm[2]) - 1 ] );
        if (perm[0] < 0) p.x *= -1;
        if (perm[1] < 0) p.y *= -1;
        if (perm[2] < 0) p.z *= -1;
        result.push_back(p);
    }
    return result;
}

vector<vector<Point>> permutate(vector<Point> & scanner) {
    vector<vector<Point>> result;
    int permutations[24][3] = {
        // Y -> Y
        { 1, 2, 3 }, { -3, 2, 1 }, { -1, 2, -3 }, { 3, 2, -1 },
        // Y -> -Y
        { 1, -2, -3 }, { 3, -2, 1 }, { -1, -2, 3 }, { -3, -2, -1 },
        // Y -> X
        { -2, 1, 3 }, { 3, 1, 2 }, { 2, 1, -3 }, { -3, 1, -2 },
        // Y -> -X
        { -2, -1, -3 }, { -3, -1, 2 }, { 2, -1, 3 }, { 3, -1, -2 },
        // Y -> Z
        { 1, 3, -2 }, { 2, 3, 1 }, { -1, 3, 2 }, { -2, 3, -1 },
        // Y -> -Z
        { 1, -3, 2 }, { 2, -3, -1 }, { -1, -3, -2 }, { -2, -3, 1 }
    };
    for (int i = 0; i < 24; i += 1) {
        result.push_back(build_from_permutation(scanner, permutations[i]));
    }
    return result;
}

struct Scanner {
    vector<int> numbers;
    vector<Point> points;
    vector<vector<Point>> permutations;
    Point center;
    int located_to_scanner;
    int matched_permutation;
    Scanner(int number, vector<Point> & points): points(points), located_to_scanner(-1), center(0, 0, 0), matched_permutation(0) {
        permutations = permutate(points);
        numbers.push_back(number);
    }
};

void print_data(vector<Scanner> & scanners) {
    for (int i = 0; i < scanners.size(); i += 1) {
        printf("--- scanner %d ---\n", scanners[i].numbers[0]);
        printf("center (relatively to %d scanner): %d,%d,%d\n", scanners[i].located_to_scanner, scanners[i].center.x, scanners[i].center.y, scanners[i].center.z);
        for (int j = 0; j < scanners[i].points.size(); j += 1) {
            printf("%d,%d,%d\n", scanners[i].points[j].x, scanners[i].points[j].y, scanners[i].points[j].z);
        }
        printf("\n");
    }
}

// TODO: Remove
void find_mapping(vector<Point> & scanner1, vector<Point> & scanner2) {
    map<float, vector<pair<int, int>>> distances;
    for (int i = 0; i < scanner1.size(); i += 1) {
        for (int j = 0; j < scanner2.size(); j += 1) {
            float d = scanner1[i].distance(scanner2[j]);
            distances[d].push_back(pair(i, j));
        }
    }
    
    for (auto it = distances.begin(); it != distances.end(); it++ ) {
        if (it->second.size() >= 2) {
            printf("Mapping with distance %4.2f:\n", it->first);
            for (int i = 0; i < it->second.size(); i += 1) {
                printf("%d,%d -> %d,%d\n", scanner1[it->second[i].first].x, scanner1[it->second[i].first].y, scanner2[it->second[i].second].x, scanner2[it->second[i].second].y);
            }
        }
    }
}

int merge_scanners(Scanner & scanner1, Scanner & _scanner2) {
    int found_matches = 0;
    // permutate across all possible orientations
    for (int perm_idx = 0; perm_idx < _scanner2.permutations.size(); perm_idx += 1) {
        auto scanner2 = _scanner2.permutations.begin() + perm_idx;
        // calculate distances from point to point and track same distances with pairs of point indexes
        map<float, vector<pair<int, int>>> distances;
        for (int i = 0; i < scanner1.points.size(); i += 1) {
            for (int j = 0; j < scanner2->size(); j += 1) {
                float d = scanner1.points[i].distance(scanner2->at(j));
                distances[d].push_back(pair(i, j));
            }
        }

//        printf("Distances counts:\n");
//        for (auto it = distances.begin(); it != distances.end(); it++ ) {
//            if (it->second.size() > 1) {
//                printf("%4.2f: %lu\n", it->first, it->second.size());
//            }
//        }
//        printf("Similar distances: ");
//        for (auto it = distances.begin(); it != distances.end(); it++ ) {
//            auto n = next(it);
//            if (n != distances.end() && fabs(it->first - n->first) < 0.005) {
//                printf("%4.6f %4.6f ", it->first, n->first);
//            }
//        }
//        printf("\n");
        
        // iterate over all distances and try to find same 12
        for (auto it = distances.begin(); it != distances.end(); it++ ) {
            if (it->second.size() >= 12) {
//                printf("Mapping with distance %4.2f:\n", it->first);
                Point v(0, 0, 0); // translate vector
                set<int> common_points; // point indexes from scanner 2 that match with scanner 1
                
                // double check that we need single translation vectors for resulting points
                bool valid = true;
                for (int i = 0; i < it->second.size(); i += 1) {
                    Point p1 = scanner1.points[it->second[i].first];
                    Point p2 = scanner2->at(it->second[i].second);
                    common_points.insert(it->second[i].second);
                    Point _v(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
//                    printf("%d,%d,%d (%d)\n", p1.x, p1.y, p1.z, it->second[i].first);
                    if (i == 0) {
                        v = _v;
                    } else if ( !(v == _v) ) {
                        // mapping requires different vectors -> this won't work
                        valid = false;
                        break;
                    }
                }
                
                // merge scanner2 -> scanner1
                if (valid) {
                    found_matches += 1;
                    _scanner2.located_to_scanner = scanner1.numbers[0];
                    _scanner2.center = v;
                    _scanner2.matched_permutation = perm_idx;
//                    for (int i = 0; i < scanner2->size(); i += 1) {
//                        // only include the point if it's not already in scanner1
//                        if ( common_points.find(i) == common_points.end() ) {
//                            Point p2 = scanner2->at(i);
//                            Point p1(p2.x + v.x, p2.y + v.y, p2.y + v.z);
//                            scanner1.push_back(p1);
//                        }
//                    }
                }
            }
        }
    }
    
    return found_matches;
}

int main(int argc, const char * argv[]) {
    FILE *f = fopen("input.txt", "r");
    char line[256];
    int x, y, z, idx = 0;
    vector<Scanner> scanners;
    vector<Point> centers;
    
    // read scanner name
    while (fgets(line, 256, f) && strlen(line) > 0) {
        // read points
        vector<Point> points;
        while (fscanf(f, "%s", line) == 1 && strlen(line) > 0) {
            if (sscanf(line, "%d,%d,%d", &x, &y, &z) != 3) {
                break;
            }
            points.push_back(Point(x, y, z));
        }
        Scanner scanner( idx++, points );
        scanners.push_back(scanner);
        centers.push_back(Point(0, 0, 0));
    }
    fclose(f);
    
    print_data(scanners);
    
    bool merged = true;
    while (scanners.size() > 1 && merged) {
        merged = false;
        for (int i = 0; i < scanners.size(); i += 1) {
            for (int j = i + 1; j < scanners.size(); j += 1) {
                int result = merge_scanners(scanners[i], scanners[j]);
                if ( result ) {
                    printf("Scanners %d and %d can be merged %d ways (used permutation %d)\n", scanners[i].numbers[0], scanners[j].numbers[0], result, scanners[j].matched_permutation);
                    printf("Scanner %d had %d points, scanner %d had %d points, should be %d points\n", scanners[i].numbers[0], scanners[i].points.size(), scanners[j].numbers[0], scanners[j].points.size(), scanners[i].points.size() + scanners[j].points.size() - 12);
                    merged = true;
                    // merging
                    for (int k = 0; k < scanners[j].permutations[scanners[j].matched_permutation].size(); k += 1) {
                        Point v = scanners[j].center;
                        Point p0 = scanners[j].permutations[scanners[j].matched_permutation][k];
                        Point p(v.x + p0.x, v.y + p0.y, v.z + p0.z);
                        scanners[i].points.push_back(p);
//                        scanners[i].numbers.push_back(j);
                    }
                    set<Point> s( scanners[i].points.begin(), scanners[i].points.end() );
                    scanners[i].points.assign( s.begin(), s.end() );
                    printf("now its %d points\n", scanners[i].points.size());
                    // recalculating permutations
                    scanners[i].permutations = permutate(scanners[i].points);
                    // recalculating centers
                    for (int k = 0; k < scanners[j].numbers.size(); k += 1) {
                        centers[scanners[j].numbers[k]].x += scanners[j].center.x;
                        centers[scanners[j].numbers[k]].y += scanners[j].center.y;
                        centers[scanners[j].numbers[k]].z += scanners[j].center.z;
                    }
                    // removing merged one
                    scanners.erase(scanners.begin() + j);
                    break;
                }
            }
            if (merged) break;
        }
    }
    
    set<Point> unique_points(scanners[0].points.begin(), scanners[0].points.end());
    printf("%lu\n", unique_points.size());
    
    for (int i = 0; i < centers.size(); i += 1) {
        printf("%d,%d,%d\n", centers[i].x, centers[i].y, centers[i].z);
    }
    
    int max = 0;
    for (int i = 0; i < centers.size(); i += 1) {
        for (int j = i + 1; j < centers.size(); j += 1) {
            int d = fabs(centers[i].x - centers[j].x) + fabs(centers[i].y - centers[j].y) + fabs(centers[i].z - centers[j].z);
            if (d > max) {
                max = d;
            }
        }
    }
    printf("Max: %d\n", max);
    
    return 0;
}
