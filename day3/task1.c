#include <stdio.h>
#include <string.h>

const int MAX_DIGITS = 16;

int main() {
  int zeros[MAX_DIGITS], ones[MAX_DIGITS], digits = 0, gamma_rate = 0, epsilon_rate = 0, pow_2 = 1;
  char n[MAX_DIGITS];

  memset(zeros, 0, sizeof(int) * MAX_DIGITS);
  memset(ones, 0, sizeof(int) * MAX_DIGITS);

  FILE *f = fopen("input.txt", "r");
  while ( fscanf(f, "%s", n) == 1 ) {
    if (digits == 0) {
      digits = strlen(n);
    }
    for (int i = 0; i < digits; i += 1) {
      if (n[i] == '0') {
        zeros[i] += 1;
      } else if (n[i] == '1') {
        ones[i] += 1;
      }
    }
  }
  fclose(f);

  for (int i = digits - 1; i >= 0; i -= 1) {
    if (ones[i] >= zeros[i]) {
      gamma_rate += pow_2;
    } else {
      epsilon_rate += pow_2;
    }
    pow_2 *= 2;
  }

  printf("%d * %d = %d\n", gamma_rate, epsilon_rate, gamma_rate * epsilon_rate);
}