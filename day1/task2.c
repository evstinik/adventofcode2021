#include <stdio.h>

int main() {
  FILE *f = fopen("input.txt", "r");
  unsigned int prev_sum = 0, sum = 0, depths[3] = { 0, 0, 0 }, increases_count = 0, numbers_read = 0;
  while ( fscanf(f, "%d", &depths[2]) == 1 ) {
    numbers_read += 1;
    sum += depths[2];
    if ( (numbers_read > 3) && (prev_sum < sum) ) {
      increases_count += 1;
    }
    prev_sum = sum;
    sum -= depths[0];
    depths[0] = depths[1];
    depths[1] = depths[2];
  }
  fclose(f);
  printf("%d\n", increases_count);
}