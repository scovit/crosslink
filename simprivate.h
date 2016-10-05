
#ifndef DEF_SIMPRIVATE_H
#define DEF_SIMPRIVATE_H

static dsfmt_t dsfmt;

#define unlikely(expr) __builtin_expect(!!(expr), 0)
#define likely(expr) __builtin_expect(!!(expr), 1)

/* #define SIGMA  0.1f /\*1. Diametro di hard core *\/ */
/* #define LAMBDA  0.19f /\* 1.90 Massima distanza monomeri collegati *\/ */
/*
  #define ALFA_UNIFORM 0.3f
  #define ALFA_LOCALIZED 0.19f
*/

/* variables */
float lambda;
#if (defined(HARDCORE) || defined (UNIFORM) ||	\
     defined (LOCALIZED) || defined(XLINK))
float sigma;
#endif
#if (defined(UNIFORM) || defined(XLINK))
float alfa_uniform;
double beta_uniform;
#endif
#if defined(LOCALIZED)
float alfa_localized;
double beta_localized;
#endif
#if defined(CONFINEMENT)
float conf_sqradius;
#endif
#if defined(XLINK)
double xlink_conc;
#endif

/* Comparisons */
#if defined(HARDCORE)
static __m256 comp_hc __attribute__ ((aligned (32)));
#endif
#if defined(UNIFORM)
static __m256 comp_sb __attribute__ ((aligned (32)));
#endif
#if defined(LOCALIZED)
static __m256 comp_sp __attribute__ ((aligned (32)));
#endif
#if defined(TOPO)
static __m256 comp_top __attribute__ ((aligned (32)));
#endif
#if defined(XLINK)
static __m256 comp_xl __attribute__ ((aligned (32)));
#endif

static float D; /* Displacement */

// number of contacts
#ifdef UNIFORM
static int contacts;
#endif
#ifdef LOCALIZED
static int contacts_loc;
#endif

#ifdef TOPO
// list of possible topological barrier break
static int topolistlength;
static int *restrict topolist;
#endif

#ifdef XLINK
// list of possible topological barrier break
static int xlinklistlength;
static int *restrict xlinklist;
#endif

#ifdef GETPERF
// displacement
static float displ;
#endif

static const char *siminfostring = 
  "# infos about the simulation\n"
  "STARTTIME=\"%s\"\n"
  "N=%d\n"
  "cfgfile=\"%s\"\n"
  "lplfile=\"%s\"\n"
  "sigma=%g\n"
  "lambda=%g\n"
  "D=%g\n"
  "alfa_uniform=%g\n"
  "alfa_localized=%g\n"
  "beta_uniform=%g\n"
  "beta_localized=%g\n"
  "conf_sqradius=%g\n"
  "xlink_conc=%g\n"
  "seed=%u\n"
  "RELAX_TIME=%llu\n"
  "CORRL_TIME=%llu\n"
  "STATISTIC=%d\n"
  "DYN_STEPS=%llu\n";

#endif
