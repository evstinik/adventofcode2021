//
//  main.c
//  aoc2021-day10-task1
//
//  Created by Nikita Evstigneev on 10.12.2021.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int MAX = 256;

//int points(char illegal) {
//    switch (illegal) {
//        case ')':
//            return 3;
//        case ']':
//            return 57;
//        case '}':
//            return 1197;
//        case '>':
//            return 25137;
//        default:
//            return 0;
//    }
//}

int points(char autocomplete) {
    switch (autocomplete) {
        case ')':
            return 1;
        case ']':
            return 2;
        case '}':
            return 3;
        case '>':
            return 4;
        default:
            return 0;
    }
}

char get_closing(char left) {
    switch (left) {
        case '(':
            return ')';
        case '[':
            return ']';
        case '{':
            return '}';
        case '<':
            return '>';
        default:
            return 0;
    }
}

char is_matching(char left, char right) {
    return get_closing(left) == right;
}

int cmpfunc (const void * a, const void * b) {
    long long _a = *(long long *)a;
    long long _b = *(long long *)b;
    if (_a < _b) {
        return -1;
    } else if (_a == _b) {
        return 0;
    } else {
        return 1;
    }
}

int main(int argc, const char * argv[]) {
    char stack[MAX], line[MAX], c;
    int stack_len = 0, len = 0, valid = 0, scores_len = 0;
    long long scores[MAX];
    
    FILE *f = fopen("input.txt", "r");
    while ( fscanf(f, "%s", line) == 1 ) {
        stack_len = 0;
        len = (int)strlen(line);
        valid = 1;
        for (int i = 0; i < len; i += 1) {
            c = line[i];
            if (c == '(' || c == '[' || c == '{' || c == '<') {
                // push
                stack[stack_len] = c;
                stack_len += 1;
            } else {
                if (stack_len == 0 || !is_matching(stack[stack_len - 1], c)) {
                    printf("Left '%c', not matching right '%c'\n", stack[stack_len - 1], c);
                    valid = 0;
                    break;
                } else {
                    stack_len -= 1; // pop
                }
            }
        }
        if (valid && stack_len > 0) {
            scores[scores_len] = 0;
            while (stack_len > 0) {
                scores[scores_len] = scores[scores_len] * (long long)5 + (long long)points(get_closing(stack[stack_len - 1]));
                stack_len -= 1;
            }
            printf("%lld\n", scores[scores_len]);
            scores_len += 1;
        }
    }
    fclose(f);
    
    qsort(scores, scores_len, sizeof(long long), cmpfunc);
    
    printf("Count: %d\n", scores_len);
    if (scores_len % 2 == 0) {
        printf("%lld\n", scores[scores_len / 2 - 1]);
    } else {
        printf("%lld\n", scores[scores_len / 2]);
    }
    
    return 0;
}
