#include <stdio.h>
#include <string.h>

int main() {
  FILE *f = fopen("input.txt", "r");
  long v = 0, h = 0, n = 0;
  long long r;
  char step[8];
  while ( fscanf(f, "%s %ld", step, &n) == 2 ) {
    if (!strcmp(step, "forward")) {
      h += n;
    } else if (!strcmp(step, "up")) {
      v -= n;
    } else {
      v += n;
    }
  }
  fclose(f);
  r = v * h;
  printf("%lld\n", r);
}