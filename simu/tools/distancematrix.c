#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


const int maxdots = 4096;

int i;
int iold;
int N = 0;

struct {
  float * restrict x;
  float * restrict y;
  float * restrict z;
} dots;

double * restrict distance;

static void usage(int argc, char *argv[]) {
  printf ("Usage: %s\n", argv[0]);
  exit (-1);
}

static void allocate_memory() {
  float * restrict buffer = malloc(3 * sizeof(float) * maxdots);
  dots.x = buffer; dots.y = dots.x + maxdots; dots.z = dots.y + maxdots;

  distance = malloc(sizeof(float) * maxdots * maxdots);
}

static void free_memory() {
  free(dots.x);
  free(distance);
}

static inline float square_distance(int a, int b) {
  return (dots.x[a] - dots.x[b]) * (dots.x[a] - dots.x[b]) +
    (dots.y[a] - dots.y[b]) * (dots.y[a] - dots.y[b]) +
    (dots.z[a] - dots.z[b]) * (dots.z[a] - dots.z[b]);
}

static void core(long long int t) {
  N++;
  for(int j = 0; j < i; j++)
    for(int k = 0; k < i; k++)
      distance[j*i + k] += square_distance(j, k);
}

static void print_distances() {
  for(int j = 0; j < i; j++)
    for(int k = 0; k < i; k++) {
      distance[j*i + k] /= N;
      printf("%g\n", distance[j*i + k]);
    }
};

static inline void check_i(unsigned long long int t) {
  if ((i != iold) && (iold != -1)) {
    fprintf(stderr, "Error, number of dots is not costant, "
	            "t = %llu, i = %d, iold = %d\n",
	    t, i, iold);
    exit(-1);
  }
}

// This just fills the dots for core
int main(int argc, char **argv) {

  if (argc != 1)
    usage(argc, argv);

  long long int t;
  long long int oldt = -1234567;
  int notfirstrun = 0;

  i = 0;
  iold = -1;

  allocate_memory();

  while (scanf("%llu %g %g %g", &t, dots.x + i, dots.y + i, dots.z + i)
	 == 4) {
    i++;

    if (oldt == -1234567)
      oldt = t;

    if (t != oldt) {
      i--;

      check_i(t);

      core(oldt);

      dots.x[0] = dots.x[i+1]; 
      dots.y[0] = dots.y[i+1]; 
      dots.z[0] = dots.z[i+1];
      iold = i;
      i = 1;
      oldt = t;
    }

    if (i >= maxdots) {
      fprintf(stderr, "maxdots too small, aborting!!!\n");
      free_memory();
      exit(-1);
    }
  }

  check_i(t);

  core(t);

  print_distances();

  free_memory();
  return 0;

}
