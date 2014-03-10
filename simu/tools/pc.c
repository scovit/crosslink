
// this one make the inertia matrix

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

int i;
unsigned long long int TIME;
float *restrict x;
float *restrict y;
float *restrict z;
int *restrict contacts;
int *restrict norm;
int N;
double Sigma;
double threshold;

int misura() {
  if(i != N) {
    fprintf(stderr, "error: Format error in input\n");
    return -1;
  }

  for (int j = 0; j < N; j++)
    for (int k = j; k < N; k++) {
      if ((((double)(x[j] - x[k]))*((double)(x[j] - x[k])) +
	   ((double)(y[j] - y[k]))*((double)(y[j] - y[k])) +
	   ((double)(z[j] - z[k]))*((double)(z[j] - z[k])) < threshold)) {
	contacts[k - j]++;
      }
      norm[k - j]++;
    }

  printf("%llu", TIME);
  for (int j = 0; j < N; j++)
    printf("\t%g", ((double)contacts[j])/norm[j]);
  putchar('\n');

  return 0;
};

int main (int argc, char **argv) {
  TIME = -5;
  i = 0;

  if (argc != 4) {
    fprintf(stderr, "Usage: %s N Sigma\n", argv[0]);
    exit(-3);
  }
  N = atoi(argv[1]);
  Sigma = atof(argv[2]);

  x = malloc(3 * sizeof(float) * N + 2 * sizeof(int) * N);
  y = x + N;
  z = y + N;
  contacts = (int *)(z + N);
  norm = contacts + N;
  memset(x, 0, 3 * sizeof(float) * N + 2 * sizeof(int) * N);

  threshold = 1.44224957 * cbrt(3.0 * Sigma / (4.0 * N * M_PI));
  threshold = threshold * threshold;

  unsigned long long int t;
  float xyz[3];

  while (scanf("%llu\t%f\t%f\t%f\n", &t, xyz, xyz+1, xyz+2) == 4) {
    if (TIME == -5)
      TIME = t;

    if (t != TIME) {
      if(misura())
	exit(-1);

      TIME = t;

      i = 0;
    }

    x[i] = xyz[0];
    y[i] = xyz[1];
    z[i] = xyz[2];
    i++;
  }

  misura();
 
}
