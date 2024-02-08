#ifndef __RAND_STORY_H__
#define __RAND_STORY_H__

#include <stdio.h>
#include <string.h>

#include "provided.h"
//any functions you want your main to use
void deleteValue(const char * value, category_t * catArray);
void chooseWithInt(char * cat, catarray_t * catArray, category_t * memArray, int nFlag);
void readStory(FILE * f, catarray_t * array, int nFlag);
void readWords(FILE * f, catarray_t * array);
void freeCategory(category_t * cat);
void freeWords(catarray_t * array);

#endif
