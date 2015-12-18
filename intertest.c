#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <math.h>
#include <xmmintrin.h>

// Here start the topological interaction
static inline void vecdiff(float r[3], float a[3], float b[3]) {
  r[0] = a[0]-b[0]; r[1] = a[1]-b[1]; r[2] = a[2]-b[2];
}

static inline void veccross(float r[3], float a[3], float b[3]) {
  r[0] = a[1] * b[2] - a[2] * b[1];
  r[1] = a[2] * b[0] - a[0] * b[2];
  r[2] = a[0] * b[1] - a[1] * b[0];
}

static inline double vecdot(float a[3], float b[3]) {
  return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}


/*
  Fast, Minimum Storage Ray/Triangle Intersection
  Tomas Moller & Ben Trumbore
*/
bool intersect_triangle(float orig[3], float dir[3],
			float vert0[3], float vert1[3], float vert2[3]) {
  float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];
  double det, inv_det;
  double t, u, v; // We don't need to pass as parameters
  bool returnval = true;
  
  /* find vectors for two edges sharing vert0 */
  vecdiff(edge1, vert1, vert0);
  vecdiff(edge2, vert2, vert0);

  /* begin calculating determinant - also used to calculate U parameter */
  veccross(pvec, dir, edge2);

  /* if determinant is near zero, ray lies in plane of triangle */
  det = vecdot(edge1, pvec);
  
  /* the non-culling branch */
  static const float EPSILON = 0.000001f;
  if (det > -EPSILON && det < EPSILON)
    returnval = false;
  inv_det = 1.0 / det;

  /* calculate distance from vert0 to ray origin */
  vecdiff(tvec, orig, vert0);

  /* calculate U parameter and test bounds */
  u = vecdot(tvec, pvec) * inv_det;
  if ((u < 0.0) || (u > 1.0))
    returnval = false;
  
  /* prepare to test V parameter */
  veccross(qvec, tvec, edge1);

  /* calculate V parameter and test bounds */
  v = vecdot(dir, qvec) * inv_det;
  if ((v < 0.0) || (u + v > 1.0))
    returnval = false;

  /* calculate t, ray intersects triangle */
  t = vecdot(edge2, qvec) * inv_det;
  /* start: added by me */
  if ((t < 0.0) || (t > 1.0))
    returnval = false;
  /* end: added by me */

  printf("%.10g %.10g %.10g %.10g\n", det, u, v, t);
  return returnval;
}

// look at igad.nhtv.nl/~bikker/files/faster.pdf

int main (int argc, char **argv) {
  // test orario, antiorario, dentro fuori puntante etc
  float A[3];
  float B[3];
  float C[3];
  float V1[3];
  float V0[3];
  int bogus1,bogus2;
  scanf("%d %d %f %f %f", &bogus1, &bogus2, &A[0],&A[1],&A[2]);
  scanf("%d %d %f %f %f", &bogus1, &bogus2, &B[0],&B[1],&B[2]);
  scanf("%d %d %f %f %f", &bogus1, &bogus2, &C[0],&C[1],&C[2]);
  scanf("%d %d %f %f %f", &bogus1, &bogus2, &V0[0],&V0[1],&V0[2]);
  scanf("%d %d %f %f %f", &bogus1, &bogus2, &V1[0],&V1[1],&V1[2]);



  float D[3]; vecdiff(D, V1, V0);

  printf("Il risultato è %d\n",
	 (int) intersect_triangle(V0, D, A, B, C));

  return 0;
}
