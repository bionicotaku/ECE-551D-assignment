#include "rand_story.h"

int main(int argc, char * argv[]) {
  char * wordsFileName;
  char * storyFileName;
  int nFlag = 0;

  switch (argc) {
    case 3:
      wordsFileName = argv[1];
      storyFileName = argv[2];
      break;
    case 4:
      if (strcmp(argv[1], "-n") == 0) {
        wordsFileName = argv[2];
        storyFileName = argv[3];
        nFlag = 1;
        break;
      }
      else {
        fprintf(stderr, "cannot recognize n flag\n");
        exit(EXIT_FAILURE);
      }
    default:
      fprintf(stderr, "wrong number of argc\n");
      exit(EXIT_FAILURE);
      break;
  }

  FILE * wordsFile = fopen(wordsFileName, "r");
  if (wordsFile == NULL) {
    fprintf(stderr, "cannot open words file\n");
    exit(EXIT_FAILURE);
  }

  FILE * storyFile = fopen(storyFileName, "r");
  if (storyFile == NULL) {
    fprintf(stderr, "cannot open story file\n");
    exit(EXIT_FAILURE);
  }

  catarray_t words;
  readWords(wordsFile, &words);
  readStory(storyFile, &words, nFlag);
  fclose(wordsFile);
  fclose(storyFile);
  freeWords(&words);

  return EXIT_SUCCESS;
}
