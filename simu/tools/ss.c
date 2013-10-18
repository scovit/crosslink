
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

void misura() {
  for (int j = 0; j < i; j++)
    for (int s = j; s < i; s++) {
      ss[s - j] += (x[s]-x[j])*(x[s]-x[j])+(y[s]-y[j])*(y[s]-y[j])+(z[s]-z[j])*(z[s]-z[j]);
      norm[s - j]++;
    }

    for (int j = 0; j < i; j++) {
      ss[j] /= norm[j];
      printf("%llu\t%g\n", TIME, ss[j]);
    }
};


int main (void) {
  TIME = -5;
  i = 0;
  ss = malloc(sizeof(float)*30000);
  norm = malloc(sizeof(int)*30000);
  x = malloc(sizeof(float)*30000);
  y = malloc(sizeof(float)*30000);
  z = malloc(sizeof(float)*30000);

  for (int j = 0; j < 30000; j++) {
    ss[j] = 0;
    norm[j] = 0;
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
      for (int j = 0; j < i; j++) {
	ss[j] = 0;
	norm[j] = 0;
      }
      i = 0;
    }

    x[i] = xyz[0];
    y[i] = xyz[1];
    z[i] = xyz[2];
    i++;
  }

  misura();
 
}
