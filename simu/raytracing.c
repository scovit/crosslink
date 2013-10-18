#include <math.h>
#include <stdio.h>
#include "raytracing.h"

void calculate_TriAccell(struct TriAccel *restrict res,
			 const float *restrict p,
			 const float *restrict e0,
			 const float *restrict e1) {
  // story extra points sse
  res -> sp[0] = _mm_set1_ps(p[0]);
  res -> sp[1] = _mm_set1_ps(p[1]);
  res -> sp[2] = _mm_set1_ps(p[2]);
  res -> sea[0] = _mm_set1_ps(e0[0]);
  res -> sea[1] = _mm_set1_ps(e0[1]);
  res -> sea[2] = _mm_set1_ps(e0[2]);
  res -> seb[0] = _mm_set1_ps(e1[0]);
  res -> seb[1] = _mm_set1_ps(e1[1]);
  res -> seb[2] = _mm_set1_ps(e1[2]);

}

bool intersect_triangle(const float orig0, const float orig1,
			const float orig2, const float dir0,
			const float dir1, const float dir2,
			const float p0, const float p1, const float p2,
			const float ea0, const float ea1, const float ea2,
			const float eb0, const float eb1, const float eb2) {
  float tvec0, tvec1, tvec2, pvec0, pvec1, pvec2, qvec0, qvec1, qvec2;
  float det, inv_det;
  float t, detu, detv; // We don't need to pass as parameters
  
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
  t = vecdot(eb0, eb1, eb2, qvec0, qvec1, qvec2) / det;
  if (!signbit(t) && signbit(t-1))
  /*  if ((t < 0.0) || (t > 1.0))*/
    return true;
  else
    return false;

}
