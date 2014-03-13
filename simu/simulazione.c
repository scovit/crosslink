#if NUM_THREADS > 1
 #include <pthread.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <xmmintrin.h>
#include <zlib.h>
#include "dSFMT-src-2.2.1/dSFMT.h"
#include "simulazione.h"
#include "simprivate.h"
#include "hex.h"
#if defined(TOPO)
#include "raytracing.h"
#endif

// Distances sse optimized - requires SSE2
static inline
__m128 sse_square_dist(const __m128 npx, const __m128 npy, const __m128 npz,
		       const __m128 x, const __m128 y, const __m128 z) {

  __m128 dx = _mm_sub_ps(x, npx);
  __m128 dy = _mm_sub_ps(y, npy);
  __m128 dz = _mm_sub_ps(z, npz);

  dx = _mm_mul_ps(dx, dx);
  dy = _mm_mul_ps(dy, dy);
  dz = _mm_mul_ps(dz, dz);

  __m128 sum = _mm_add_ps (dz, _mm_add_ps(dx, dy));

  // now we have (sqr 4, sqr 3, sqr 2, sqr 1)

  return sum;
}

/* This function uses magic. Only wizards need venture within */
static inline int msk_popcount_32(uint32_t x) {
#if HAVE_POPCNT == 1
  return __builtin_popcount(x);
#else
  x -= ((x >> 1) & 0x55555555);
  x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
  x = (((x >> 4) + x) & 0x0f0f0f0f);
  return (x * 0x01010101) >> 24;
#endif
}
/* This one we use because we need only 4 bit */
static inline uint8_t msk_popcount_4(const uint8_t x) {
#if HAVE_POPCNT == 1
  return __builtin_popcount(x);
#else
  static const uint8_t bit_lookup[16] = { 0, 1, 1, 2, 1, 2, 2, 3,
					  1, 2, 2, 3, 2, 3, 3, 4 };
  return bit_lookup[x & 15];
#endif
}

/* count the number of contacts or reject in case of overlap 
   should check the stickyball bitmap and return 0 if is not interactive
   use 64 bit return value to pass two integers
*/
static bool prob_pot_hc_sse(const float *restrict np, const unsigned int m
#if defined(UNIFORM)
			    , int *restrict sticky_old
			    , int *restrict sticky_new
#endif
#if defined(LOCALIZED)
			    , int *restrict locali_old
			    , int *restrict locali_new
#endif
			    ) {

#if defined(TOPO)
  static const uint8_t topohittable[64] __attribute__ ((aligned (16))) = {
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0,
    2, 0, 0, 0, 0, 2, 0, 0, 1, 2, 0, 0, 0, 1, 2, 0,
    3, 0, 0, 0, 0, 3, 0, 0, 1, 3, 0, 0, 0, 1, 3, 0,
    2, 3, 0, 0, 0, 2, 3, 0, 1, 2, 3, 0, 0, 1, 2, 3,
  };
#endif

#if !defined(HARDCORE) && !defined(UNIFORM) && !defined(TOPO)
  return 0; // Se ghost
#endif

  unsigned int highm = m & (~3);     // and clear them
  uint8_t lowm = m & 3;              // take the lower bits
  
  unsigned int n = N;
  uint8_t bmhighm =  0xF ^ (1 << lowm);

#if defined(UNIFORM)
  *sticky_new = 0;
  *sticky_old = 0;
#endif
#if defined(LOCALIZED)
  *locali_new = 0;
  *locali_old = 0;
  bool topunctu = is_loc_interacting(m);
#endif
#if defined(TOPO)
  topolistlength = 0;
#endif

  __m128 npx = _mm_load1_ps(np);
  __m128 npy = _mm_load1_ps(np + 1);
  __m128 npz = _mm_load1_ps(np + 2);
  __m128 opx = _mm_load1_ps(dots.x + m);
  __m128 opy = _mm_load1_ps(dots.y + m);
  __m128 opz = _mm_load1_ps(dots.z + m);

  for (int i = 0; i < n; i += 4) {
    uint8_t bitmask = (i != highm ? 0xF : bmhighm);

    __m128 x = _mm_load_ps(dots.x + i);
    __m128 y = _mm_load_ps(dots.y + i);
    __m128 z = _mm_load_ps(dots.z + i);

    __m128 distances_new = sse_square_dist(npx, npy, npz, x, y, z);

#if defined(HARDCORE)
    uint8_t hardcore = _mm_movemask_ps(_mm_cmplt_ps(distances_new,comp_hc));

    if (hardcore & bitmask)
      return false;
#endif

#if defined(UNIFORM) || defined(LOCALIZED)
    __m128 distances_old = sse_square_dist(opx, opy, opz, x, y, z);
    uint8_t stickyball;
#endif
#if defined(UNIFORM)
    stickyball = _mm_movemask_ps(_mm_cmplt_ps(distances_new,comp_sb));
    *sticky_new += msk_popcount_4(stickyball & bitmask);
    stickyball = _mm_movemask_ps(_mm_cmplt_ps(distances_old,comp_sb));
    *sticky_old += msk_popcount_4(stickyball & bitmask);
#endif
#if defined(LOCALIZED)
    if (topunctu) {
      stickyball = _mm_movemask_ps(_mm_cmplt_ps(distances_old,comp_sp));
      *locali_old += msk_popcount_4(stickyball & bitmask & locmask[i / 4]);
      stickyball = _mm_movemask_ps(_mm_cmplt_ps(distances_new,comp_sp));
      *locali_new += msk_popcount_4(stickyball & bitmask & locmask[i / 4]);
    }
#endif

#if defined(TOPO)
    uint8_t topohit = _mm_movemask_ps(_mm_cmple_ps(distances_new,comp_top)) &
      bitmask;
    if (unlikely(topohit)) {
      topolist[topolistlength] = i + topohittable[4 * topohit];
      topolist[topolistlength + 1] = i + topohittable[4 * topohit + 1];
      topolist[topolistlength + 2] = i + topohittable[4 * topohit + 2];
      topolist[topolistlength + 3] = i + topohittable[4 * topohit + 3];
      topolistlength += msk_popcount_4(topohit);
    }
#endif

  }

  return true;
}

