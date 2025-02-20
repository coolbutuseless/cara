
#define R_NO_REMAP

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>
#include "include/cairo.h"

#include "nr-utils.h"
#include "color.h"
#include "cairo-core.h"
#include "cairo-params.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Translate
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_translate_(SEXP cr_, SEXP tx_, SEXP ty_) {
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_translate(cr, Rf_asReal(tx_), Rf_asReal(ty_));
  return(cr_); 
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Scale
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_scale_(SEXP cr_, SEXP sx_, SEXP sy_) {
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_scale(cr, Rf_asReal(sx_), Rf_asReal(sy_));
  return(cr_); 
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Rotate
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_rotate_(SEXP cr_, SEXP angle_) {
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_rotate(cr, -Rf_asReal(angle_));
  return(cr_); 
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Reset the transformation matrix back to the identity
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_transform_reset_(SEXP cr_) {
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_identity_matrix(cr);
  return(cr_);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Overwrite transform with given matrix values
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_transform_set_(SEXP cr_, SEXP xx_, SEXP yx_, SEXP xy_, SEXP yy_, SEXP x0_, SEXP y0_) {
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  
  cairo_matrix_t mat = {
    .xx = Rf_asReal(xx_),
    .yx = Rf_asReal(yx_),
    .xy = Rf_asReal(xy_),
    .yy = Rf_asReal(yy_),
    .x0 = Rf_asReal(x0_),
    .y0 = Rf_asReal(y0_)
  };
  
  cairo_set_matrix(cr, &mat);
  
  return(cr_);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Augment the current transformation matrix with the given values
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_transform_(SEXP cr_, SEXP xx_, SEXP yx_, SEXP xy_, SEXP yy_, SEXP x0_, SEXP y0_) {
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  
  cairo_matrix_t mat = {
    .xx = Rf_asReal(xx_),
    .yx = Rf_asReal(yx_),
    .xy = Rf_asReal(xy_),
    .yy = Rf_asReal(yy_),
    .x0 = Rf_asReal(x0_),
    .y0 = Rf_asReal(y0_)
  };
  
  cairo_transform(cr, &mat);
  
  return(cr_);
}








