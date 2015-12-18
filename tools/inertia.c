
// this one make the inertia matrix

#include <stdio.h>
#include <stdlib.h>

int i;
unsigned long long int TIME;
float *x;
float *y;
float *z;
float *inertia;

static inline void recenter(void) {
  float xm, ym, zm;
  xm = 0; ym = 0; zm = 0;

  for (int j = 0; j < i; j++) {
    xm += x[j];
    ym += y[j];
    zm += z[j];
  }
  xm /= i; ym /= i; zm /= i;

  for (int j = 0; j < i; j++) {
    x[j] -= xm;
    y[j] -= ym;
    z[j] -= zm;
  }
}


void misura() {

  recenter();

  for (int j = 0; j < 9; j++)
    inertia[j] = 0;

  float v[3];
  for (int j = 0; j < 3; j++)
    for (int k = 0; k < 3; k++)
      for (int m = 0; m < i; m++) {
	v[0] = x[m]; v[1] = y[m]; v[2] = z[m];

	inertia[3*j + k] += 
	  (j == k ? v[0]*v[0] + v[1]*v[1] + v[2]*v[2] : 0)
	  - v[j]*v[k];
      }

  for (int j = 0; j < 9; j++)
    inertia[j] /= i;

  printf("%llu", TIME);
  for (int j = 0; j < 9; j++)
    printf("\t%g", inertia[j]);
  putchar('\n');

};


int main (void) {
  TIME = -5;
  i = 0;
  x = malloc(sizeof(float)*30000);
  y = malloc(sizeof(float)*30000);
  z = malloc(sizeof(float)*30000);
  inertia = malloc(sizeof(float) * 9);

  for (int j = 0; j < 30000; j++) {
    x[j] = 0;
    y[j] = 0;
    z[j] = 0;
  }

  unsigned long long int t;
  float xyz[3];

  while (scanf("%llu\t%f\t%f\t%f\n", &t, xyz, xyz+1, xyz+2) == 4) {
    if (TIME == -5)
      TIME = t;

    if (t != TIME) {
      misura();

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