static inline float squareDist(const float a0, const float a1, const float a2,
			       const float b0, const float b1, const float b2) {

  return ((a0 - b0) * (a0 - b0) +
	  (a1 - b1) * (a1 - b1) +
	  (a2 - b2) * (a2 - b2));
}


// Volume escluso
static int prob_pot_hc(float *restrict np, unsigned int m) {

#if !defined(HARDCORE) && !defined(UNIFORM) && !defined(TOPO)
  return 0; // Se ghost
#endif

  unsigned int sticky = 0;

#if defined(TOPO)
  topolistlength = 0;
#endif

  for ( int i = 0; i < N; i ++)
    if (i != m) {
      float distance = squareDist(np[0], np[1], np[2], 
				  dots.x[i], dots.y[i], dots.z[i]);
#if defined(HARDCORE)
      if (distance < (sigma*sigma)) {
	return -1;
      }
#endif
#if defined(UNIFORM)
      if (distance < (sigma + alfa_uniform)*(sigma + alfa_uniform))
	sticky++;
#endif
#if defined(TOPO)
      if (distance < (4.0f*lambda*4.0f*lambda)) {
	topolist[topolistlength] = i;
	topolistlength++;
      }
#endif
    }

  return sticky;
}

#if defined(TOPO)

static inline void intersect_triangle_prepare() {
  fillstate_triaccel = 0;
}

static inline int intersect_triangle_fill(const float o0, const float o1,
					  const float o2, const float d0,
					  const float d1, const float d2) {
  o_sse.t[fillstate_triaccel] = o0;
  o_sse.t[fillstate_triaccel + 4] = o1;
  o_sse.t[fillstate_triaccel + 8] = o2;
  d_sse.t[fillstate_triaccel] = d0;
  d_sse.t[fillstate_triaccel + 4] = d1;
  d_sse.t[fillstate_triaccel + 8] = d2;
  fillstate_triaccel++;

  return fillstate_triaccel;
}

static inline bool intersect_triangle_query() {
  return fillstate_triaccel;
}

static inline bool intersect_triangle_doit_slow() {
  int fill = fillstate_triaccel;
  fillstate_triaccel = 0;
  for (int i = 0; i < fill; i++) {
    if (intersect_triangle(o_sse.t[i], o_sse.t[i + 4], o_sse.t[i + 8],
			   d_sse.t[i], d_sse.t[i + 4], d_sse.t[i + 8],
			   (float)raytr_sp0[0],
			   (float)raytr_sp1[0],
			   (float)raytr_sp2[0],
			   (float)raytr_sea0[0],
			   (float)raytr_sea1[0],
			   (float)raytr_sea2[0],
			   (float)raytr_seb0[0],
			   (float)raytr_seb1[0],
			   (float)raytr_seb2[0]))
      return true;
  }
  return false;
}

static inline bool intersect_triangle_doit_sse() {
  uint8_t msk = fillmask[fillstate_triaccel];
  fillstate_triaccel = 0;
  return intersect_triangle_sse(msk);
}


static bool prob_pot_topological(const float *restrict np,
				 const unsigned int m) {
  int ncncts = lpl_index[m + 1] - lpl_index[m];
  int *cncts = lpl + lpl_index[m];

  raytr_sp0 = _mm_set1_ps(np[0]);
  raytr_sp1 = _mm_set1_ps(np[1]);
  raytr_sp2 = _mm_set1_ps(np[2]);
  raytr_sea0 = _mm_set1_ps(dots.x[m] - np[0]);
  raytr_sea1 = _mm_set1_ps(dots.y[m] - np[1]);
  raytr_sea2 = _mm_set1_ps(dots.z[m] - np[2]);
  for (int k = 0; k < ncncts; k++) { // foreach triangle
    intersect_triangle_prepare();
    raytr_seb0 = _mm_set1_ps(dots.x[cncts[k]] - np[0]);
    raytr_seb1 = _mm_set1_ps(dots.y[cncts[k]] - np[1]);
    raytr_seb2 = _mm_set1_ps(dots.z[cncts[k]] - np[2]);

    // this can be rewritten so that the links are precalculated
    // and here only a mask is applyed
    // ITERATE OVER LINKS
    for (int h = 0; h < topolistlength; h++) {
      int i = topolist[h];
      if (i == cncts[k]) { //beads like this are connected to
	continue;          //the triangle
      }

      for (int q = lpl_index[i]; q < lpl_index[i+1]; q++) {
	int c = lpl[q];
	if(c > i) {          // avoid to compare link two times
	  break;
	}
	// TILL HERE

	if((c == m) || (c == cncts[k]))  // same as before
	  continue;


	if (intersect_triangle_fill(dots.x[i], dots.y[i], dots.z[i],
				    dots.x[c] - dots.x[i],
				    dots.y[c] - dots.y[i],
				    dots.z[c] - dots.z[i]) == 4)
	  if (intersect_triangle_doit_sse())
	  return false;
      }
    }

    if (intersect_triangle_query())
      if (intersect_triangle_doit_sse())
	return false;
  }
  return true;
}

