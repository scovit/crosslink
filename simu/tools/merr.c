
// this one make the end to end position in function of s
// and print the whole function for every time

#include <stdio.h>
#include <stdlib.h>

int i;
unsigned long long int TIME;
float *x;
float *y;
float *z;
float *ss;
int *norm;

int main (void) {
  long long int TIME = -5;
  int i = 0;
  int n = 0;

  double *restrict x; x = calloc(sizeof(double), 10000*10000);
  double *restrict mean; mean = calloc(sizeof(double), 10000);
  double *restrict M2; M2 = calloc(sizeof(double), 10000);
  double *restrict M3; M3 = calloc(sizeof(double), 10000);
  double *restrict M4; M4 = calloc(sizeof(double), 10000);

  long long int t;
  double val;

  while (scanf("%lld %lg", &t, &val) == 2) {
    if (TIME == -5)
      TIME = t;

    if (t != TIME) {
      TIME = t;
      i = 0;
      n++;
    }

    x[30000*i + n] = val;
    i++;
  }
  n++;

  for (int k = 0; k < i; k++)
    for(int j = 0; j < n; j++) 
      mean[k] += x[30000*k + j];

  for (int k = 0; k < i; k++)
    mean[k] /= n;

  for (int k = 0; k < i; k++)
    for(int j = 0; j < n; j++) {
      val = x[30000*k + j] - mean[k];
      M2[k] += val*val;
      M3[k] += M2[k]*val;
      M4[k] += M3[k]*val;
    }

  for (int k = 0; k < i; k++)
    printf("%d %lg %lg %lg %lg\n", k, mean[k],
	   M2[k] / n, M3[k] / n, M4[k] /= n);

}
