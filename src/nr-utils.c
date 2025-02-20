
#define R_NO_REMAP

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include "include/cairo.h"

#include "nr-utils.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Assert object is a native raster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void assert_nativeraster(SEXP nr_) {
  if (Rf_isInteger(nr_) && !Rf_inherits(nr_, "nativeRaster")) {
    Rf_error("Object is not a nativeRaster");
  }
} 


void premultiply_and_swizzle(uint8_t *pixel, int width, int height) {
  for (int i = 0; i < width * height; i++, pixel += 4) {
    double alpha = *(pixel + 3) / 255.0;
    uint8_t tmp    = (uint8_t)(*(pixel + 0) * alpha);
    *(pixel + 0)   = (uint8_t)(*(pixel + 2) * alpha);
    *(pixel + 1)   = (uint8_t)(*(pixel + 1) * alpha);
    *(pixel + 2)   = tmp;
  }
}


void demultiply_and_swizzle(uint8_t *pixel, int width, int height) {
  for (int i = 0; i < width * height; i++, pixel += 4) {
    double alpha = *(pixel + 3) / 255.0;
    if (alpha == 0) {
      *(pixel + 0) = 0;
      *(pixel + 1) = 0;
      *(pixel + 2) = 0;
    } else {
      uint8_t tmp    = (uint8_t)(*(pixel + 0) / alpha);
      *(pixel + 0)   = (uint8_t)(*(pixel + 2) / alpha);
      *(pixel + 1)   = (uint8_t)(*(pixel + 1) / alpha);
      *(pixel + 2)   = tmp;
    }
  }
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert an R INTEGER or REAL to a C  "double *"
// 
// @param vec_ R SEXP
// @param N Expected Rf_length
// @param do_Free was new memory allocat in this function (the caller should free())
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
double *as_dbl_vec(SEXP vec_, int N, bool *do_free) {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Must be REALSXP or INTSXP
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!Rf_isInteger(vec_) && !Rf_isReal(vec_)) {
    Rf_error("as_dbl_vec(): Expecting numeric but got %s\n", Rf_type2char((SEXPTYPE)TYPEOF(vec_)));
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Must be Rf_length 1 or N
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (Rf_length(vec_) != 1 && Rf_length(vec_) != N) {
    Rf_error("as_dbl_vec(): Input vector must be Rf_length 1 or N, not %i", Rf_length(vec_));
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Do we have an integer vector of the correct Rf_length?
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (Rf_isReal(vec_) && Rf_length(vec_) == N) {
    return REAL(vec_);
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // If we got here:
  //   - need to allocate memory for an integer vector
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  *do_free = 1;
  double *dbl_vec = malloc((size_t)N * sizeof(double));
  if (dbl_vec == NULL) {
    Rf_error("as_dbl_vec(): out of memory");
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // If given INTSXP
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (Rf_isReal(vec_)) {
    // Can only be an REALSXP of Rf_length=1 on this branch
    double value = Rf_asReal(vec_);
    for (int i = 0; i < N; i++) {
      dbl_vec[i] = value;
    }
  } else {
    // INTSXP
    int *vec = INTEGER(vec_);
    if (Rf_length(vec_) == N) {
      for (int i = 0; i < N; i++) {
        dbl_vec[i] = vec[i];
      }
    } else {
      // Rf_length == 1
      double value = (double)vec[0];
      for (int i = 0; i < N; i++) {
        dbl_vec[i] = value;
      }
    }
  }
  
  return dbl_vec;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Calculate group N
//
// Calculate the maximum Rf_length out of a number of SEXP objects
//
// @param ... multiple numeric SEXP variables
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int calc_max_length(int count, ...) {
  va_list ap;
  va_start(ap, count);
  
  int N = -1;
  
  for (int i = 0; i < count; i++) {
    SEXP x = va_arg(ap, SEXP);
    int this_N = Rf_length(x);
    N = this_N > N ? this_N : N;
  }
  
  if (N < 0) {
    Rf_error("calc_ma_length(): -1");
  }
  
  return N;
}