// Here stop the topological interaction
#endif

static bool prob_pot_linked(float *restrict np, unsigned int m) {

  for (int q = lpl_index[m]; q < lpl_index[m+1]; q++)
    if (squareDist(np[0], np[1], np[2],
		   dots.x[lpl[q]], dots.y[lpl[q]], dots.z[lpl[q]]) >
	(lambda*lambda) )
      return false;

  return true;
}

static inline void displace_sphere(float *restrict np,
				   const float d0,
				   const float d1,
				   const float d2) {
  double x0, x1, x2;
  double summa;
  do {
    // x0 x1 x2 should be taken in (-1, 1);
    x0 = dsfmt_genrand_open_open(&dsfmt);
    x1 = dsfmt_genrand_open_open(&dsfmt);
    x2 = dsfmt_genrand_open_open(&dsfmt);
    x0 += x0 - 1;
    x1 += x1 - 1;
    x2 += x2 - 1;
    summa = x0 * x0 + x1 * x1 + x2 * x2;
  } while (summa >= 1 || summa < 0.1);

  np[0] = d0 + D * (float)x0;
  np[1] = d1 + D * (float)x1;
  np[2] = d2 + D * (float)x2;
}

static int move_ele() {
  float n_pos[3];

  // Prendo un punto a caso per vedere se lo posso muovere
  unsigned int buf_p = dsfmt_genrand_uint32(&dsfmt) % N;

  displace_sphere(n_pos, dots.x[buf_p], dots.y[buf_p], dots.z[buf_p]);

  if ( prob_pot_linked(n_pos, buf_p) == false ) {
    return 0;
  }

#if defined(CONFINEMENT)
  if (n_pos[0]*n_pos[0] + n_pos[1]*n_pos[1] + n_pos[2]*n_pos[2] >
      conf_sqradius)
    return 0;
#endif

#if defined(UNIFORM)
  int oldc_uni;
  int newc_uni;
#endif
#if defined(LOCALIZED)
  int oldc_loc;
  int newc_loc;
#endif

  bool test = prob_pot_hc_sse(n_pos, buf_p
#if defined(UNIFORM)
			      , &oldc_uni
			      , &newc_uni
#endif
#if defined(LOCALIZED)
			      , &oldc_loc
			      , &newc_loc
#endif
			      );

  // Does it overlap with something?
#if defined(HARDCORE)
  if ( test == false )
   return 0;
#endif

  /* if ( dsfmt_genrand_uint32(&dsfmt) % 100 != 0 ) */
  /*   return 0; */

  /* if (buf_p  > 100 || buf_p < 50 ) { */
  /*   float deltay = n_pos[1] - dots.y[buf_p]; */
  /*   if (deltay > D/1.2) */
  /*     return 0; */
  /* } else */
  /*   return 0; */

  // Metropolis test
  // three different codepath for optimisation
#if defined(UNIFORM) && !defined(LOCALIZED)
  if (newc_uni < oldc_uni) {
    double a = __builtin_exp(beta_uniform * (newc_uni - oldc_uni));
    double r = dsfmt_genrand_open_open(&dsfmt);
    if (r >= a)
      return 0;
  }
#endif
#if defined(LOCALIZED) && !defined(UNIFORM)
  if (newc_loc < oldc_loc) {
    double a = __builtin_exp(beta_localized * (newc_loc - oldc_loc));
    double r = dsfmt_genrand_open_open(&dsfmt);
    if (r >= a)
      return 0;
  }
#endif
#if (defined(UNIFORM) && defined(LOCALIZED))
  double c =
    beta_uniform * (newc_uni - oldc_uni) +
    beta_localized * (newc_loc - oldc_loc);
  if (c < 0.) {
    double a = __builtin_exp(c);
    double r = dsfmt_genrand_open_open(&dsfmt);
    if (r >= a)
      return 0;
  }
#endif


#if defined(TOPO)
  // Topological interaction, maybe faster to move it before
  // the metropolis test  
  if ( prob_pot_topological(n_pos, buf_p) == false ) {
    return 0;
  }
#endif

#if defined(UNIFORM)
  contacts += newc_uni - oldc_uni;
#endif
#if defined(LOCALIZED)
  contacts_loc += newc_loc - oldc_loc;
#endif

#if defined(GETPERF)
  displ += (n_pos[0] - dots.x[buf_p])*(n_pos[0] - dots.x[buf_p])
    + (n_pos[1] - dots.y[buf_p])*(n_pos[1] - dots.y[buf_p])
    + (n_pos[2] - dots.z[buf_p])*(n_pos[2] - dots.z[buf_p]);
#endif

  dots.x[buf_p] = n_pos[0];
  dots.y[buf_p] = n_pos[1];
  dots.z[buf_p] = n_pos[2];

  return 1;
}

