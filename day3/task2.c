#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const int MAX_DIGITS = 16;

typedef struct TNode {
  struct TNode *l, *r;
  int l_count, r_count;
} TNode;

TNode *init_node() {
  TNode *n = (TNode *)malloc(sizeof(TNode));
  memset(n, 0, sizeof(TNode));
  return n;
}

void free_node(TNode *n) {
  if (!n) return;
  free_node(n->l);
  free_node(n->r);
  free(n);
}

void print_node(TNode *n) {
  printf("zeros: %d (%dx), ones: %d (%dx)\n", !!n->l, n->l_count, !!n->r, n->r_count);
}

int main() {
  TNode *root = init_node(), *it = root;
  char n[MAX_DIGITS], digits = 0, ones_prevale = 0;
  int oxy_rating = 0, co2_rating = 0, pow_2 = 0, max_pow_2 = 0;
  
  // input -> binary tree
  FILE *f = fopen("input.txt", "r");
  while ( fscanf(f, "%s", n) == 1 ) {
    if (digits == 0) {
      digits = strlen(n);
    }
    it = root;
    for (int i = 0; i < digits; i += 1) {
      if (n[i] == '0') {
        it->l_count += 1;
        if (!it->l) {
          it->l = init_node();
        }
        it = it->l;
      } else if (n[i] == '1') {
        it->r_count += 1;
        if (!it->r) {
          it->r = init_node();
        }
        it = it->r;
      }
    } 
  }
  fclose(f);

  max_pow_2 = (int)pow(2, digits - 1);

  // oxygen generator rating (with dec->bin convertion)
  it = root;
  pow_2 = max_pow_2;
  while (it && ( it->r_count > 0 || it->l_count > 0 ) ) {
    ones_prevale = it->r_count >= it->l_count;
    if ( it->r && ( !it->l || ones_prevale ) ) {
      oxy_rating += pow_2;
      it = it->r;
    } else if ( it->l && ( !it->r || !ones_prevale ) ) {
      it = it->l;
    } else {
      break;
    }
    pow_2 = pow_2 / 2;
  }

  // CO2 scrubber rating (with dec->bin convertion)
  it = root;
  pow_2 = max_pow_2;
  while (it && ( it->r_count > 0 || it->l_count > 0 ) ) {
    ones_prevale = it->r_count >= it->l_count;
    if ( it->r && ( !it->l || !ones_prevale ) ) {
      co2_rating += pow_2;
      it = it->r;
    } else if ( it->l && ( !it->r || ones_prevale ) ) {
      it = it->l;
    } else {
      break;
    }
    pow_2 = pow_2 / 2;
  }

  printf("%d * %d = %d\n", oxy_rating, co2_rating, oxy_rating * co2_rating);
  free_node(root);
}