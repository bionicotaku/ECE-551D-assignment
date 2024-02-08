#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y);

void run_check(unsigned x, unsigned y, unsigned expected_ans) {
  unsigned result = power(x, y);
  if (result != expected_ans) {
    printf("Fail: %u^%u = %u, expected %u\n", x, y, result, expected_ans);
    exit(EXIT_FAILURE);
  }
}

int main() {
  run_check(0, 0, 1);
  run_check(10000, 2, 100000000);
  run_check(2, 5, 32);
  run_check(2, 2, 4);
  run_check(1, 123123123, 1);
  run_check(0, 100, 0);
  run_check(0, 1, 0);
  run_check(123, 0, 1);
  run_check(121, 1, 121);
  run_check(1, 123, 1);
  run_check(1, 1, 1);
  run_check(65535, 2, 4294836225);
  run_check(1, -1, 1);
  run_check(-1, 1, -1);
  run_check(0, -1, 0);
  run_check(-1, 0, 1);
  run_check(2, 3, 8);
  printf("all correct.\n");
  exit(EXIT_SUCCESS);
}
