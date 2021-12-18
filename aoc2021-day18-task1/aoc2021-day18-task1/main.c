//
//  main.c
//  aoc2021-day18-task1
//
//  Created by Nikita Evstigneev on 18.12.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct TNode {
    int number;
    struct TNode *left, *right, *parent;
} TNode;

TNode *init_node(int number, TNode *left, TNode *right, TNode *parent) {
    TNode *r = (TNode *)malloc(sizeof(TNode));
    r->number = number;
    r->left = left;
    r->right = right;
    r->parent = parent;
    if (r->left) {
        r->left->parent = r;
    }
    if (r->right) {
        r->right->parent = r;
    }
    return r;
}

TNode *copy_node(TNode *n, TNode *parent) {
    TNode *r = init_node(n->number, NULL, NULL, parent);
    if (n->left) {
        r->left = copy_node(n->left, r);
    }
    if (n->right) {
        r->right = copy_node(n->right, r);
    }
    return r;
}

void free_tree(TNode *n) {
    if (n->left) {
        free_tree(n->left);
    }
    if (n->right) {
        free_tree(n->right);
    }
    free(n);
}

TNode *read_number(char *s, int *idx, TNode *parent) {
    int len = (int)strlen(s);
    TNode *result = init_node(0, NULL, NULL, parent);
    
    if (s[*idx] == '[') {
        *idx += 1;
        result->left = read_number(s, idx, result);
        *idx += 1; // ','
        result->right = read_number(s, idx, result);
        *idx += 1; // ']'
        return result;
    }
    
    result->number = 0;
    while ( '0' <= s[*idx] && s[*idx] <= '9' && *idx < len ) {
        result->number = result->number * 10 + (s[*idx] - '0');
        *idx += 1;
    }
    
    return result;
}

void print_tree(TNode *n) {
    if (n->left && n->right) {
        printf("[");
        print_tree(n->left);
        printf(",");
        print_tree(n->right);
        printf("]");
    } else {
        printf("%d", n->number);
    }
}

int is_number_node(TNode *n) {
    return n->left == NULL && n->right == NULL;
}

TNode *explode_node(TNode *n, int level, int *changed) {
    if (level >= 4 && n->left && n->right && is_number_node(n->left) && is_number_node(n->right)) {
        // explode
        *changed = 1;
        
        // add to the left
        TNode *t = n;
        // go up until we are the left child
        while (t && t->parent && t->parent->left == t) {
            t = t->parent;
        }
        // we are either at the top or we are the right child
        if (t && t->parent && t->parent->right == t) {
            // now travel down
            t = t->parent->left;
            while (t->right) {
                t = t->right;
            }
            // we found the closes left number node
            t->number += n->left->number;
        }
        
        // add to the right
        t = n;
        // go up until we are the right child
        while (t && t->parent && t->parent->right == t) {
            t = t->parent;
        }
        // we are either at the top or we are the left child
        if (t && t->parent && t->parent->left == t) {
            // now travel down
            t = t->parent->right;
            while (t->left) {
                t = t->left;
            }
            // we found the closest right number node
            t->number += n->right->number;
        }
        
        // current node becomes 0
        TNode *r = init_node(0, NULL, NULL, n->parent);
        free_tree(n);
        return r;
    }
    
    if (n->left) {
        n->left = explode_node(n->left, level + 1, changed);
        if (*changed) {
            return n;
        }
    }
    if (n->right) {
        n->right = explode_node(n->right, level + 1, changed);
    }
    
    return n;
}

TNode *split_node(TNode *n, int *changed) {
    if (!n->left && !n->right && n->number >= 10) {
        TNode *r = init_node(0, NULL, NULL, n->parent);
        r->left = init_node((int)floorf((float)n->number * 0.5f), NULL, NULL, r);
        r->right = init_node((int)ceilf((float)n->number * 0.5f), NULL, NULL, r);
        *changed = 1;
        free_tree(n);
        return r;
    }
    
    if (n->left) {
        n->left = split_node(n->left, changed);
        if (*changed) {
            return n;
        }
    }
    if (n->right) {
        n->right = split_node(n->right, changed);
    }
    
    return n;
}

TNode *reduce_node(TNode *n, int level, int *changed) {
    if (level >= 4 && n->left && n->right && is_number_node(n->left) && is_number_node(n->right)) {
        printf("explode:\n");
        // explode
        *changed = 1;
        
        // add to the left
        TNode *t = n;
        // go up until we are the left child
        while (t && t->parent && t->parent->left == t) {
            t = t->parent;
        }
        // we are either at the top or we are the right child
        if (t && t->parent && t->parent->right == t) {
            // now travel down
            t = t->parent->left;
            while (t->right) {
                t = t->right;
            }
            // we found the closes left number node
            t->number += n->left->number;
        }
        
        // add to the right
        t = n;
        // go up until we are the right child
        while (t && t->parent && t->parent->right == t) {
            t = t->parent;
        }
        // we are either at the top or we are the left child
        if (t && t->parent && t->parent->left == t) {
            // now travel down
            t = t->parent->right;
            while (t->left) {
                t = t->left;
            }
            // we found the closest right number node
            t->number += n->right->number;
        }
        
        // current node becomes 0
        TNode *r = init_node(0, NULL, NULL, n->parent);
        free_tree(n);
        return r;
    }
    
    if (!n->left && !n->right && n->number >= 10) {
        printf("split:\n");
        TNode *r = init_node(0, NULL, NULL, n->parent);
        r->left = init_node((int)floorf((float)n->number * 0.5f), NULL, NULL, r);
        r->right = init_node((int)ceilf((float)n->number * 0.5f), NULL, NULL, r);
        *changed = 1;
        free_tree(n);
        return r;
    }
    
    if (n->left) {
        n->left = reduce_node(n->left, level + 1, changed);
        if (*changed) {
            return n;
        }
    }
    if (n->right) {
        n->right = reduce_node(n->right, level + 1, changed);
    }
    
    return n;
}

TNode *add(TNode *a, TNode *b) {
    TNode *sum = init_node(0, copy_node(a, NULL), copy_node(b, NULL), NULL);
    
    printf("After addition:\n");
    print_tree(sum);
    printf("\n");
    
    int changed = 0;
    do {
        changed = 0;
        sum = explode_node(sum, 0, &changed);
        if (!changed) {
            sum = split_node(sum, &changed);
        }
    } while (changed);
    
    return sum;
}

long long magnitude(TNode *n) {
    if (!n->left && !n->right) {
        return n->number;
    }
    return magnitude(n->left) * 3 + magnitude(n->right) * 2;
}

const int MAX = 100;

int main(int argc, const char * argv[]) {
    FILE *f = fopen("input.txt", "r");
    TNode *numbers[MAX];
    char s[256];
    int numbers_len = 0;
    int read_idx = 0;
    
    while (fscanf(f, "%s", s) == 1 && strlen(s) > 0) {
        read_idx = 0;
        numbers[ numbers_len++ ] = read_number(s, &read_idx, NULL);
        print_tree(numbers[numbers_len - 1]);
        printf("\n");
    }
    fclose(f);
    
    for (int i = 1; i < numbers_len; i += 1) {
        TNode *r = add(numbers[0], numbers[i]);
        free_tree(numbers[0]);
        numbers[0] = r;
    }

    printf("Result:\n");
    print_tree(numbers[0]);
    printf("\n");
    
    printf("Magnitude: %lld\n", magnitude(numbers[0]));
    
    for (int i = 0; i < numbers_len; i += 1) {
        free_tree(numbers[i]);
    }
    
    return 0;
}