static inline double energy() {

#ifdef UNIFORM
  double retval = beta_uniform * contacts;
#else
  double retval = 0.0;
#endif

#ifdef LOCALIZED
  retval += beta_localized * contacts_loc;
#endif

  return retval;

}

static inline void recenter(void) {
  float xm, ym, zm;
  xm = 0; ym = 0; zm = 0;

  for (int i = 0; i < N; i++) {
    xm += dots.x[i];
    ym += dots.y[i];
    zm += dots.z[i];
  }
  xm /= N; ym /= N; zm /= N;

  for (int i = 0; i < N; i++) {
    dots.x[i] -= xm;
    dots.y[i] -= ym;
    dots.z[i] -= zm;
  }
}

static inline void print_buffer_gz(gzFile *out) {
  unsigned long long int time = mc_time.DYN_STEPS - mc_time.t;
  for (unsigned int i = 0; i < N; i++)
    gzprintf(out, "%llu\t%.10f\t%.10f\t%.10f\n", time,
	     dots.x[i], dots.y[i], dots.z[i]);
}

void print_buffer(FILE *out) {
  unsigned long long int time = mc_time.DYN_STEPS - mc_time.t;
  for (unsigned int i = 0; i < N; i++)
    fprintf(out, "%llu\t%.10f\t%.10f\t%.10f\n", time,
	    dots.x[i], dots.y[i], dots.z[i]);
}

static __attribute__ ((noinline))
void print_infos(int pN, char *pcnffile, char *plplfile,
		 double psigma, double plambda, double pD,
		 double palfa_uniform, double palfa_localized,
		 double pbeta_uniform,  double pbeta_localized,
		 double pconf_sqradius, unsigned int seed,
		 unsigned long long int pRELAX_TIME,
		 unsigned long long int pCORRL_TIME,
		 int pSTATISTIC, unsigned long long int pDYN_STEPS) {

  time_t tstamp = time(NULL);
  char *timestr = ctime(&tstamp);
  *strchr(timestr,'\n') = '\0';

  fprintf(simufiles.inffile, siminfostring,
	  timestr,
	  pN, pcnffile, plplfile, psigma, plambda, pD, 
	  palfa_uniform, palfa_localized,
	  pbeta_uniform, pbeta_localized,
	  pconf_sqradius, seed,
	  pRELAX_TIME, pCORRL_TIME, pSTATISTIC, pDYN_STEPS);
  fflush(simufiles.inffile);
}

static __attribute__ ((noinline))
int cmpint(const void *p1, const void *p2) {
  return *(int *)p1 - *(int *)p2;
}

static __attribute__ ((noinline))
void set_param_normalized(int enne, double big_sigma,
			  double energy_uniform, double energy_localized,
			  double volume_variable) {
  N = enne;

  if (N % 16) {
    fputs("N should be a multiple of 16\n", stderr);
    exit(-1);
  }

  lambda = sqrt(5.0 / 3.0 / N);
#ifdef HARDCORE
  sigma = cbrt(3.0 * big_sigma / (4.0 * N * M_PI));
#endif
#ifdef UNIFORM
  alfa_uniform = /* 2 * cbrt(0.01 / N); */ /*2*/ 1.44224957 * sigma;
  beta_uniform = energy_uniform;
#endif
#ifdef LOCALIZED
#warning Localized interaction range setted by default to uniform ones
  alfa_localized =  /* 2 * cbrt(0.01 / N); */ /*2*/ 1.44224957 * sigma;
  beta_localized = energy_localized;
#endif
#ifdef CONFINEMENT
  conf_sqradius = cbrt(volume_variable) * cbrt(volume_variable);
#endif

  D = 0.6 * (lambda);

#ifdef HARDCORE
  comp_hc = _mm_set1_ps(sigma*sigma);
#endif
#ifdef UNIFORM
  comp_sb = _mm_set1_ps(alfa_uniform*alfa_uniform);
#endif
#ifdef LOCALIZED
  comp_sp = _mm_set1_ps(alfa_localized*alfa_localized);
#endif
#ifdef TOPO
  //conservative topological cutoff 
  comp_top = _mm_set1_ps(4.0f*lambda*4.0f*lambda);
#endif
}

