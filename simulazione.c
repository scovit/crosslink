#if NUM_THREADS > 1
 #include <pthread.h>
 #ifdef __APPLE__
 #include "mac_pthread.h"
 #endif
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
#include <immintrin.h>
#include <zlib.h>
#include "dSFMT-src-2.2.1/dSFMT.h"
#include "infofile/infofile.h"
#include "simulazione.h"
#include "simprivate.h"
#include "hex.h"
#include "checkpoint.h"
#if defined(TOPO)
#include "raytracing.h"
#endif

// Distances sse optimized - requires AVX
static inline
__m256 avx_square_dist(const __m256 npx, const __m256 npy, const __m256 npz,
		       const __m256 x, const __m256 y, const __m256 z) {

  __m256 dx = _mm256_sub_ps(x, npx);
  __m256 dy = _mm256_sub_ps(y, npy);
  __m256 dz = _mm256_sub_ps(z, npz);

  dx = _mm256_mul_ps(dx, dx);
  dy = _mm256_mul_ps(dy, dy);
  dz = _mm256_mul_ps(dz, dz);

  __m256 sum = _mm256_add_ps (dz, _mm256_add_ps(dx, dy));

  // now we have (sqr 4, sqr 3, sqr 2, sqr 1)

  return sum;
}


static inline uint8_t msk_popcount_8(const uint8_t x) {
  return __builtin_popcount(x);
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

#if (defined(TOPO) || defined(XLINK))
  static const uint8_t hittable[2048] __attribute__ ((aligned (32))) = {
    // seq 0 255 | perl6 -ne 'my @g = .Int.base(2).comb.reverse.grep: 1, :k;
    //                        @g = @g.append(0 xx 8);
    //                        say @g[0..7].join(", "), ","'
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    2, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 0, 0, 0, 0, 0,
    1, 2, 0, 0, 0, 0, 0, 0,
    0, 1, 2, 0, 0, 0, 0, 0,
    3, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 0, 0,
    1, 3, 0, 0, 0, 0, 0, 0,
    0, 1, 3, 0, 0, 0, 0, 0,
    2, 3, 0, 0, 0, 0, 0, 0,
    0, 2, 3, 0, 0, 0, 0, 0,
    1, 2, 3, 0, 0, 0, 0, 0,
    0, 1, 2, 3, 0, 0, 0, 0,
    4, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 0, 0, 0, 0, 0, 0,
    1, 4, 0, 0, 0, 0, 0, 0,
    0, 1, 4, 0, 0, 0, 0, 0,
    2, 4, 0, 0, 0, 0, 0, 0,
    0, 2, 4, 0, 0, 0, 0, 0,
    1, 2, 4, 0, 0, 0, 0, 0,
    0, 1, 2, 4, 0, 0, 0, 0,
    3, 4, 0, 0, 0, 0, 0, 0,
    0, 3, 4, 0, 0, 0, 0, 0,
    1, 3, 4, 0, 0, 0, 0, 0,
    0, 1, 3, 4, 0, 0, 0, 0,
    2, 3, 4, 0, 0, 0, 0, 0,
    0, 2, 3, 4, 0, 0, 0, 0,
    1, 2, 3, 4, 0, 0, 0, 0,
    0, 1, 2, 3, 4, 0, 0, 0,
    5, 0, 0, 0, 0, 0, 0, 0,
    0, 5, 0, 0, 0, 0, 0, 0,
    1, 5, 0, 0, 0, 0, 0, 0,
    0, 1, 5, 0, 0, 0, 0, 0,
    2, 5, 0, 0, 0, 0, 0, 0,
    0, 2, 5, 0, 0, 0, 0, 0,
    1, 2, 5, 0, 0, 0, 0, 0,
    0, 1, 2, 5, 0, 0, 0, 0,
    3, 5, 0, 0, 0, 0, 0, 0,
    0, 3, 5, 0, 0, 0, 0, 0,
    1, 3, 5, 0, 0, 0, 0, 0,
    0, 1, 3, 5, 0, 0, 0, 0,
    2, 3, 5, 0, 0, 0, 0, 0,
    0, 2, 3, 5, 0, 0, 0, 0,
    1, 2, 3, 5, 0, 0, 0, 0,
    0, 1, 2, 3, 5, 0, 0, 0,
    4, 5, 0, 0, 0, 0, 0, 0,
    0, 4, 5, 0, 0, 0, 0, 0,
    1, 4, 5, 0, 0, 0, 0, 0,
    0, 1, 4, 5, 0, 0, 0, 0,
    2, 4, 5, 0, 0, 0, 0, 0,
    0, 2, 4, 5, 0, 0, 0, 0,
    1, 2, 4, 5, 0, 0, 0, 0,
    0, 1, 2, 4, 5, 0, 0, 0,
    3, 4, 5, 0, 0, 0, 0, 0,
    0, 3, 4, 5, 0, 0, 0, 0,
    1, 3, 4, 5, 0, 0, 0, 0,
    0, 1, 3, 4, 5, 0, 0, 0,
    2, 3, 4, 5, 0, 0, 0, 0,
    0, 2, 3, 4, 5, 0, 0, 0,
    1, 2, 3, 4, 5, 0, 0, 0,
    0, 1, 2, 3, 4, 5, 0, 0,
    6, 0, 0, 0, 0, 0, 0, 0,
    0, 6, 0, 0, 0, 0, 0, 0,
    1, 6, 0, 0, 0, 0, 0, 0,
    0, 1, 6, 0, 0, 0, 0, 0,
    2, 6, 0, 0, 0, 0, 0, 0,
    0, 2, 6, 0, 0, 0, 0, 0,
    1, 2, 6, 0, 0, 0, 0, 0,
    0, 1, 2, 6, 0, 0, 0, 0,
    3, 6, 0, 0, 0, 0, 0, 0,
    0, 3, 6, 0, 0, 0, 0, 0,
    1, 3, 6, 0, 0, 0, 0, 0,
    0, 1, 3, 6, 0, 0, 0, 0,
    2, 3, 6, 0, 0, 0, 0, 0,
    0, 2, 3, 6, 0, 0, 0, 0,
    1, 2, 3, 6, 0, 0, 0, 0,
    0, 1, 2, 3, 6, 0, 0, 0,
    4, 6, 0, 0, 0, 0, 0, 0,
    0, 4, 6, 0, 0, 0, 0, 0,
    1, 4, 6, 0, 0, 0, 0, 0,
    0, 1, 4, 6, 0, 0, 0, 0,
    2, 4, 6, 0, 0, 0, 0, 0,
    0, 2, 4, 6, 0, 0, 0, 0,
    1, 2, 4, 6, 0, 0, 0, 0,
    0, 1, 2, 4, 6, 0, 0, 0,
    3, 4, 6, 0, 0, 0, 0, 0,
    0, 3, 4, 6, 0, 0, 0, 0,
    1, 3, 4, 6, 0, 0, 0, 0,
    0, 1, 3, 4, 6, 0, 0, 0,
    2, 3, 4, 6, 0, 0, 0, 0,
    0, 2, 3, 4, 6, 0, 0, 0,
    1, 2, 3, 4, 6, 0, 0, 0,
    0, 1, 2, 3, 4, 6, 0, 0,
    5, 6, 0, 0, 0, 0, 0, 0,
    0, 5, 6, 0, 0, 0, 0, 0,
    1, 5, 6, 0, 0, 0, 0, 0,
    0, 1, 5, 6, 0, 0, 0, 0,
    2, 5, 6, 0, 0, 0, 0, 0,
    0, 2, 5, 6, 0, 0, 0, 0,
    1, 2, 5, 6, 0, 0, 0, 0,
    0, 1, 2, 5, 6, 0, 0, 0,
    3, 5, 6, 0, 0, 0, 0, 0,
    0, 3, 5, 6, 0, 0, 0, 0,
    1, 3, 5, 6, 0, 0, 0, 0,
    0, 1, 3, 5, 6, 0, 0, 0,
    2, 3, 5, 6, 0, 0, 0, 0,
    0, 2, 3, 5, 6, 0, 0, 0,
    1, 2, 3, 5, 6, 0, 0, 0,
    0, 1, 2, 3, 5, 6, 0, 0,
    4, 5, 6, 0, 0, 0, 0, 0,
    0, 4, 5, 6, 0, 0, 0, 0,
    1, 4, 5, 6, 0, 0, 0, 0,
    0, 1, 4, 5, 6, 0, 0, 0,
    2, 4, 5, 6, 0, 0, 0, 0,
    0, 2, 4, 5, 6, 0, 0, 0,
    1, 2, 4, 5, 6, 0, 0, 0,
    0, 1, 2, 4, 5, 6, 0, 0,
    3, 4, 5, 6, 0, 0, 0, 0,
    0, 3, 4, 5, 6, 0, 0, 0,
    1, 3, 4, 5, 6, 0, 0, 0,
    0, 1, 3, 4, 5, 6, 0, 0,
    2, 3, 4, 5, 6, 0, 0, 0,
    0, 2, 3, 4, 5, 6, 0, 0,
    1, 2, 3, 4, 5, 6, 0, 0,
    0, 1, 2, 3, 4, 5, 6, 0,
    7, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 0, 0, 0, 0, 0, 0,
    1, 7, 0, 0, 0, 0, 0, 0,
    0, 1, 7, 0, 0, 0, 0, 0,
    2, 7, 0, 0, 0, 0, 0, 0,
    0, 2, 7, 0, 0, 0, 0, 0,
    1, 2, 7, 0, 0, 0, 0, 0,
    0, 1, 2, 7, 0, 0, 0, 0,
    3, 7, 0, 0, 0, 0, 0, 0,
    0, 3, 7, 0, 0, 0, 0, 0,
    1, 3, 7, 0, 0, 0, 0, 0,
    0, 1, 3, 7, 0, 0, 0, 0,
    2, 3, 7, 0, 0, 0, 0, 0,
    0, 2, 3, 7, 0, 0, 0, 0,
    1, 2, 3, 7, 0, 0, 0, 0,
    0, 1, 2, 3, 7, 0, 0, 0,
    4, 7, 0, 0, 0, 0, 0, 0,
    0, 4, 7, 0, 0, 0, 0, 0,
    1, 4, 7, 0, 0, 0, 0, 0,
    0, 1, 4, 7, 0, 0, 0, 0,
    2, 4, 7, 0, 0, 0, 0, 0,
    0, 2, 4, 7, 0, 0, 0, 0,
    1, 2, 4, 7, 0, 0, 0, 0,
    0, 1, 2, 4, 7, 0, 0, 0,
    3, 4, 7, 0, 0, 0, 0, 0,
    0, 3, 4, 7, 0, 0, 0, 0,
    1, 3, 4, 7, 0, 0, 0, 0,
    0, 1, 3, 4, 7, 0, 0, 0,
    2, 3, 4, 7, 0, 0, 0, 0,
    0, 2, 3, 4, 7, 0, 0, 0,
    1, 2, 3, 4, 7, 0, 0, 0,
    0, 1, 2, 3, 4, 7, 0, 0,
    5, 7, 0, 0, 0, 0, 0, 0,
    0, 5, 7, 0, 0, 0, 0, 0,
    1, 5, 7, 0, 0, 0, 0, 0,
    0, 1, 5, 7, 0, 0, 0, 0,
    2, 5, 7, 0, 0, 0, 0, 0,
    0, 2, 5, 7, 0, 0, 0, 0,
    1, 2, 5, 7, 0, 0, 0, 0,
    0, 1, 2, 5, 7, 0, 0, 0,
    3, 5, 7, 0, 0, 0, 0, 0,
    0, 3, 5, 7, 0, 0, 0, 0,
    1, 3, 5, 7, 0, 0, 0, 0,
    0, 1, 3, 5, 7, 0, 0, 0,
    2, 3, 5, 7, 0, 0, 0, 0,
    0, 2, 3, 5, 7, 0, 0, 0,
    1, 2, 3, 5, 7, 0, 0, 0,
    0, 1, 2, 3, 5, 7, 0, 0,
    4, 5, 7, 0, 0, 0, 0, 0,
    0, 4, 5, 7, 0, 0, 0, 0,
    1, 4, 5, 7, 0, 0, 0, 0,
    0, 1, 4, 5, 7, 0, 0, 0,
    2, 4, 5, 7, 0, 0, 0, 0,
    0, 2, 4, 5, 7, 0, 0, 0,
    1, 2, 4, 5, 7, 0, 0, 0,
    0, 1, 2, 4, 5, 7, 0, 0,
    3, 4, 5, 7, 0, 0, 0, 0,
    0, 3, 4, 5, 7, 0, 0, 0,
    1, 3, 4, 5, 7, 0, 0, 0,
    0, 1, 3, 4, 5, 7, 0, 0,
    2, 3, 4, 5, 7, 0, 0, 0,
    0, 2, 3, 4, 5, 7, 0, 0,
    1, 2, 3, 4, 5, 7, 0, 0,
    0, 1, 2, 3, 4, 5, 7, 0,
    6, 7, 0, 0, 0, 0, 0, 0,
    0, 6, 7, 0, 0, 0, 0, 0,
    1, 6, 7, 0, 0, 0, 0, 0,
    0, 1, 6, 7, 0, 0, 0, 0,
    2, 6, 7, 0, 0, 0, 0, 0,
    0, 2, 6, 7, 0, 0, 0, 0,
    1, 2, 6, 7, 0, 0, 0, 0,
    0, 1, 2, 6, 7, 0, 0, 0,
    3, 6, 7, 0, 0, 0, 0, 0,
    0, 3, 6, 7, 0, 0, 0, 0,
    1, 3, 6, 7, 0, 0, 0, 0,
    0, 1, 3, 6, 7, 0, 0, 0,
    2, 3, 6, 7, 0, 0, 0, 0,
    0, 2, 3, 6, 7, 0, 0, 0,
    1, 2, 3, 6, 7, 0, 0, 0,
    0, 1, 2, 3, 6, 7, 0, 0,
    4, 6, 7, 0, 0, 0, 0, 0,
    0, 4, 6, 7, 0, 0, 0, 0,
    1, 4, 6, 7, 0, 0, 0, 0,
    0, 1, 4, 6, 7, 0, 0, 0,
    2, 4, 6, 7, 0, 0, 0, 0,
    0, 2, 4, 6, 7, 0, 0, 0,
    1, 2, 4, 6, 7, 0, 0, 0,
    0, 1, 2, 4, 6, 7, 0, 0,
    3, 4, 6, 7, 0, 0, 0, 0,
    0, 3, 4, 6, 7, 0, 0, 0,
    1, 3, 4, 6, 7, 0, 0, 0,
    0, 1, 3, 4, 6, 7, 0, 0,
    2, 3, 4, 6, 7, 0, 0, 0,
    0, 2, 3, 4, 6, 7, 0, 0,
    1, 2, 3, 4, 6, 7, 0, 0,
    0, 1, 2, 3, 4, 6, 7, 0,
    5, 6, 7, 0, 0, 0, 0, 0,
    0, 5, 6, 7, 0, 0, 0, 0,
    1, 5, 6, 7, 0, 0, 0, 0,
    0, 1, 5, 6, 7, 0, 0, 0,
    2, 5, 6, 7, 0, 0, 0, 0,
    0, 2, 5, 6, 7, 0, 0, 0,
    1, 2, 5, 6, 7, 0, 0, 0,
    0, 1, 2, 5, 6, 7, 0, 0,
    3, 5, 6, 7, 0, 0, 0, 0,
    0, 3, 5, 6, 7, 0, 0, 0,
    1, 3, 5, 6, 7, 0, 0, 0,
    0, 1, 3, 5, 6, 7, 0, 0,
    2, 3, 5, 6, 7, 0, 0, 0,
    0, 2, 3, 5, 6, 7, 0, 0,
    1, 2, 3, 5, 6, 7, 0, 0,
    0, 1, 2, 3, 5, 6, 7, 0,
    4, 5, 6, 7, 0, 0, 0, 0,
    0, 4, 5, 6, 7, 0, 0, 0,
    1, 4, 5, 6, 7, 0, 0, 0,
    0, 1, 4, 5, 6, 7, 0, 0,
    2, 4, 5, 6, 7, 0, 0, 0,
    0, 2, 4, 5, 6, 7, 0, 0,
    1, 2, 4, 5, 6, 7, 0, 0,
    0, 1, 2, 4, 5, 6, 7, 0,
    3, 4, 5, 6, 7, 0, 0, 0,
    0, 3, 4, 5, 6, 7, 0, 0,
    1, 3, 4, 5, 6, 7, 0, 0,
    0, 1, 3, 4, 5, 6, 7, 0,
    2, 3, 4, 5, 6, 7, 0, 0,
    0, 2, 3, 4, 5, 6, 7, 0,
    1, 2, 3, 4, 5, 6, 7, 0,
    0, 1, 2, 3, 4, 5, 6, 7
  };
#endif

#if (!defined(HARDCORE) && !defined(UNIFORM) && !defined(TOPO) && \
     !defined(LOCALIZED) && !defined(XLINK))
  return 0; // Se ghost
