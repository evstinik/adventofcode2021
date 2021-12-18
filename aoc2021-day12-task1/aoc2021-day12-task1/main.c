//
//  main.c
//  aoc2021-day12-task1
//
//  Created by Nikita Evstigneev on 12.12.2021.
//

#include <stdio.h>
#include <string.h>

const int MAX_CAVES = 100;
const int CAVE_NAME_LEN = 7;

char cave_names[CAVE_NAME_LEN][MAX_CAVES];
char edges[MAX_CAVES][MAX_CAVES], visited[MAX_CAVES];
int caves_count = 0, start, end, current_path[MAX_CAVES * MAX_CAVES], path_len = 0;

int is_small_cave(int cave) {
    return 'a' <= cave_names[cave][0] && cave_names[cave][0] <= 'z';
}

/** Finds cave by name and returns it index. If cave is not found adds it to the array and returns index. */
int find_cave_by_name(char *name) {
    for (int i = 0; i < caves_count; i += 1) {
        if (strcmp(cave_names[i], name) == 0) {
            return i;
        }
    }
    memcpy(&cave_names[caves_count], name, sizeof(char) * (strlen(name) + 1));
    caves_count += 1;
    return caves_count - 1;
}

void print_path() {
    for (int i = 0; i < path_len; i += 1) {
        printf("%s", cave_names[ current_path[i] ]);
        if (i < path_len - 1) {
            printf(",");
        }
    }
    printf("\n");
}

int dfs(int cave) {
    if (cave == end) {
//        print_path();
        return 1;
    }
    int paths = 0;
    for (int next_cave = 0; next_cave < caves_count; next_cave += 1) {
        if (next_cave == cave || !edges[cave][next_cave]) {
            continue;
        }
        if (
            !is_small_cave(next_cave)
            || !visited[next_cave]
        ) {
            visited[next_cave] = 1;
            current_path[path_len] = next_cave;
            path_len += 1;
            paths += dfs(next_cave);
            path_len -= 1;
            visited[next_cave] = 0;
        }
    }
    return paths;
}

int main(int argc, const char * argv[]) {
    char line[CAVE_NAME_LEN * 2 + 1], *name1, *name2;
    int cave1, cave2, paths = 0;
    
    memset(edges, 0, MAX_CAVES * MAX_CAVES * sizeof(char));
    memset(visited, 0, MAX_CAVES * sizeof(char));
    
    FILE *f = fopen("input0.txt", "r");
    while ( fscanf(f, "%s", line) == 1 ) {
        for (name2 = line; *name2 != '-'; name2 += 1) {}
        *name2 = 0;
        name2 += 1;
        name1 = line;
        cave1 = find_cave_by_name(name1);
        cave2 = find_cave_by_name(name2);
        edges[cave1][cave2] = 1;
        edges[cave2][cave1] = 1;
        if (strcmp(name1, "start") == 0) {
            start = cave1;
        }
        if (strcmp(name1, "end") == 0) {
            end = cave1;
        }
        if (strcmp(name2, "start") == 0) {
            start = cave2;
        }
        if (strcmp(name2, "end") == 0) {
            end = cave2;
        }
    }
    fclose(f);
    
    visited[start] = 1;
    current_path[0] = start;
    path_len += 1;
    paths = dfs(start);
    printf("%d\n", paths);
    
    return 0;
}
