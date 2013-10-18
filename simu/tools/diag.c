
// find eigenvalues of all the matrices

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Constants
const double M_SQRT3 = 1.73205080756887729352744634151;   // sqrt(3)

// Macros
static inline double SQR(double x) { return ((x)*(x)); }
static inline int EL(int j, int k) { return 3*j + k; }

static __attribute__ ((noinline))
int cmpfloat_inv(const void *p1, const void *p2) {
  return (*(float *)p1 - *(float *)p2 < 0 ? 1 : -1);
}

// ----------------------------------------------------------------------------
// Calculates the eigenvalues of a symmetric 3x3 matrix A using Cardano's
// analytical algorithm.
// Only the diagonal and upper triangular parts of A are accessed. The access
// is read-only.
// ----------------------------------------------------------------------------
// Parameters:
//   A: The symmetric input matrix
//   w: Storage buffer for eigenvalues
// ----------------------------------------------------------------------------
static void CardanoDiag3x3(float A[9], float w[3])
{
  double m, c1, c0;
  
  // Determine coefficients of characteristic poynomial. We write
  //       | a   d   f  |
  //  A =  | d   b   e  |
  //       | f   e   c  |
  double de = A[EL(0,1)] * A[EL(1,2)];                              // d*e
  double dd = SQR(A[EL(0,1)]);                                      // d^2
  double ee = SQR(A[EL(1,2)]);                                      // e^2
  double ff = SQR(A[EL(0,2)]);                                      // f^2
  m  = A[EL(0,0)] + A[EL(1,1)] + A[EL(2,2)];
  c1 = (A[EL(0,0)]*A[EL(1,1)] + A[EL(0,0)]*A[EL(2,2)] + A[EL(1,1)]*A[EL(2,2)])
    // a*b + a*c + b*c - d^2 - e^2 - f^2
    - (dd + ee + ff);
  c0 = A[EL(2,2)]*dd 
    + A[EL(0,0)]*ee
    + A[EL(1,1)]*ff
    - A[EL(0,0)]*A[EL(1,1)]*A[EL(2,2)]
    - 2.0 * A[EL(0,2)]*de;   // c*d^2 + a*e^2 + b*f^2 - a*b*c - 2*f*d*e)

  double p, sqrt_p, q, c, s, phi;
  p = SQR(m) - 3.0*c1;
  q = m*(p - (3.0/2.0)*c1) - (27.0/2.0)*c0;
  sqrt_p = sqrt(fabs(p));

  phi = 27.0 * ( 0.25*SQR(c1)*(p - c1) + c0*(q + 27.0/4.0*c0));
  phi = (1.0/3.0) * atan2(sqrt(fabs(phi)), q);

  c = sqrt_p*cos(phi);
  s = (1.0/M_SQRT3)*sqrt_p*sin(phi);

  w[1]  = (1.0/3.0)*(m - c);
  w[2]  = w[1] + s;
  w[0]  = w[1] + c;
  w[1] -= s;

  qsort(w, 3, sizeof(float), cmpfloat_inv);

}

int main(void) {

  unsigned long long int t;
  float inertia[9];
  float w[3];

  while (scanf("%llu\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\t%f\n", &t,
	       inertia, inertia+1, inertia+2,
	       inertia+3, inertia+4, inertia+5,
	       inertia+6, inertia+7, inertia+8) == 10) {

    CardanoDiag3x3(inertia, w);
    printf("%llu\t%g\t%g\t%g\n", t, w[0], w[1], w[2]);
  }

}