static void allocate_memory() {
  size_t dotsalloc = (3 * N * sizeof(float));
#if defined(LOCALIZED)
  size_t locmaskalloc = N / 4 * sizeof(uint8_t);
#else
  size_t locmaskalloc = (size_t)0;
#endif
#if defined(TOPO)
  size_t topolistalloc = N * sizeof(int);
#else
  size_t topolistalloc = (size_t)0;
#endif
  size_t lpl_indexalloc = (N + 4) * sizeof(int);
  size_t lplalloc = ODGRMAX * N * sizeof(int);
  if(posix_memalign((void **)&buffer, 16,
		    dotsalloc + locmaskalloc + topolistalloc +
		    lpl_indexalloc + lplalloc)) {
    fprintf(stderr, "Error allocating memory\n");
    exit(-8);
  }
  dots.x = (float *restrict)buffer;
  dots.y = (float *restrict)buffer + N;
  dots.z = (float *restrict)buffer + N + N;
#if defined(LOCALIZED)
  locmask = (uint8_t *restrict)((uint8_t *)buffer + dotsalloc);
#endif
#if defined(TOPO)
  topolist = (int *restrict)((uint8_t *)buffer + dotsalloc + locmaskalloc);
#endif
  lpl_index = (int *restrict)((uint8_t *)buffer + dotsalloc + locmaskalloc
			      + topolistalloc);
  lpl = (int *restrict)((uint8_t *)buffer + dotsalloc + locmaskalloc
			+ topolistalloc + lpl_indexalloc);
}

static __attribute__ ((noinline))
void set_laplacian_null() {
  // linear laplacian
  if (ODGRMAX < 2) {
    fputs("I need more memory, please modify constant ODGRMAX"
	  " in simprivate.h\n", stderr);
    exit(-4);
  }
  lpl_index[0] = 0;
  lpl[0] = 1;
  int k = 1;
  for (int i = 1; i < N - 1; i++) {
    lpl_index[i] = k;
    lpl[k] = i - 1;
    lpl[k+1] = i + 1;
    k += 2;
  }
  lpl_index[N-1] = k;
  lpl[k] = N-2;
  k += 1;
  lpl_index[N] = k;
  if (k > ODGRMAX * N /* lplalloc */) {
    fputs("I need more memory, please modify constant ODGRMAX"
	  " in simprivate.h\n", stderr);
    exit(-4);
  }
}

static __attribute__ ((noinline))
void load_laplacian(char *lplfilepath) {
  if ((simufiles.lplfile = fopen(lplfilepath, "r")) == NULL) {
    if (strcmp(lplfilepath, "NULL")) {
      fprintf(stderr, "Warning: file %s not found and not NULL\n",
	      lplfilepath);
      lplfilepath = "NULL";
    }
    set_laplacian_null();
  } else {
    // from file
    lpl_index[0] = 0;
    for (int i = 0; i < N; i++) {
      int nlinks;
      if (fscanf(simufiles.lplfile, "%d", &nlinks) != 1) {
	fprintf(stderr, "File format error %s\n", lplfilepath);
	exit(-3);
      }
      if (nlinks > ODGRMAX) {
	fputs("I need more memory, please modify constant ODGRMAX"
	      " in simprivate.h\n", stderr);
	exit(-4);
      }
      for (int links = nlinks; links > 0; links--) {
	if (fscanf(simufiles.lplfile, "%d",
		   lpl + lpl_index[i] + nlinks - links) != 1) {
	  fprintf(stderr, "File format error %s\n", lplfilepath);
	  exit(-3);
	}
	if(lpl[lpl_index[i] + nlinks - links] >= N) {
	  nlinks--;
	}
      }
      lpl_index[i + 1] = lpl_index[i] + nlinks;
      if (lpl_index[i + 1] > /* lplalloc */ ODGRMAX * N) {
	fputs("I need more memory, please modify constant ODGRMAX"
	      " in simprivate.h\n", stderr);
	exit(-4);
      }
      qsort(lpl + lpl_index[i], nlinks,
	    sizeof(int), cmpint);
    }
    fclose(simufiles.lplfile);
  }
}


static inline bool is_valid_initial_position(float x, float y, float z, int m) {

  // Checks for links
  for (int q = lpl_index[m]; q < lpl_index[m+1]; q++)
    if ((!isnan(dots.x[lpl[q]])) && 
	(squareDist(x, y, z,
		    dots.x[lpl[q]], dots.y[lpl[q]], dots.z[lpl[q]]) >
	 (lambda*lambda)))
      return false;

#ifdef HARDCORE
  for (int q = 0; q < N; q++)
    if ((!isnan(q)) && 
	(squareDist(x, y, z,
		    dots.x[q], dots.y[q], dots.z[q]) <
	 (sigma*sigma)))
      return false;
#endif

#ifdef CONFINEMENT
  if (x*x + y*y + z*z > conf_sqradius)
    return false;
#endif
  
  return true;
}

static __attribute__ ((noinline))
int set_bead_initial_position(float o_posx, float o_posy, float o_posz,
			       int m) {
  if (!isnan(dots.x[m])) {
    return 0;
  }

  float n_pos[3];
  for (int tentatives = 1000; ; tentatives--) {
    displace_sphere(n_pos, o_posx, o_posy, o_posz);

    if (is_valid_initial_position(n_pos[0], n_pos[1], n_pos[2], m))
      break;

    if (!tentatives)
      return -1;
  }
  dots.x[m] = n_pos[0];
  dots.y[m] = n_pos[1];
  dots.z[m] = n_pos[2];


  int ncncts = lpl_index[m + 1] - lpl_index[m];
  int *cncts = lpl + lpl_index[m];
  for (int k = 0; k < ncncts; k++)
    if(set_bead_initial_position(n_pos[0], n_pos[1], n_pos[2],
				 cncts[k]))
      return -1;

  return 0;
}

