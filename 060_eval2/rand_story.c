#include "rand_story.h"

int isDigits(const char * str) {
  while (*str) {
    if (*str < '0' || *str > '9') {
      return 0;
    }
    str++;
  }
  return 1;
}

void deleteValue(const char * value, category_t * catArray) {
  for (size_t i = 0; i < catArray->n_words; i++) {
    if (strcmp(value, catArray->words[i]) == 0) {
      free(catArray->words[i]);
      for (size_t j = i; j < catArray->n_words - 1; j++) {
        catArray->words[j] = catArray->words[j + 1];
      }
      catArray->n_words--;
      return;
    }
  }
}

void chooseWithInt(char * category,
                   catarray_t * catArray,
                   category_t * memArray,
                   int nFlag) {
  if (catArray == NULL) {
    printf("%s", chooseWord(category, catArray));
    return;
  }

  char * choosed = NULL;
  int catExists = 0;
  if (isDigits(category)) {
    int digit = atoi(category);
    // if choose the previous category
    if (digit > 0 && digit <= memArray->n_words) {
      catExists = 1;
      choosed = strdup(memArray->words[memArray->n_words - digit]);
    }
  }

  // check if the category exists
  for (size_t i = 0; i < catArray->n && !catExists; i++) {
    if (strcmp(catArray->arr[i].name, category) == 0) {
      catExists = 1;
      choosed = strdup(chooseWord(category, catArray));
      if (nFlag) {
        deleteValue(choosed, &catArray->arr[i]);
      }
    }
  }

  if (!catExists) {
    fprintf(stderr, "category doesn't exist\n");
    freeCategory(memArray);
    freeWords(catArray);
    exit(EXIT_FAILURE);
  }
  else {
    printf("%s", choosed);
    memArray->n_words++;
    memArray->words =
        realloc(memArray->words, memArray->n_words * sizeof(*memArray->words));
    memArray->words[memArray->n_words - 1] = choosed;
  }
}

void readStory(FILE * f, catarray_t * array, int nFlag) {
  int ch;
  int blankBegin = 0;
  char * category = malloc(sizeof(*category));
  int catLength = 0;
  category[0] = '\0';

  category_t memory = {NULL, NULL, 0};

  while ((ch = fgetc(f)) != EOF) {
    if (ch == '_') {
      // the end of blank, read a correct blank
      if (blankBegin) {
        chooseWithInt(category, array, &memory, nFlag);

        blankBegin = 0;
        catLength = 0;
        category = realloc(category, sizeof(*category));
        category[0] = '\0';
      }
      // the begin of blank
      else {
        blankBegin = 1;
      }
    }
    else {
      // read the category
      if (blankBegin) {
        if (ch == '\n') {
          fprintf(stderr, "wrong format of blank\n");
          fclose(f);
          free(category);
          freeCategory(&memory);
          exit(EXIT_FAILURE);
        }
        catLength++;
        category = realloc(category, (catLength + 1) * sizeof(*category));
        category[catLength - 1] = ch;
        category[catLength] = '\0';
      }
      // correct for now
      else {
        putchar(ch);
      }
    }
  }
  // cannot find another '_'
  if (blankBegin) {
    fprintf(stderr, "cannot find another _ till the end of file\n");
    fclose(f);
    freeCategory(&memory);
    free(category);
    exit(EXIT_FAILURE);
  }
  // success
  free(category);
  freeCategory(&memory);
}

void readWords(FILE * f, catarray_t * array) {
  array->arr = NULL;
  array->n = 0;

  char * line = NULL;
  size_t len = 0;

  while (getline(&line, &len, f) != -1) {
    char * colon = strchr(line, ':');
    if (colon == NULL) {
      fprintf(stderr, "cannot find ':'\n");
      fclose(f);
      freeWords(array);
      exit(EXIT_FAILURE);
      break;
    }
    *colon = '\0';
    char * value = colon + 1;
    value[strlen(value) - 1] = '\0';  // Remove newline

    int catExist = 0;
    for (size_t i = 0; i < array->n; ++i) {
      if (strcmp(array->arr[i].name, line) == 0) {
        array->arr[i].n_words++;
        array->arr[i].words = realloc(
            array->arr[i].words, array->arr[i].n_words * sizeof(*array->arr[i].words));
        array->arr[i].words[array->arr[i].n_words - 1] = strdup(value);
        catExist = 1;
        break;
      }
    }

    if (!catExist) {
      array->n++;
      array->arr = realloc(array->arr, array->n * sizeof(*array->arr));
      array->arr[array->n - 1].n_words = 1;
      array->arr[array->n - 1].name = strdup(line);
      array->arr[array->n - 1].words = malloc(sizeof(*array->arr[array->n - 1].words));
      array->arr[array->n - 1].words[0] = strdup(value);
    }
  }

  free(line);
}

void freeCategory(category_t * cat) {
  free(cat->name);
  for (size_t i = 0; i < cat->n_words; i++) {
    free(cat->words[i]);
  }
  free(cat->words);
}

void freeWords(catarray_t * array) {
  for (size_t i = 0; i < array->n; ++i) {
    freeCategory(&array->arr[i]);
  }
  free(array->arr);
}
