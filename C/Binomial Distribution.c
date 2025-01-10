#include <stdio.h>
#include <math.h>

double binomialDistribution(int n, int k, double p) {
  double c = 1.0;
  for (int i = 1; i <= k; i++) {
    c = c * (n - (k - i)) / i;
  }

  return c * pow(p, k) * pow(1-p, n-k);
}

int main() {
  int n = 20;
  int k = 19;
  double p = 0.5;

  double result = binomialDistribution(n, k, p);

  printf("二項分佈計算結果: %lf\n", result);
  system("pause");

  return 0;
}