static __attribute__ ((noinline))
void set_configuration_rand() {
  for (int i = 0; i <  N; i++) {
    dots.x[i] = (float)NAN;
    dots.y[i] = (float)NAN;
    dots.z[i] = (float)NAN;
  }

  int tentatives = 1000;
  for (int i = 0; i <  N; i++) {
    if (set_bead_initial_position(0.0F, 0.0F, 0.0F, i)) {
      if (!tentatives) {
	fprintf(stderr,
		"Error: Topology don't allow RAND beads configuration, "
		"please provide a filename\n");
	exit(-4);
      }
      tentatives--;
      i = -1;
    }
  }
}

static __attribute__ ((noinline))
void set_configuration_null() {
#if !defined(HARDCORE)
  float gsigma = lambda - lambda/1000;
#else
  float gsigma = sigma;
#endif
#if !defined(UNIFORM)
  float galfa_uniform = sigma + lambda/1000;
#else
  float galfa_uniform = alfa_uniform;
#endif
  dots.x[0] = - ((gsigma + galfa_uniform
		  + lambda/1000) < lambda ? 
		 gsigma + galfa_uniform + lambda/1000 :
		 gsigma + lambda/1000) * N / 2;
  dots.y[0] = 0.0f;
  dots.z[0] = 0.0f;
  for (int i = 1; i <  N; i++) {
    dots.x[i] = dots.x[i-1] + ((gsigma + galfa_uniform
				+ lambda/1000) < lambda ? 
			       gsigma + galfa_uniform + lambda/1000 :
			       gsigma + lambda/1000);
    dots.y[i] = 0.0f;
    dots.z[i] = 0.0f;
  }
}

static __attribute__ ((noinline))
 unsigned long long int load_configuration(FILE *cnffile, 
					   char *cnffilepath) {
  // Attention: input should be gz compressed!!!
  unsigned long long int filetime = 0ULL;
  if (cnffile != NULL) {
    unsigned long long int oldfiletime = 0ULL;
    z_off_t curpos = ftell(cnffile);
    z_off_t startpos = curpos;
    char linebuffer[512];
    // here it finds the last iteration
    while(fgets(linebuffer, sizeof(linebuffer), cnffile)) {
      if (sscanf(linebuffer, "%llu", &filetime) != 1) {
	fprintf(stderr, "File format error %s\n", cnffilepath);
	exit(-3);
      }
      if (filetime != oldfiletime) {
	startpos = curpos;
	oldfiletime = filetime;
      }
      curpos = ftell(cnffile);
    }
    // it moves there and read the configuration
    fseek(cnffile, startpos, SEEK_SET);
    for (unsigned int i = 0; i < N; i++)
      if(!fgets(linebuffer, sizeof(linebuffer), cnffile) || 
	 (sscanf(linebuffer, "%llu%g%g%g", &filetime, 
		 dots.x + i, dots.y + i, dots.z + i) != 4)) {
	fprintf(stderr, "File format error %s\n", cnffilepath);
	exit(-3);
      }
    fflush(cnffile);
  } else {
    fprintf(stderr, "Error: file %s not found and not NULL "
	    "or RAND, aborting\n",
	    cnffilepath);
    exit(-12);
  }
  return filetime;
}

static __attribute__ ((noinline))
void load_localized_stuff(char *locfilepath) {
#if defined(LOCALIZED)
  for (unsigned int i = 0; i < N; i += 4)
    locmask[i / 4] = 0;
  locnum = 0;

  if ((simufiles.locfile = fopen(locfilepath, "r")) == NULL) {
    // no localized interaction
    if (strcmp(locfilepath, "NULL")) {
      fprintf(stderr, "Warning: file %s not found and not NULL\n",
	      locfilepath);
      locfilepath = "NULL";
    }
  } else {
    // load localized interaction from file
    unsigned int interagentlink;
    while(fscanf(simufiles.locfile, "%d", &interagentlink) == 1)
      if (interagentlink < N)
	if (!is_loc_interacting(interagentlink)) {
	    locmask[interagentlink/4] |= (1 << (interagentlink & 3));
	    locnum++;
	  }

    fclose(simufiles.locfile);
  } 
#endif
}

static __attribute__ ((noinline))
void count_contacts() {
#if defined(UNIFORM)
  contacts = 0;
#endif
#if defined(LOCALIZED)
  contacts_loc = 0;
#endif
  for (int i = 0; i < N; i++) {
    for (int k = 0; k < N; k++) if (k != i) {
	float distance = squareDist(dots.x[i], dots.y[i], dots.z[i],
				    dots.x[k], dots.y[k], dots.z[k]);
	if (distance < (sigma*sigma)) {
	  fprintf(stderr, "Wrong initial conditions --> %d %d\n",
		  i, k);
	  exit(-2);
	}
#if defined(UNIFORM)
	if (distance < (sigma + alfa_uniform)*(sigma + alfa_uniform))
	  if (k > i)
	    contacts++;
#endif
#if defined(LOCALIZED)
        if (distance < (sigma + alfa_localized)*(sigma + alfa_localized))
          if (k > i)
	    if (is_loc_interacting(k) && is_loc_interacting(i))
	      contacts_loc++;
#endif
      }
  }
}

