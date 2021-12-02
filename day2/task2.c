#include <stdio.h>
#include <string.h>

int main() {
  FILE *f = fopen("input.txt", "r");
  long long v = 0, h = 0, x = 0, r, aim = 0;
  char step[8];
  while ( fscanf(f, "%s %lld", step, &x) == 2 ) {
    if (!strcmp(step, "forward")) {
      h += x;
      v += aim * x;
    } else if (!strcmp(step, "up")) {
      aim -= x;
    } else {
      aim += x;
    }
  }
  fclose(f);
  r = v * h;
  printf("%lld\n", r);
}