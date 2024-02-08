#include <stdio.h>
#include <stdlib.h>

struct _retire_info {
  int months;
  double contribution;
  double rate_of_return;
};

typedef struct _retire_info retire_info;

void retirement(int startAge, double initial, retire_info working, retire_info retired) {
  double balance = initial;
  int age_years = startAge / 12;
  int age_months = startAge % 12;

  printf("Age %3d month %2d you have $%.2f\n", age_years, age_months, balance);
  for (int i = 0; i < working.months; i++) {
    balance += balance * working.rate_of_return + working.contribution;
    if (age_months == 11) {
      age_years++;
      age_months = 0;
    }
    else {
      age_months++;
    }
    printf("Age %3d month %2d you have $%.2f\n", age_years, age_months, balance);
  }

  for (int i = 1; i < retired.months; i++) {
    balance += balance * retired.rate_of_return + retired.contribution;
    if (age_months == 11) {
      age_years++;
      age_months = 0;
    }
    else {
      age_months++;
    }
    printf("Age %3d month %2d you have $%.2f\n", age_years, age_months, balance);
  }
}

int main() {
  retire_info working;
  working.months = 489;
  working.contribution = 1000.0;
  working.rate_of_return = 0.045 / 12.0;

  retire_info retired;
  retired.months = 384;
  retired.contribution = -4000.0;
  retired.rate_of_return = 0.01 / 12.0;

  int start_age = 327;
  double init_balance = 21345.0;

  retirement(start_age, init_balance, working, retired);

  return 0;
}
