#ifndef DEF_SIMULAZIONE_H
#define DEF_SIMULAZIONE_H

#if defined(__cplusplus)
#include <cstdio>
#include <cstdlib>
#include <cstdint>
extern "C" {
#else
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#endif
#include "dSFMT-src-2.2.1/dSFMT.h"
#include "infofile/infofile.h"

  // RNG state
  dsfmt_t dsfmt;

  // info file
  struct infofile *infos;

  // number of elements
  int N;
  // elements
  size_t buffer_size;
  __attribute__ ((aligned (32))) void *restrict buffer;

  __attribute__ ((aligned (32))) struct {
    float *x;
    float *y;
    float *z;
  } dots;

  // link allocation out degree
  int ODGRMAX;

  __attribute__ ((aligned (32))) int *restrict lpl;
  __attribute__ ((aligned (32))) int *restrict lpl_index;
#if ((NUM_THREADS > 1) && defined(XLINK))
  __attribute__ ((aligned (32))) int *restrict crx;
  __attribute__ ((aligned (32))) int *restrict crx_index;
#endif

#ifdef LOCALIZED
  __attribute__ ((aligned (32))) uint8_t *restrict locmask;
  int locnum;
#endif

  // time
  struct mc_time_t {
    unsigned long long t;
    unsigned long long DYN_STEPS;
    unsigned long long RELAX_TIME;
    unsigned long long CORRL_TIME;
    int STATISTIC;
  };
  __attribute__ ((aligned (32))) struct mc_time_t mc_time;

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
#if defined(GETPERF)
    FILE *accfile;
#endif
#if defined(GETENERGY)
    FILE *ctcfile;
#endif
#if defined(GETXLINK) 
    FILE *xlkfile;
#endif
#if defined(GETXYZ)
    FILE *xyzfile;
#endif
  };
  __attribute__ ((aligned (32))) struct simufiles_t simufiles;

  __attribute__ ((noinline)) void closefiles();
  __attribute__ ((noinline)) void flushfiles();
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
