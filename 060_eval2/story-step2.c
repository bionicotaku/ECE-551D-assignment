#include "rand_story.h"

int main(int argc, char * argv[]) {
  if (argc != 2) {
    fprintf(stderr, "wrong number of argc\n");
    exit(EXIT_FAILURE);
  }

  FILE * f = fopen(argv[1], "r");
  if (f == NULL) {
    fprintf(stderr, "cannot open file\n");
    exit(EXIT_FAILURE);
  }
  
  catarray_t words;
  readWords(f, &words);
  printWords(&words);
  fclose(f);
  freeWords(&words);

  return EXIT_SUCCESS;
}
