
// this one make the end to end position in function of s
// and print the whole function for every time

#include <stdio.h>
#include <stdlib.h>

unsigned int i;

unsigned long long int *time;
double *value;

unsigned int *norm;
double *R;

int main (void) {
  i = 0;
  time = malloc(sizeof(unsigned long long int) * 100000000);
  value = malloc(sizeof(double) * 100000000);

  while (scanf("%llu\t%lf\n", &(time[i]), &(value[i])) == 2)
    i++;

  // calculate mean and variance
  double sum1 = 0;
 
  for (unsigned int j = 0; j < i; j++)
    sum1 = sum1 + value[j];

  double mean = sum1/i;

  double sum2 = 0;
  double sum3 = 0;
  for (unsigned int j = 0; j < i; j++) {
    sum2 = sum2 + (value[j] - mean)*(value[j] - mean);
    sum3 = sum3 + (value[j] - mean);
  }
  double variance = (sum2 - sum3*sum3/i)/(i - 1);

  // real calculation
  norm = malloc(sizeof(int) * i);
  R = malloc(sizeof(double) * i);
  for (unsigned int j = 0; j < i; j++) {
    norm[j] = 0;
    R[j] = 0.;
  }

  for (unsigned int j = 0; j < i; j++)
    for (unsigned int s = j; s < i; s++) {
      R[s - j] += (value[s] - mean)*(value[j] - mean)/variance;
      norm[s - j]++;
    }

  unsigned long long int deltat = time[1] - time[0];
  for (unsigned long long int j = 0; j < i; j++) {
    R[j] /= norm[j];
    printf("%llu\t%lf\n", j*deltat, R[j]);
  }
}
