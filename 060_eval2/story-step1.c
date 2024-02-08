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
  readStory(f, NULL, 0);
  fclose(f);

  return EXIT_SUCCESS;
}
