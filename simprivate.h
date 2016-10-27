
#ifndef DEF_SIMPRIVATE_H
#define DEF_SIMPRIVATE_H

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
#if defined(HARDCORE)
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
double xlink_rad;
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

#define MAXFILENAME 127
#define FNAMENBUF   4
static char basename[MAXFILENAME + 1];

static char *cfgfile;
static char *lplfile;
#if defined(LOCALIZED)
static char *locfile;
#endif

#endif