#endif

  unsigned int highm = m & (~7);     // and clear them
  uint8_t lowm = m & 7;              // take the lower bits
  
  unsigned int n = N;
  uint8_t bmhighm =  0xFF ^ (1 << lowm);

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
#if defined(XLINK)
  xlinklistlength = 0;
#endif

  __m256 npx = _mm256_broadcast_ss(np);
  __m256 npy = _mm256_broadcast_ss(np + 1);
  __m256 npz = _mm256_broadcast_ss(np + 2);
  __m256 opx = _mm256_broadcast_ss(dots.x + m);
  __m256 opy = _mm256_broadcast_ss(dots.y + m);
  __m256 opz = _mm256_broadcast_ss(dots.z + m);

  for (int i = 0; i < n; i += 8) {
    uint8_t bitmask = (i != highm ? 0xFF : bmhighm);

    __m256 x = _mm256_load_ps(dots.x + i);
    __m256 y = _mm256_load_ps(dots.y + i);
    __m256 z = _mm256_load_ps(dots.z + i);

    __m256 distances_new = avx_square_dist(npx, npy, npz, x, y, z);

#if defined(HARDCORE)
    uint8_t hardcore = _mm256_movemask_ps(_mm256_cmp_ps(distances_new, comp_hc, _CMP_LT_OS));

    if (hardcore & bitmask)
      return false;
#endif

#if defined(UNIFORM) || defined(LOCALIZED)
    __m256 distances_old = sse_square_dist(opx, opy, opz, x, y, z);
    uint8_t stickyball;
#endif
#if defined(UNIFORM)
    stickyball = _mm256_movemask_ps(_mm256_cmp_ps(distances_new,comp_sb, _CMP_LT_OS));
    *sticky_new += msk_popcount_8(stickyball & bitmask);
    stickyball = _mm256_movemask_ps(_mm256_cmp_ps(distances_old,comp_sb, _CMP_LT_OS));
    *sticky_old += msk_popcount_8(stickyball & bitmask);
#endif
#if defined(LOCALIZED)
    if (topunctu) {
      stickyball = _mm256_movemask_ps(_mm256_cmp_ps(distances_old,comp_sp, _CMP_LT_OS));
      *locali_old += msk_popcount_8(stickyball & bitmask & locmask[i / 4]);
      stickyball = _mm256_movemask_ps(_mm256_cmp_ps(distances_new,comp_sp, _CMP_LT_OS));
      *locali_new += msk_popcount_8(stickyball & bitmask & locmask[i / 4]);
    }
#endif
#if defined(XLINK)
    uint8_t xlinkhit = _mm256_movemask_ps(_mm256_cmp_ps(distances_new,comp_xl, _CMP_LT_OS)) &
      bitmask;
    if (unlikely(xlinkhit)) {
      // This generates shitty code because we are cvtepi8 to epi32
      // in the AVX current standard (supported by my cluster)
      // this cannot be done by intrinsics, not even cvtepi16 to epi32
      // (which is in AVX2)
      // future versions of AVX (2+) will allow for a rewrite using
      // a couple of intrinsics, altough at that point we will work
      // on groups of 16 links and so the hittable
      // might get too big for a good cacheing and pheraps,
      // will need to be retought
      xlinklist[xlinklistlength] = i + hittable[8 * xlinkhit];
      xlinklist[xlinklistlength + 1] = i + hittable[8 * xlinkhit + 1];
      xlinklist[xlinklistlength + 2] = i + hittable[8 * xlinkhit + 2];
      xlinklist[xlinklistlength + 3] = i + hittable[8 * xlinkhit + 3];
      xlinklist[xlinklistlength + 4] = i + hittable[8 * xlinkhit + 4];
      xlinklist[xlinklistlength + 5] = i + hittable[8 * xlinkhit + 5];
      xlinklist[xlinklistlength + 6] = i + hittable[8 * xlinkhit + 6];
      xlinklist[xlinklistlength + 7] = i + hittable[8 * xlinkhit + 7];
      xlinklistlength += msk_popcount_8(xlinkhit);
    }
#endif

#if defined(TOPO)
    uint8_t topohit = _mm256_movemask_ps(_mm256_cmp_ps(distances_new,comp_top, _CMP_LE_OS)) &
      bitmask;
    if (unlikely(topohit)) {
      topolist[topolistlength] = i + hittable[8 * topohit];
      topolist[topolistlength + 1] = i + hittable[8 * topohit + 1];
      topolist[topolistlength + 2] = i + hittable[8 * topohit + 2];
      topolist[topolistlength + 3] = i + hittable[8 * topohit + 3];
      topolist[topolistlength + 4] = i + hittable[8 * topohit + 4];
      topolist[topolistlength + 5] = i + hittable[8 * topohit + 5];
      topolist[topolistlength + 6] = i + hittable[8 * topohit + 6];
      topolist[topolistlength + 7] = i + hittable[8 * topohit + 7];
      topolistlength += msk_popcount_8(topohit);
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

#if !defined(HARDCORE) && !defined(UNIFORM) && !defined(TOPO) && !defined(XLINK)
  return 0; // Se ghost
#endif

  unsigned int sticky = 0;

#if defined(TOPO)
  topolistlength = 0;
#endif
#if defined(XLINK)
  xlinklistlength = 0;
#endif

  for ( int i = 0; i < N; i ++)
    if (i != m) {
      float distance = squareDist(np[0], np[1], np[2], 
				  dots.x[i], dots.y[i], dots.z[i]);
#if defined(HARDCORE)
      if (distance < comp_hc[0] * comp_hc[0]) {
	return -1;
      }
#endif
#if defined(UNIFORM)
      if (distance < comp_sb[0] * comp_sb[0])
	sticky++;
#endif
#if defined(XLINK)
      if (distance < comp_xl[0] * comp_xl[0]) {
	xlinklist[xlinklistlength] = i;
	xlinklistlength++;
      }
#endif
#if defined(TOPO)
      if (distance < comp_top[0] * comp_top[0]) {
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

static void push_in_laplacian(int pos, int value) {
  for (int q = lpl_index[N] - 1; q >= lpl_index[pos + 1]; q--)
    lpl[q + 1] = lpl[q];
  lpl[lpl_index[pos + 1]] = value;
  for (int q = pos + 1; q <= N; q++)
    lpl_index[q]++;
#if ((NUM_THREADS > 1) && defined(XLINK))
  for (int q = crx_index[N] - 1; q >= crx_index[pos + 1]; q--)
    crx[q + 1] = crx[q];
  crx[crx_index[pos + 1]] = value;
  for (int q = pos + 1; q <= N; q++)
    crx_index[q]++;
#endif
}

static inline bool connected_laplacian(int i, int j) {

  for (int q = lpl_index[i]; q < lpl_index[i+1]; q++)
    if (lpl[q] == j)
      return true; // Already connected

  return false;
}


#if defined(FASTEXP)
#define EXPLOOKUPSIZE 256
#define EXPLOOKUPFIRST 64

#if defined(UNIFORM)
__attribute__ ((aligned (32))) double uniform_exp_lookup[EXPLOOKUPSIZE];
#endif
#if defined(LOCALIZED)
__attribute__ ((aligned (32))) double localized_exp_lookup[EXPLOOKUPSIZE];
#endif

__attribute__ ((noinline))
void populate_lookup_tables() {
  for (int i = 0; i < EXPLOOKUPSIZE; i++) {
#if defined(UNIFORM)
    uniform_exp_lookup[i] =
      __builtin_exp(beta_uniform * (EXPLOOKUPFIRST - i));
#endif
#if defined(LOCALIZED)
    localized_exp_lookup[i] =
      __builtin_exp(beta_localized * (EXPLOOKUPFIRST - i));
#endif
  }
}

#if defined(UNIFORM)
static inline double fastexp_uniform(int arg) {
  arg = EXPLOOKUPFIRST - arg;
  if (arg < EXPLOOKUPSIZE && (arg > 0))
    return uniform_exp_lookup[arg];
  else
    return __builtin_exp(beta_uniform * (EXPLOOKUPFIRST - arg));
}
#endif

#if defined(LOCALIZED)
static inline double fastexp_localized(int arg) {
  arg = EXPLOOKUPFIRST - arg;
  if (arg < EXPLOOKUPSIZE && (arg > 0))
    return localized_exp_lookup[arg];
  else
    return __builtin_exp(beta_localized * (EXPLOOKUPFIRST - arg));
}
#endif
#endif

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

#if (defined(XLINK) && !defined(UNIFORM) && !defined(LOCALIZED) \
     && !defined(HARDCORE) && !defined(TOPO))
  if (lpl_index[buf_p + 1] - lpl_index[buf_p] == ODGRMAX
      || mc_time.DYN_STEPS - mc_time.t <= mc_time.RELAX_TIME)
    goto accept; // Optimization don't calculate anything
                 // but this is asking for troubles
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
#if defined(FASTEXP)
    double a = fastexp_uniform(newc_uni - oldc_uni);
#else
    double a = __builtin_exp(beta_uniform * (newc_uni - oldc_uni));
#endif
    double r = dsfmt_genrand_open_open(&dsfmt);
    if (r >= a)
      return 0;
  }
#endif
#if defined(LOCALIZED) && !defined(UNIFORM)
  if (newc_loc < oldc_loc) {
#if defined(FASTEXP)
    double a = fastexp_localized(newc_loc - oldc_loc);
#else
    double a = __builtin_exp(beta_localized * (newc_loc - oldc_loc));
#endif
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
#if defined(FASTEXP)
    double a = fastexp_localized(newc_loc - oldc_loc) *
      fastexp_uniform(newc_uni - oldc_uni);
#else
    double a = __builtin_exp(c);
#endif
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

#if defined(XLINK)
  if (mc_time.DYN_STEPS - mc_time.t > mc_time.RELAX_TIME
      && lpl_index[buf_p + 1] - lpl_index[buf_p] < ODGRMAX) {
    for (int i = 0; i < xlinklistlength; i++) {
      if (lpl_index[xlinklist[i] + 1] - lpl_index[xlinklist[i]] < ODGRMAX
	  && !connected_laplacian(buf_p, xlinklist[i])
	  // Monte-carlo
	  && dsfmt_genrand_open_open(&dsfmt) < xlink_conc) {

	// Arrived here, let's connect it
	push_in_laplacian(buf_p, xlinklist[i]);
	push_in_laplacian(xlinklist[i], buf_p);
#if defined(GETXLINK)
	fprintf(simufiles.xlkfile, "%llu\t%u\t%u\n",
		mc_time.DYN_STEPS - mc_time.t - mc_time.RELAX_TIME,
		buf_p, xlinklist[i]);
#endif

      }
    }
  }
#endif

 accept:
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

static inline void print_buffer_gz(gzFile out) {
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
int cmpint(const void *p1, const void *p2) {
  return *(int *)p1 - *(int *)p2;
}

static __attribute__ ((noinline))
void set_param_normalized(int enne, double big_sigma,
			  double energy_uniform, double energy_localized,
			  double volume_variable, double xlinker_conc) {
  N = enne;

  if (N % 16) {
    fprintf(stderr, "N should be a multiple of %d\n", 16);
    exit(-1);
  }

  lambda = sqrt(5.0 / 3.0 / N);
  double sigmal = cbrt(3.0 * big_sigma / (4.0 * N * M_PI));
#if (defined(HARDCORE) || defined(UNIFORM) \
     || defined(LOCALIZED))
  sigma = sigmal;
#else
  float sigma = 0;
#endif
#if defined(UNIFORM)
  alfa_uniform = /* 2 * cbrt(0.01 / N); */ /*2*/ 1.44224957 * sigmal;
  beta_uniform = energy_uniform;
#endif
#ifdef LOCALIZED
#warning Localized interaction range setted by default to uniform ones
  alfa_localized =  /* 2 * cbrt(0.01 / N); */ /*2*/ 1.44224957 * sigmal;
  beta_localized = energy_localized;
#endif
#ifdef CONFINEMENT
  conf_sqradius = cbrt(volume_variable) * cbrt(volume_variable);
#endif
#ifdef XLINK
  xlink_conc = xlinker_conc;
#endif

  D = 0.6 * (lambda);

#ifdef HARDCORE
  comp_hc = _mm256_set1_ps(sigma*sigma);
#endif
#ifdef UNIFORM
  comp_sb = _mm256_set1_ps(alfa_uniform*alfa_uniform);
#endif
#ifdef LOCALIZED
  comp_sp = _mm256_set1_ps(alfa_localized*alfa_localized);
#endif
#ifdef TOPO
  //conservative topological cutoff 
  comp_top = _mm256_set1_ps(4.0f*lambda*4.0f*lambda);
#endif
#ifdef XLINK
  double xlink_rad =  cbrt(1./ pow(64, 3)
			   * (pow(lambda, 3) - pow(sigma, 3))
			   + pow(sigma, 3));

  comp_xl = _mm256_set1_ps(xlink_rad * xlink_rad);
#endif

#ifdef FASTEXP
  populate_lookup_tables();
#endif
}

static void allocate_memory() {
  size_t dotsalloc = (3 * N * sizeof(float));
#if defined(LOCALIZED)
  size_t locmaskalloc = N / 4 * sizeof(uint8_t);
#else
  size_t locmaskalloc = (size_t)0;
#endif
#if defined(XLINK)
  size_t xlinklistalloc = N * sizeof(int);
#else
  size_t xlinklistalloc = (size_t)0;
#endif
#if ((NUM_THREADS > 1) && defined(XLINK))
  size_t crx_indexalloc = (N + 4) * sizeof(int);
  size_t crxalloc       = ODGRMAX * N * sizeof(int);
#else
  size_t crx_indexalloc = (size_t)0;
  size_t crxalloc       = (size_t)0;
#endif
#if defined(TOPO)
  size_t topolistalloc = N * sizeof(int);
#else
  size_t topolistalloc = (size_t)0;
#endif
  size_t lpl_indexalloc = (N + 4) * sizeof(int);
  size_t lplalloc = ODGRMAX * N * sizeof(int);
  buffer_size = dotsalloc + locmaskalloc + xlinklistalloc + topolistalloc +
    lpl_indexalloc + lplalloc + crx_indexalloc + crxalloc;
  if(posix_memalign((void **)&buffer, 32,
		    buffer_size)) {
    fprintf(stderr, "Error allocating memory\n");
    exit(-8);
  }
  dots.x = (float *restrict)buffer;
  dots.y = (float *restrict)buffer + N;
  dots.z = (float *restrict)buffer + N + N;
#if defined(LOCALIZED)
  locmask = (uint8_t *restrict)((uint8_t *)buffer + dotsalloc);
#endif
#if defined(XLINK)
  xlinklist = (int *restrict)((uint8_t *)buffer + dotsalloc + locmaskalloc);
#endif
#if defined(TOPO)
  topolist = (int *restrict)((uint8_t *)buffer + dotsalloc + locmaskalloc
			     + xlinklistalloc);
#endif
  lpl_index = (int *restrict)((uint8_t *)buffer + dotsalloc + locmaskalloc
			      + xlinklistalloc + topolistalloc);
  lpl = (int *restrict)((uint8_t *)buffer + dotsalloc + locmaskalloc
			+ xlinklistalloc + topolistalloc + lpl_indexalloc);
#if ((NUM_THREADS > 1) && defined(XLINK))
  crx_index = (int *restrict)((uint8_t *)buffer + dotsalloc + locmaskalloc
			      + xlinklistalloc + topolistalloc
			      + lpl_indexalloc + lplalloc);
  crx = (int *restrict)((uint8_t *)buffer + dotsalloc + locmaskalloc
			+ xlinklistalloc + topolistalloc + lpl_indexalloc
			+ lpl_indexalloc + lplalloc + crx_indexalloc);
#endif
}

static __attribute__ ((noinline))
void set_laplacian_null() {
  // linear laplacian
  if (ODGRMAX < 2) {
    fputs("I need more memory, please modify constant ODGRMAX"
	  " in simprivate.h\n", stderr);
    exit(-4);
  }

  for (int i = 0; i < N; i++) {
    lpl_index[i] = 2 * i;
    lpl[2 * i] = i - 1;
    lpl[2 * i + 1] = i + 1;
  }
  lpl_index[N] = 2 * N;
  lpl[0] = 0;
  lpl[2 * N - 1] = N - 1;

#if ((NUM_THREADS > 1) && defined(XLINK))
  for (int i = 0; i < N + 4; i++)
    crx_index[i] = 0;
#endif
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
  float galfa_uniform = gsigma + lambda/1000;
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
  unsigned long long int filetime    = 0ULL;

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
#if defined(HARDCORE)
	if (distance < (sigma*sigma)) {
	  fprintf(stderr, "Wrong initial conditions --> %d %d\n",
		  i, k);
	  exit(-2);
	}
#endif
#if defined(UNIFORM)
	if (distance < alfa_uniform * alfa_uniform)
	  if (k > i)
	    contacts++;
#endif
#if defined(LOCALIZED)
        if (distance < alfa_localized * alfa_localized)
          if (k > i)
	    if (is_loc_interacting(k) && is_loc_interacting(i))
	      contacts_loc++;
#endif
      }
  }
}

static __attribute__ ((noinline))
char *out_filename(const char *outstring, const char *ext) {
#define MAXFILENAME 255
  const char dir[] = "out/";
  static char filenames[4][MAXFILENAME + 1];
  static int cur = 0;

  char *filename = filenames[cur++];
  if (cur > 3) cur = 0;
  size_t lenght = strlen(ext) + strlen(outstring) + strlen(dir) + 1;

  if(lenght > MAXFILENAME) {
    fprintf(stderr, "Filename too long\n");
    exit(-12);
  }

  strcpy(filename, dir); strcat(filename, outstring);
  strcat(filename, ext);

  return filename;
}

static __attribute__ ((noinline))
void openfiles(const char *outstring,
	       const char *mode) {
  simufiles.xyzfile = gzopen(out_filename(outstring, ".xyz.dat.gz"), mode);
#if defined(GETXLINK)
  simufiles.xlkfile = fopen(out_filename(outstring, ".xlk"), mode);
#endif
  simufiles.accfile = fopen(out_filename(outstring, ".acc.dat"), mode);
  simufiles.ctcfile = fopen(out_filename(outstring, ".ctc.dat"), mode);

  if (simufiles.xyzfile == NULL || simufiles.xlkfile == NULL ||
      simufiles.accfile == NULL || simufiles.ctcfile == NULL) {
    fprintf(stderr, "Could not open files\n");
    exit(-13);
  }
}

__attribute__ ((noinline))
void closefiles() {
  gzclose(simufiles.xyzfile);
  fclose(simufiles.accfile);
  fclose(simufiles.ctcfile);
#if defined (GETXLINK)
  fclose(simufiles.xlkfile);
#endif
  close_infofile(infos);
}

__attribute__ ((noinline))
void flushfiles() {
  fflush(stderr);
  fflush(stdout);
  gzflush(simufiles.xyzfile, Z_SYNC_FLUSH);
  fflush(simufiles.accfile);
  fflush(simufiles.ctcfile);
#if defined (GETXLINK)
  fflush(simufiles.xlkfile);
#endif
  flush_infofile(infos);
}

void *simulazione(void *threadarg) {

  // This sets all denormal numbers to zero - DAZ and FTZ flags
  //  _mm_setcsr( _mm_getcsr() | 0x8040 );

  //
  // Initialization code, set global variables and files
  //
  char *outstring = ((struct thread_data *) threadarg) -> argv[2];
  char *cnffilepath = ((struct thread_data *) threadarg) -> argv[3];
  char *lplfilepath = ((struct thread_data *) threadarg) -> argv[4];
  char *locfilepath = ((struct thread_data *) threadarg) -> argv[5];
  set_param_normalized(atoi(((struct thread_data *)threadarg) -> argv[1]),
		       atof(((struct thread_data *)threadarg) -> argv[6]),
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

  allocate_memory();

  // put laplacian online from file or automatically
  load_laplacian(lplfilepath);

  // Set initial xyz configuration
  if (!strcmp(cnffilepath, "RAND")) {
    set_configuration_rand();
  } else if (!strcmp(cnffilepath, "NULL")) {
    set_configuration_null();
  } else  {
    FILE *cnffile = fopen(cnffilepath, "r");
    load_configuration(cnffile, cnffilepath);
  }

  // Check if info file is already there and parsable
  // infos contains a file pointer as well as a list
  // of infos compiled by append or open
  infos = open_infofile(out_filename(outstring, ".info"));
  struct list_infofile *checkpoint;
  if (infos == NULL) {
    infos = create_infofile(out_filename(outstring, ".info"));
    openfiles(outstring, "w");
    checkpoint = NULL;
  } else {
    seed = (get_infofile(infos, "SEED", 0)).u;
    // take the last
    checkpoint = (get_infofile(infos, "CHECKPOINT", -1)).list;
    if (!checkpoint) {
      fprintf(stderr, "Info file found, but no checkpoint was made\n");
      exit(-4);
    }
    openfiles(outstring, "a");
  }

  // put locally interacting beads from or automatically
  load_localized_stuff(locfilepath);

  // count the contacts in order to put variables online
  count_contacts();

  //
  // Simulation code, here starts the simulation
  //

  /* Prova */
  /**/
  mc_time.CORRL_TIME = 2*458ULL * N * N / 10;
  mc_time.RELAX_TIME = mc_time.CORRL_TIME / 2; // 200ULL
  mc_time.STATISTIC  = 256;
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
  const int STATISTIC = 1;
  */

  mc_time.DYN_STEPS = mc_time.RELAX_TIME
    + mc_time.CORRL_TIME * mc_time.STATISTIC + 1;

  // Print infos only we are not starting from checkpoint,
  // otherwise check info
  if (!checkpoint) {
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
#if !defined(XLINK)
    double xlink_conc = NAN;
#endif

    struct data_infofile d;
    d.time = time(NULL);         d.type = is_time_infofile; append_infofile(infos, "STARTTIME", d);
    d.d    = N;                  d.type = is_d_infofile;    append_infofile(infos, "N", d);
    d.s    = cnffilepath;        d.type = is_s_infofile; append_infofile(infos, "cfgfile", d);
    d.s    = lplfilepath;        d.type = is_s_infofile; append_infofile(infos, "lplfile", d);
    d.g    = sigma;              d.type = is_g_infofile; append_infofile(infos, "sigma", d);
    d.g    = lambda;             d.type = is_g_infofile; append_infofile(infos, "lambda", d);
    d.g    = D;                  d.type = is_g_infofile; append_infofile(infos, "D", d);
    d.g    = alfa_uniform;       d.type = is_g_infofile; append_infofile(infos, "alfa_uninform", d);
    d.g    = alfa_localized;     d.type = is_g_infofile; append_infofile(infos, "alfa_localized", d);
    d.g    = beta_uniform;       d.type = is_g_infofile; append_infofile(infos, "beta_uniform", d);
    d.g    = beta_localized;     d.type = is_g_infofile; append_infofile(infos, "beta_localized", d);
    d.g    = conf_sqradius;      d.type = is_g_infofile; append_infofile(infos, "conf_sqradius", d);
    d.g    = xlink_conc;         d.type = is_g_infofile; append_infofile(infos, "xlink_conc", d);
    d.u    = seed;               d.type = is_u_infofile; append_infofile(infos, "SEED", d);
    d.llu  = mc_time.RELAX_TIME; d.type = is_llu_infofile; append_infofile(infos, "RELAX_TIME", d);
    d.llu  = mc_time.CORRL_TIME; d.type = is_llu_infofile; append_infofile(infos, "CORRL_TIME", d);
    d.d    = mc_time.STATISTIC;  d.type = is_d_infofile; append_infofile(infos, "STATISTIC", d);
    d.llu  = mc_time.DYN_STEPS;  d.type = is_llu_infofile; append_infofile(infos, "DYN_STEPS", d);
    flush_infofile(infos);
  } else {
    if (       (get_infofile(infos, "N", 0)).d                != N             ||
	strcmp((get_infofile(infos, "cfgfile", 0)).s          ,  cnffilepath)   ||
	strcmp((get_infofile(infos, "lplfile", 0)).s          ,  lplfilepath)   ||
	fabs(  (get_infofile(infos, "lambda", 0)).g - lambda) >  0.00001 * lambda ||
	fabs(  (get_infofile(infos, "D", 0)).g - D)           >  0.00001 * D) {

      fputs("Checkpoint found, but some parameters does not match\n", stderr);
      exit(-4);
    }
  }

#if defined(GETPERF)
  displ = 0.0;
  struct timeval t2;
  struct timeval *then = &t1;
  struct timeval *now = &t2;
  gettimeofday(then, NULL);
#endif

  unsigned int accepted = 0;
  unsigned int total = 0;

  mc_time.t = mc_time.DYN_STEPS;

  unsigned long long int toprint = mc_time.t -
    (mc_time.RELAX_TIME + mc_time.CORRL_TIME);

  // Checkpoint initialization
  init_checkpoint(out_filename(outstring, ".checkpoint"),
		  accepted, total, toprint);

  if (checkpoint) {
    load_checkpoint((checkpoint -> next -> data).s,
		    &accepted,
		    &total,
		    &toprint);
  }

#if NUM_THREADS > 1
  pthread_barrier_wait(&startbarr);
  pthread_barrier_wait(&firstbarr);
#endif

  for ( ; mc_time.t != 0; mc_time.t-- ) {
#if NUM_THREADS > 1
    pthread_spin_lock (&spinsum);
#endif
    // TODO: - Extimate the correlation time and use it for measurements
    //       - load with datafiles from fortran simulation

    if ( unlikely(mc_time.t == toprint) ) {
      toprint -= mc_time.CORRL_TIME;

      prepare_checkpoint(accepted, total, toprint);

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
      fprintf(simufiles.accfile, "%llu\t%d\t%d\t%g\t%llu\t%g\t%g\n",
	      mc_time.DYN_STEPS - mc_time.t,
	      accepted, total,
	      (float)accepted / total, ellapsed_time,
	      displ, displ / ellapsed_time);

      displ = 0.0;
      struct timeval *temp = now;
      now = then; then = temp;
#else
      fprintf(simufiles.accfile, "%llu\t%g\n",
	      mc_time.DYN_STEPS - mc_time.t,
	      (float) accepted / total);
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
	      contacts_loc
#else
              0
#endif
	      );
#endif

#if (defined(XLINK) && !defined(UNIFORM) && !defined(LOCALIZED))
      // Optimization, just finish
      // but this is asking for troubles
      if (lpl_index[N] >= ODGRMAX * N)
	break;
#endif

    }

    if (move_ele())
      accepted++;
    total++;

#if NUM_THREADS > 1
    pthread_spin_unlock (&spinsum);
#endif

  }

  {
    struct data_infofile d;
    d.time = time(NULL); d.type = is_time_infofile; append_infofile(infos, "ENDTIME", d);
  }
  flushfiles();

  return NULL;
}
