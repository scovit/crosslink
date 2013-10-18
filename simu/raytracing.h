#ifndef DEF_RAYTRACING_H
#define DEF_RAYTRACING_H

#include <xmmintrin.h>
#include <stdbool.h>
#include <stdint.h>


struct RTSSEVec3f{
  float t[12];
};

static __m128 raytr_sp0 __attribute__((aligned(16)));
static __m128 raytr_sp1 __attribute__((aligned(16)));
static __m128 raytr_sp2 __attribute__((aligned(16)));
static __m128 raytr_sea0 __attribute__((aligned(16)));
static __m128 raytr_sea1 __attribute__((aligned(16)));
static __m128 raytr_sea2 __attribute__((aligned(16)));
static __m128 raytr_seb0 __attribute__((aligned(16)));
static __m128 raytr_seb1 __attribute__((aligned(16)));
static __m128 raytr_seb2 __attribute__((aligned(16)));
static struct RTSSEVec3f o_sse __attribute__ ((aligned (16)));
static struct RTSSEVec3f d_sse __attribute__ ((aligned (16)));
static int fillstate_triaccel;
static const uint8_t fillmask[5] = { 0x0, 0x1, 0x3, 0x7, 0xF };

/*
  Fast, Minimum Storage Ray/Triangle Intersection
  Tomas Moller & Ben Trumbore
  look at igad.nhtv.nl/~bikker/files/faster.pdf for an sse version
*/
// Here start the topological interaction
static inline void vecdiff(float *restrict r0, float *restrict r1,
			    float *restrict r2,
			    const float a0, const float a1, const float a2,
			    const float b0, const float b1, const float b2) {
  *r0 = a0 - b0;
  *r1 = a1 - b1;
  *r2 = a2 - b2;
}

static inline void veccross(float *restrict r0, float *restrict r1,
			    float *restrict r2,
			    const float a0, const float a1, const float a2,
			    const float b0, const float b1, const float b2) {
  *r0 = a1 * b2 - a2 * b1;
  *r1 = a2 * b0 - a0 * b2;
  *r2 = a0 * b1 - a1 * b0;
}

static inline float vecdot(const float a0, const float a1, const float a2,
			    const float b0, const float b1, const float b2) {
  return a0 * b0 + a1 * b1 + a2 * b2;
}

/*
static inline double vecdot(const float a0, const float a1, const float a2,
			    const float b0, const float b1, const float b2) {
  return (double)a0 * (double)b0 +
    (double)a1 * (double)b1 + 
    (double)a2 * (double)b2;
    }*/

static inline void vecdiff_sse(__m128 *restrict r0, __m128 *restrict r1,
			    __m128 *restrict r2,
			    const __m128 a0, const __m128 a1, const __m128 a2,
			    const __m128 b0, const __m128 b1, const __m128 b2) {
  *r0 = _mm_sub_ps(a0, b0);
  *r1 = _mm_sub_ps(a1, b1);
  *r2 = _mm_sub_ps(a2, b2);
}

static inline void veccross_sse(__m128 *restrict r0, __m128 *restrict r1,
			    __m128 *restrict r2,
			    const __m128 a0, const __m128 a1, const __m128 a2,
			    const __m128 b0, const __m128 b1, const __m128 b2) {
  *r0 = _mm_sub_ps(_mm_mul_ps(a1, b2), _mm_mul_ps(a2, b1));
  *r1 = _mm_sub_ps(_mm_mul_ps(a2, b0), _mm_mul_ps(a0, b2));
  *r2 = _mm_sub_ps(_mm_mul_ps(a0, b1), _mm_mul_ps(a1, b0));
}

static inline __m128 vecdot_sse(const __m128 a0, const __m128 a1,
				const __m128 a2, const __m128 b0,
				const __m128 b1, const __m128 b2) {
  return _mm_add_ps(_mm_add_ps(_mm_mul_ps(a0, b0), _mm_mul_ps(a1, b1)),
		    _mm_mul_ps(a2, b2));
}

static bool
intersect_triangle(const float orig0, const float orig1,
		   const float orig2, const float dir0,
		   const float dir1, const float dir2,
		   const float p0, const float p1, const float p2,
		   const float ea0, const float ea1, const float ea2,
		   const float eb0, const float eb1, const float eb2) {
  float tvec0, tvec1, tvec2, pvec0, pvec1, pvec2, qvec0, qvec1, qvec2;
  float det, inv_det;
  float t, detu, detv, dett; // We don't need to pass as parameters
  
  /*fprintf(stderr, "%f %f %f %f %f %f\t",
    orig0, orig1, orig2, dir0, dir1, dir2 );*/

  /* begin calculating determinant - also used to calculate U parameter */
  veccross(&pvec0, &pvec1, &pvec2, 
	   dir0, dir1, dir2, eb0, eb1, eb2);

  /* if determinant is near zero, ray lies in plane of triangle */
  det = vecdot(ea0, ea1, ea2, pvec0, pvec1, pvec2);
  
  static const double EPSILON = 0.000001;

  /*  if ((det > -EPSILON) && (det < EPSILON))
      return false;*/

  /* calculate distance from p to ray origin */
  vecdiff(&tvec0, &tvec1, &tvec2,
	  orig0, orig1, orig2, p0, p1, p2);

  /* calculate U parameter and test bounds */
  detu = vecdot(tvec0, tvec1, tvec2, pvec0, pvec1, pvec2);
  
  /* prepare to test V parameter */
  veccross(&qvec0, &qvec1, &qvec2,
	   tvec0, tvec1, tvec2, ea0, ea1, ea2);

  /* calculate V parameter and test bounds */
  detv = vecdot(dir0, dir1, dir2, qvec0, qvec1, qvec2);


  if (signbit(detv)!=signbit(detu) ||
      signbit(det - detu - detv) != signbit(detu))
    return false;

  /* calculate t, ray intersects triangle */
  dett = vecdot(eb0, eb1, eb2, qvec0, qvec1, qvec2);
  /*  t = dett / det;
  if (!signbit(t) && signbit(t-1))
  */
  if (signbit(dett) != signbit(det) ||
      signbit(det - dett) != signbit(det))
    return false;
  else
    return true;

}

