#include <stdio.h>

int main() {
  FILE *f = fopen("input.txt", "r");
  int prev_depth = -1, depth = 0, increases_count = 0;
  while ( fscanf(f, "%d", &depth) == 1 ) {
    if ( (prev_depth >= 0) && (depth > prev_depth) ) {
      increases_count += 1;
    }
    prev_depth = depth;
  }
  fclose(f);
  printf("%d\n", increases_count);
}