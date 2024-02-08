#include "sunspots.h"
ss_monthly_t parseLine(char * line) {
  // WRITE ME
  ss_monthly_t ans;
  // if length is not enough
  if (line == NULL || strlen(line) < 9) {
    fprintf(stderr, "number of chars is too small\n");
    exit(EXIT_FAILURE);
  }

  // seperate the numbers
  if (line[4] != '-' || line[7] != ',') {
    fprintf(stderr, "wrong number of digits\n");
    exit(EXIT_FAILURE);
  }
  else {
    line[4] = '\0';
    line[7] = '\0';
  }

  // check digits
  for (int i = 0; i < 7; i++) {
    if (i == 4)
      continue;
    if (line[i] < '0' || line[i] > '9') {
      fprintf(stderr, "expecting a digit char\n");
      exit(EXIT_FAILURE);
    }
  }
  ans.year = atoi(line);
  ans.month = atoi(line + 5);
  ans.num = atof(line + 8);

  // check value of month
  if (ans.month >= 1 && ans.month <= 12) {
    return ans;
  }
  else {
    fprintf(stderr, "wrong number for month\n");
    exit(EXIT_FAILURE);
  }
}

void meanFilter(ss_monthly_t * data, size_t n, ss_monthly_t * mean, unsigned w) {
  // WRITE ME
  if (w % 2 == 0 || w > n || n == 0 || data == NULL) {
    fprintf(stderr, "wrong number of n or w\n");
    exit(EXIT_FAILURE);
  }

  int half = w / 2;
  for (int i = 0; i < n; i++) {
    double total = 0;
    int window = 0;
    for (int j = i - half; j <= i + half; j++) {
      if (j >= 0 && j < n) {
        total += data[j].num;
        window++;
      }
    }
    mean[i].num = total / window;
    mean[i].month = data[i].month;
    mean[i].year = data[i].year;
  }
}

double findLocalMax(ss_monthly_t * data, size_t n) {
  // WRITE ME
  if (n == 0 || data == NULL) {
    fprintf(stderr, "n = 0 \n");
    exit(EXIT_FAILURE);
  }

  size_t max_time = 0;
  double max = -1;

  for (size_t i = 0; i < n; i++) {
    if (data[i].num > max) {
      max = data[i].num;
      max_time = i;
    }
  }

  return data[max_time].year + data[max_time].month / 12.0;
}

double calcSsPeriod(double * timeStamps, size_t n) {
  //WRITE ME
  if (n < 2 || timeStamps == NULL) {
    fprintf(stderr, "wrong value of n \n");
    exit(EXIT_FAILURE);
  }

  double difference = 0;
  for (size_t i = 0; i < n - 1; i++) {
    difference += timeStamps[i + 1] - timeStamps[i];
  }

  return difference / (n - 1);
}