static __attribute__ ((noinline))
unsigned int parse_info(char *filepath) {
  FILE *tmpfile = fopen(filepath, "r");
  unsigned int retval;
  bool got_there = false;
  char linebuffer[512];
  while(fgets(linebuffer, sizeof(linebuffer), tmpfile)) {
    if (!strncmp(linebuffer, "seed=", 5)) {
      got_there = true;
      if (sscanf(linebuffer + 5, "%u", &retval) != 1) {
	fprintf(stderr, "Checkpoint, file format error %s\n", filepath);
	exit(-3);
      }
    }
  }
  fclose(tmpfile);

  if (!got_there) {
    fprintf(stderr, "Checkpoint, no seed there in %s\n",
	    filepath);
    exit(-3);
  }

  return retval;
}

static __attribute__ ((noinline))
void openfiles(char *outstring, const char *mode,
	       unsigned int *seed) {
  const char *dir = "out/";
  const char *xyzext = ".xyz.dat.gz";
  const char *accext = ".acc.dat";
  const char *ctcext = ".ctc.dat";
  const char *infext = ".info";
  const char *rndext = ".rndstate";
  const char *chkext = ".chk";
  int lenght = strlen(xyzext) + strlen(outstring) + strlen(dir) + 1;
  char filename[lenght];
  bool mode_is_a = !strcmp(mode, "a");

  if (!mode_is_a)
    (void)mkdir(dir, 0775);

  strcpy(filename, dir); strcat(filename, outstring);
  strcat(filename, xyzext);
  simufiles.xyzfile = gzopen(filename, mode);

  strcpy(filename, dir); strcat(filename, outstring);
  strcat(filename, accext);
  simufiles.accfile = fopen(filename, mode);

  strcpy(filename, dir); strcat(filename, outstring);
  strcat(filename, ctcext);
  simufiles.ctcfile = fopen(filename, mode);

  strcpy(filename, dir); strcat(filename, outstring);
  strcat(filename, infext);
  if (mode_is_a) {
    unsigned int temp = parse_info(filename);
    if (seed)
      *seed = temp;
  }
  simufiles.inffile = fopen(filename, "w");

  strcpy(filename, dir); strcat(filename, outstring);
  strcat(filename, rndext);
  if (mode_is_a) {
    int error = get_saved_dsfmt(filename, &dsfmt);
    if (error) {
      fprintf(stderr, "Warning, was unable to open old seed, r=%d\n",
	      error);
    } 
  }
  simufiles.rndfile = fopen(filename, "w");

  strcpy(filename, dir); strcat(filename, outstring);
  strcat(filename, chkext);
  if (mode_is_a)
    simufiles.chkfile = fopen(filename, "r+");
  else
    simufiles.chkfile = fopen(filename, "w");
}

__attribute__ ((noinline))
void closefiles() {
  gzclose(simufiles.xyzfile);
  fclose(simufiles.accfile);
  fclose(simufiles.ctcfile);
  fclose(simufiles.rndfile);
  fclose(simufiles.chkfile);

  time_t tstamp = time(NULL);
  char *timestr = ctime(&tstamp);
  *strchr(timestr,'\n') = '\0';
  fprintf(simufiles.inffile, "ENDTIME=\"%s\"\n", timestr);
  fclose(simufiles.inffile);
}

__attribute__ ((noinline))
void flushfiles() {
  fflush(stderr);
  fflush(stdout);
  gzflush(simufiles.xyzfile, Z_SYNC_FLUSH);
  fflush(simufiles.accfile);
  fflush(simufiles.ctcfile);
  fflush(simufiles.rndfile);
  fflush(simufiles.chkfile);
}

