
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
int N;
float Sigma;
float threshold;
int locnum;
uint8_t *restrict locmask;
uint8_t *restrict adiacence;
int *restrict group;
int numgroup;

static inline bool is_loc_interacting(const int m) {
  return (locmask[m/4] & (1 << (m & 3)));
}

void misura() {
  if(i != N) {
    fprintf(stderr, "Format error in input\n");
    exit(-2);
  }

  memset(adiacence, 0, sizeof(uint8_t) * numgroup * numgroup);

  float v[3];
  for (int j = 0; j < N; j++)
    for (int k = j; k < N; k++)
      if (is_loc_interacting(j) &&
	  is_loc_interacting(k) && 
	  ((x[j] - x[k])*(x[j] - x[k]) +
	   (y[j] - y[k])*(y[j] - y[k]) +
	   (z[j] - z[k])*(z[j] - z[k]) < threshold)) {
	adiacence[numgroup*group[j] + group[k]] = 1;
	adiacence[numgroup*group[k] + group[j]] = 1;
      }


  for (int m = 0; m < numgroup; m++)
    for (int j = 0; j < numgroup; j++)
      for (int k = j + 1; k < numgroup; k++)
	for (int l = k + 1; l < numgroup; l++)
	  if (adiacence[numgroup*j + k] +
	      adiacence[numgroup*k + l] +
	      adiacence[numgroup*j + l] == 2) {
	    adiacence[numgroup*j + k] = 1;
	    adiacence[numgroup*k + j] = 1;
	    adiacence[numgroup*k + l] = 1;
	    adiacence[numgroup*l + k] = 1;
	    adiacence[numgroup*j + l] = 1;
	    adiacence[numgroup*l + j] = 1;
	  }

  printf("%llu", TIME);
  for (int j = 0; j < numgroup*numgroup; j++)
    printf("\t%hhd", adiacence[j]);
  putchar('\n');

};

static __attribute__ ((noinline))
void load_localized_stuff(char *locfilepath) {
  for (unsigned int i = 0; i < N; i += 4)
    locmask[i / 4] = 0;
  locnum = 0;

  FILE *locfile;
  if ((locfile = fopen(locfilepath, "r")) == NULL) {
    fprintf(stderr, "Error opening file %s\n", locfilepath);
    exit(-1);
  } else {
    // load localized interaction from file
    unsigned int interagentlink;
    while(fscanf(locfile, "%d", &interagentlink) == 1)
      if (interagentlink < N)
	if (!is_loc_interacting(interagentlink)) {
	    locmask[interagentlink/4] |= (1 << (interagentlink & 3));
	    locnum++;
	  }

    fclose(locfile);
  } 
}

static __attribute__ ((noinline))
void set_group() {
  numgroup = 0;
  group = calloc(N, sizeof(int));

  for (int j = 0; j < N; j++)
    if (is_loc_interacting(j)) {
      if ((j != 0) && !is_loc_interacting(j-1))
	numgroup++;
      else if (j == 0)
	numgroup++;


      group[j] = numgroup - 1;
    }
}


int main (int argc, char **argv) {
  TIME = -5;
  i = 0;

  if (argc != 4) {
    fprintf(stderr, "Usage: %s N Sigma LOC\n", argv[0]);
    exit(-3);
  }
  N = atoi(argv[1]);
  Sigma = atof(argv[2]);

  x = malloc(3 * sizeof(float) * N + sizeof(uint8_t) * N);
  y = x + N;
  z = y + N;
  locmask = (uint8_t *)(z + N);
  memset(x, 0, 3 * sizeof(float) * N + sizeof(uint8_t) * N);

  load_localized_stuff(argv[3]);

  set_group();
  adiacence = calloc(numgroup*numgroup, sizeof(uint8_t));

  threshold = 1.44224957 * cbrt(3.0 * Sigma / (4.0 * N * M_PI));

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
