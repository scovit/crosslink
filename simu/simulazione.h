#ifndef DEF_SIMULAZIONE_H
#define DEF_SIMULAZIONE_H

#if defined(__cplusplus)
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <zlib.h>
extern "C" {
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <zlib.h>
#endif

  // number of elements
  int N;
  // elements
  __attribute__ ((aligned (16))) void *restrict buffer;

  __attribute__ ((aligned (16))) struct {
    float *x;
    float *y;
    float *z;
  } dots;

  // link allocation out degree
#define ODGRMAX 2

  __attribute__ ((aligned (16))) int *restrict lpl;
  __attribute__ ((aligned (16))) int *restrict lpl_index;

#ifdef LOCALIZED
  __attribute__ ((aligned (16))) uint8_t *restrict locmask;
  int locnum;
#endif

  // time
  struct mc_time_t {
    unsigned long long t;
    unsigned long long DYN_STEPS;
  };
  __attribute__ ((aligned (16))) struct mc_time_t mc_time;

  struct thread_data{
    int thread_id;
    int argc;
    char **argv;
  };

#if NUM_THREADS > 1
  pthread_spinlock_t spinsum;
  pthread_barrier_t startbarr;
  pthread_barrier_t firstbarr;
#endif

  struct simufiles_t {
    gzFile *xyzfile;
    gzFile *cnffile;
    FILE *accfile;
    FILE *ctcfile;
    FILE *inffile;
    FILE *lplfile;
    FILE *locfile;
    FILE *rndfile;
  };
  __attribute__ ((aligned (16))) struct simufiles_t simufiles;

  __attribute__ ((noinline)) void closefiles();
  void *simulazione(void *threadarg);

#if defined(__cplusplus)
}
#endif

#ifdef LOCALIZED
static inline bool is_loc_interacting(const int m) {
  return (locmask[m/4] & (1 << (m & 3)));
}
#endif

void print_buffer(FILE *out);

#endif
