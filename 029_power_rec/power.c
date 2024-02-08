unsigned power(unsigned x, unsigned y) {
  if (y == 0) {
    return 1;
  }
  else if (y % 2 == 0) {
    unsigned half_power = power(x, y / 2);
    return half_power * half_power;
  }
  else {
    return x * power(x, y - 1);
  }
}