static inline bool 
intersect_triangle_sse(uint8_t mask) {

  __m128 dir0 = _mm_load_ps(d_sse.t);
  __m128 dir1 = _mm_load_ps(d_sse.t + 4);
  __m128 dir2 = _mm_load_ps(d_sse.t + 8);

  __m128 pvec0, pvec1, pvec2;
  /* begin calculating determinant - also used to calculate U parameter */
  veccross_sse(&pvec0, &pvec1, &pvec2,
	       dir0, dir1, dir2, raytr_seb0, raytr_seb1, raytr_seb2);

  /* if determinant is near zero, ray lies in plane of triangle */
  __m128 det = vecdot_sse(raytr_sea0, raytr_sea1, raytr_sea2,
			  pvec0, pvec1, pvec2);
  
  /*  static const double EPSILON = 0.000001; */

  /*  if ((det > -EPSILON) && (det < EPSILON))
      return false;*/

  __m128 tvec0, tvec1, tvec2;
  __m128 orig0 = _mm_load_ps(o_sse.t);
  __m128 orig1 = _mm_load_ps(o_sse.t + 4);
  __m128 orig2 = _mm_load_ps(o_sse.t + 8);

  /* calculate distance from p to ray origin */
  vecdiff_sse(&tvec0, &tvec1, &tvec2,
	      orig0, orig1, orig2, raytr_sp0, raytr_sp1, raytr_sp2);

  /* calculate U parameter and test bounds */
  __m128 detu = vecdot_sse(tvec0, tvec1, tvec2, pvec0, pvec1, pvec2);

  /* { */
  /* /\* First branch (U) - not good for performances *\/ */
  /*   __mm128 tmpdet0 = _mm_xor_ps(_mm_sub_ps(det, detu), det); */
  /*   __mm128 tmpdet1 = _mm_xor_ps(detu, det); */
  /*   mask = ~(_mm_movemask_ps(_mm_or_ps(tmpdet0, tmpdet1))) & mask; */
  /*   if (mask == 0) */
  /*     return false; */
  /* } */

  __m128 qvec0, qvec1, qvec2;
  /* prepare to test V parameter */
  veccross_sse(&qvec0, &qvec1, &qvec2,
	       tvec0, tvec1, tvec2, raytr_sea0, raytr_sea1, raytr_sea2);

  /* calculate V parameter and test bounds */
  __m128 detv = vecdot_sse(dir0, dir1, dir2, qvec0, qvec1, qvec2);

  {
  /* Second branch (cone) */
    __m128 tmpdet0 = _mm_xor_ps(_mm_sub_ps(det, _mm_add_ps(detu, detv)), detu);
    __m128 tmpdet1 = _mm_xor_ps(detv, detu);       // different sign
    mask = ~(_mm_movemask_ps(_mm_or_ps(tmpdet0, tmpdet1))) & mask;
    if (mask == 0)
      return false;
  }

  {
    __m128 dett = vecdot_sse(raytr_seb0, raytr_seb1, raytr_seb2,
			     qvec0, qvec1, qvec2);

    /* Third branch (t test) */
    __m128 tmpdet0 = _mm_xor_ps(_mm_sub_ps(det, dett), det);
    __m128 tmpdet1 = _mm_xor_ps(dett, det);
    mask = ~(_mm_movemask_ps(_mm_or_ps(tmpdet0, tmpdet1))) & mask;
    if (mask == 0)
      return false;
  }

  return true;

  /* /\* This is the less performant branchless version *\/ */
  /* __m128 dett = vecdot_sse(raytr_seb0, raytr_seb1, raytr_seb2, */
  /*   qvec0, qvec1, qvec2); */

  /* /\* cone test *\/ */
  /* tmpdet0 = _mm_sub_ps(det, _mm_add_ps(detu, detv)); */
  /* tmpdet0 = _mm_xor_ps(tmpdet0, detu);    // different sign */
  /* tmpdet1 = _mm_xor_ps(detv, detu);       // different sign */
  /* __m128 conetest = _mm_or_ps(tmpdet0, tmpdet1); */
  
  /* /\* /\* t test *\/ */
  /* tmpdet0 = _mm_xor_ps(_mm_sub_ps(det, dett), det); */
  /* tmpdet1 = _mm_xor_ps(dett, det); */
  /* __m128 ttest = _mm_or_ps(tmpdet0, tmpdet1); */
  /* mask = (~_mm_movemask_ps(_mm_or_ps(conetest, ttest))) & mask; */
  /* if (mask == 0) */
  /*   return false; */
  /* else */
  /*   return true; */
}

#endif
