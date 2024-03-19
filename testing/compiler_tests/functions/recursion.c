int f(int k) {
  if (k > 0) {
    return k + f(k - 1);
  } else {
    return 0;
  }
}
