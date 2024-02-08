#include "rand_story.h"

int main(int argc, char * argv[]) {
  if (argc != 3) {
    fprintf(stderr, "wrong number of argc\n");
    exit(EXIT_FAILURE);
  }

  FILE * wordsFile = fopen(argv[1], "r");
  if (wordsFile == NULL) {
    fprintf(stderr, "cannot open words file\n");
    exit(EXIT_FAILURE);
  }

  FILE * storyFile = fopen(argv[2], "r");
  if (storyFile == NULL) {
    fprintf(stderr, "cannot open story file\n");
    exit(EXIT_FAILURE);
  }

  catarray_t words;
  readWords(wordsFile, &words);
  readStory(storyFile, &words, 0);
  fclose(wordsFile);
  fclose(storyFile);
  freeWords(&words);

  return EXIT_SUCCESS;
}