void *simulazione(void *threadarg) {

  // This sets all denormal numbers to zero - DAZ and FTZ flags
  _mm_setcsr( _mm_getcsr() | 0x8040 );

  //
  // Initialization code, set global variables and files
  //
  char *outstring = ((struct thread_data *) threadarg) -> argv[3];
  char *cnffilepath = ((struct thread_data *) threadarg) -> argv[4];
  char *lplfilepath = ((struct thread_data *) threadarg) -> argv[5];
  char *locfilepath = ((struct thread_data *) threadarg) -> argv[6];
  set_param_normalized(atoi(((struct thread_data *)threadarg) -> argv[2]),
		       atof(((struct thread_data *)threadarg) -> argv[7]),
		       atof(((struct thread_data *)threadarg) -> argv[8]),
		       atof(((struct thread_data *)threadarg) -> argv[9]),
		       atof(((struct thread_data *)threadarg) -> argv[10]));

  // initialize seed
  struct timeval t1;
  gettimeofday(&t1, NULL);
  unsigned int seed;
  seed = t1.tv_usec * t1.tv_sec;
  dsfmt_init_gen_rand(&dsfmt, seed);

  bool isresume = 
    !strcmp(((struct thread_data *)threadarg) -> argv[1], "resume");

  allocate_memory();

  // put laplacian online from file or automatically
  load_laplacian(lplfilepath);

  unsigned long long int resume_elapsed;
  if (!isresume) {
    if (!strcmp(cnffilepath, "RAND")) {
      set_configuration_rand();
    } else if (!strcmp(cnffilepath, "NULL")) {
      set_configuration_null();
    } else  {
      FILE *cnffile = fopen(cnffilepath, "r");
      load_configuration(cnffile, cnffilepath);
    }
    resume_elapsed = 0;
    openfiles(outstring, "w", NULL);
  } else {
    openfiles(outstring, "a", &seed);
    resume_elapsed = load_configuration(simufiles.chkfile, "Checkpoint");
  }

  write_dsfmt_file(&dsfmt, simufiles.rndfile);

  // put locally interacting beads from or automatically
  load_localized_stuff(locfilepath);

  // count the contacts in order to put variables online
  count_contacts();

  //
  // Simulation code, here starts the simulation
  //

  /* Prova */
  /**/
  const unsigned long long CORRL_TIME = 2*458ULL * N * N;
  const unsigned long long RELAX_TIME = 100ULL * CORRL_TIME; //200
  const int STATISTIC = 1000;
  /**/

  /* Simulations (for the Cacciuto stuff) */
  /*
  const unsigned long long CORRL_TIME = 
    2*458ULL * N * N;
  const unsigned long long RELAX_TIME = 200ULL * CORRL_TIME;
  const int STATISTIC = 32*4800;
  */

  /* Autocorrelations */
  /*
  const unsigned long long RELAX_TIME = 0;
  const unsigned long long CORRL_TIME = 200*N;
  const int STATISTIC = 40000;
  */

  /* Performances */
  /*
  const unsigned long long RELAX_TIME = 
    0;
  const unsigned long long CORRL_TIME =
    300000*N;
  const int STATISTIC = 1000;
  */

  mc_time.DYN_STEPS = RELAX_TIME + CORRL_TIME * STATISTIC + 1;

#if !defined(HARDCORE)
  float sigma = NAN;
#endif
#if !defined(UNIFORM)
  float alfa_uniform = NAN;
  double beta_uniform = NAN;
#endif
#if !defined(LOCALIZED)
  float alfa_localized = NAN;
  double beta_localized = NAN;
#endif
#if !defined(CONFINEMENT)
  float conf_sqradius = NAN;
#endif

  print_infos(N, cnffilepath, lplfilepath, sigma, lambda, D, 
	      alfa_uniform, alfa_localized,
	      beta_uniform, beta_localized, conf_sqradius,
	      seed, RELAX_TIME, CORRL_TIME, STATISTIC, mc_time.DYN_STEPS);

#if NUM_THREADS > 1
  pthread_barrier_wait(&startbarr);
  pthread_barrier_wait(&firstbarr);
#endif

#if defined(GETPERF)
  displ = 0.0;
  struct timeval t2;
  struct timeval *then = &t1;
  struct timeval *now = &t2;
  gettimeofday(then, NULL);
#endif

  unsigned int accepted = 0;
  unsigned int total = 0;

  mc_time.t = mc_time.DYN_STEPS - resume_elapsed;

  unsigned long long int toprint = mc_time.t -
    (RELAX_TIME + CORRL_TIME);

  for ( ; mc_time.t != 0; mc_time.t-- ) {
#if NUM_THREADS > 1
    pthread_spin_lock (&spinsum);
#endif
    // TODO: - Extimate the correlation time and use it for measurements
    //       - load with datafiles from fortran simulation

    if ( unlikely(mc_time.t == toprint) ) {
      toprint -= CORRL_TIME;

      rewind(simufiles.rndfile);
      write_dsfmt_file(&dsfmt, simufiles.rndfile);

#if !defined(CONFINEMENT)
      recenter();
#endif

#if defined(GETXYZ)
      print_buffer_gz(simufiles.xyzfile);
#endif

#if defined(GETPERF)
      gettimeofday(now, NULL);
      unsigned long long ellapsed_time = (now->tv_sec - then->tv_sec) *
	1000000ULL + (now->tv_usec - then->tv_usec);
      fprintf(simufiles.accfile, "%d\t%d\t%g\t%llu\t%g\t%g\n", accepted, total, 
	      (float)accepted / total, ellapsed_time,
	      displ, displ / ellapsed_time);

      displ = 0.0;
      struct timeval *temp = now;
      now = then; then = temp;
#else
      fprintf(simufiles.accfile, "%g\n", (float) accepted / total);
#endif

      total = 0;
      accepted = 0;

#if defined(GETENERGY)
      fprintf(simufiles.ctcfile, "%llu\t%g\t%d\t%d\n",
	      mc_time.DYN_STEPS - mc_time.t,
	      energy(),
#ifdef UNIFORM
	      contacts,
#else
	      0,
#endif
#ifdef LOCALIZED
	      contacts_loc);
#else
              0);
#endif

#endif
    }

    if (move_ele())
      accepted++;
    total++;

#if NUM_THREADS > 1
    pthread_spin_unlock (&spinsum);
#endif

  }

  closefiles();

  return NULL;
}
