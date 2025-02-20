
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
#include "cairo-pattern.h"



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Used internally when within R the 'externalpointer' object gets
// garbage collected
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void cairo_pattern_t_finalizer(SEXP pat_) {
  cairo_pattern_t *pat = (cairo_pattern_t *)R_ExternalPtrAddr(pat_);
  if (pat != NULL) {       
    cairo_pattern_destroy(pat);
    R_ClearExternalPtr(pat_);
  }
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Determine extensions mode for pattern
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cairo_extend_t pattern_mode(SEXP extend_) {
  
  const char *extend = CHAR(STRING_ELT(extend_, 0));
  
  cairo_extend_t mode = CAIRO_EXTEND_REPEAT;
  
  if (strcmp(extend, "none") == 0) {
    mode = CAIRO_EXTEND_NONE;
  } else if (strcmp(extend, "pad") == 0) {
    mode = CAIRO_EXTEND_PAD;
  } else if (strcmp(extend, "reflect") == 0) {
    mode = CAIRO_EXTEND_REFLECT;
  } 
  
  return mode;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// cairo_pattern_t * cairo_create (cairo_surface_t *target)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP create_pattern_image_(SEXP cr_, SEXP mode_, SEXP filter_)  {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create pattern from 'cr'
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_surface_t *surface = cairo_get_target(cr);
  cairo_pattern_t *pat = cairo_pattern_create_for_surface(surface);
  cr_pattern_status(pat);
  cairo_pattern_set_extend(pat, pattern_mode(mode_));
  cairo_pattern_set_filter(pat, parse_filter(filter_));
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create an external pointer to this cairo_pattern_t
  // Protect the source 'cr' from garbage collection
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP pat_ = PROTECT(R_MakeExternalPtr(pat, R_NilValue, cr_));
  SEXP cls_ = PROTECT(Rf_mkString("cairo_pattern_t"));
  SET_CLASS(pat_, cls_);
  R_RegisterCFinalizer(pat_, cairo_pattern_t_finalizer);
  UNPROTECT(2);
  return pat_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Linear Gradient
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP create_pattern_linear_gradient_(SEXP colors_, SEXP stops_, 
                                            SEXP x1_, SEXP y1_, SEXP x2_, SEXP y2_,
                                            SEXP mode_, SEXP filter_) {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Unpack and sanity check
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (Rf_length(colors_) != Rf_length(stops_)) {
    Rf_error("Length mismatch between 'colors' and 'stops'");
  }
  
  double x1 = Rf_asReal(x1_);
  double x2 = Rf_asReal(x2_);
  double y1 = Rf_asReal(y1_);
  double y2 = Rf_asReal(y2_);
  
  bool freecolors = false;
  uint32_t *colors = multi_rcolors_to_ints(colors_, Rf_length(colors_), &freecolors);
  double *stops = REAL(stops_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create pattern
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_pattern_t *pat = cairo_pattern_create_linear(x1, y1, x2, y2);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create stops
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < Rf_length(colors_); ++i) {
    uint32_t color = colors[i];
    double r = ( color        & 0xff) / 255.0;
    double g = ((color >>  8) & 0xff) / 255.0;
    double b = ((color >> 16) & 0xff) / 255.0;
    double a = ((color >> 24) & 0xff) / 255.0;
    cairo_pattern_add_color_stop_rgba(pat, stops[i], r, g, b, a);
  }
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Wrap cairo_pattern_t as external pointer
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cr_pattern_status(pat);
  cairo_pattern_set_extend(pat, pattern_mode(mode_));
  cairo_pattern_set_filter(pat, parse_filter(filter_));
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create an external pointer to this cairo_pattern_t
  // Protect the source 'cr' from garbage collection
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP pat_ = PROTECT(R_MakeExternalPtr(pat, R_NilValue, R_NilValue));
  SEXP cls_ = PROTECT(Rf_mkString("cairo_pattern_t"));
  SET_CLASS(pat_, cls_);
  R_RegisterCFinalizer(pat_, cairo_pattern_t_finalizer);
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freecolors) free(colors);
  UNPROTECT(2);
  return pat_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Radial gradient
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP create_pattern_radial_gradient_(SEXP colors_, SEXP stops_, 
                                            SEXP x1_, SEXP y1_, SEXP r1_, 
                                            SEXP x2_, SEXP y2_, SEXP r2_, 
                                            SEXP mode_, SEXP filter_) {
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Unpack and sanity check
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (Rf_length(colors_) != Rf_length(stops_)) {
    Rf_error("Length mismatch between 'colors' and 'stops'");
  }
  
  double x1 = Rf_asReal(x1_);
  double y1 = Rf_asReal(y1_);
  double r1 = Rf_asReal(r1_);
  double x2 = Rf_asReal(x2_);
  double y2 = Rf_asReal(y2_);
  double r2 = Rf_asReal(r2_);
  
  bool freecolors = false;
  uint32_t *colors = multi_rcolors_to_ints(colors_, Rf_length(colors_), &freecolors);
  double *stops = REAL(stops_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create pattern
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_pattern_t *pat = cairo_pattern_create_radial(x1, y1, r1, x2, y2, r2);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create stops
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < Rf_length(colors_); ++i) {
    uint32_t color = colors[i];
    double r = ( color        & 0xff) / 255.0;
    double g = ((color >>  8) & 0xff) / 255.0;
    double b = ((color >> 16) & 0xff) / 255.0;
    double a = ((color >> 24) & 0xff) / 255.0;
    cairo_pattern_add_color_stop_rgba(pat, stops[i], r, g, b, a);
  }
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Wrap cairo_pattern_t as external pointer
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cr_pattern_status(pat);
  cairo_pattern_set_extend(pat, pattern_mode(mode_));
  cairo_pattern_set_filter(pat, parse_filter(filter_));
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create an external pointer to this cairo_pattern_t
  // Protect the source 'cr' from garbage collection
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP pat_ = PROTECT(R_MakeExternalPtr(pat, R_NilValue, R_NilValue));
  SEXP cls_ = PROTECT(Rf_mkString("cairo_pattern_t"));
  SET_CLASS(pat_, cls_);
  R_RegisterCFinalizer(pat_, cairo_pattern_t_finalizer);
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freecolors) free(colors);
  UNPROTECT(2);
  return pat_;
  
}










//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cairo external pointer to 'cairo_t'
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cairo_pattern_t *cairo_pattern_extptr_to_cairo_pattern_t(SEXP pat_) {
  if (!Rf_inherits(pat_, "cairo_pattern_t")) {
    Rf_error("Not a 'cairo_pattern_t' object");
  }
  cairo_pattern_t *pat = TYPEOF(pat_) != EXTPTRSXP ? NULL : (cairo_pattern_t *)R_ExternalPtrAddr(pat_);
  if (pat == NULL) Rf_error("'cairo_pattern_t * pat' pointer is invalid/NULL");
  return pat;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Translate a pattern
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_pattern_translate_(SEXP pat_, SEXP x_, SEXP y_) {
  
  cairo_pattern_t *pat = cairo_pattern_extptr_to_cairo_pattern_t(pat_);
  double x = Rf_asReal(x_);
  double y = Rf_asReal(y_);
  
  cairo_matrix_t mat;
  cairo_pattern_get_matrix(pat, &mat);
  
  cairo_matrix_translate(&mat, -x, -y);
  
  cairo_pattern_set_matrix(pat, &mat);
  
  return pat_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Scale a pattern
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_pattern_scale_(SEXP pat_, SEXP x_, SEXP y_) {
  
  cairo_pattern_t *pat = cairo_pattern_extptr_to_cairo_pattern_t(pat_);
  double x = Rf_asReal(x_);
  double y = Rf_asReal(y_);
  
  cairo_matrix_t mat;
  cairo_pattern_get_matrix(pat, &mat);
  
  cairo_matrix_scale(&mat, 1/x, 1/y);
  
  cairo_pattern_set_matrix(pat, &mat);
  
  return pat_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Scale a pattern
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_pattern_transform_reset_(SEXP pat_) {
  
  cairo_pattern_t *pat = cairo_pattern_extptr_to_cairo_pattern_t(pat_);
  
  cairo_matrix_t mat;
  cairo_pattern_get_matrix(pat, &mat);
  
  cairo_matrix_init_identity(&mat);
  
  cairo_pattern_set_matrix(pat, &mat);
  
  return pat_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Rotate a pattern
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_pattern_rotate_(SEXP pat_, SEXP angle_) {
  
  cairo_pattern_t *pat = cairo_pattern_extptr_to_cairo_pattern_t(pat_);
  double angle = Rf_asReal(angle_);
  
  cairo_matrix_t mat;
  cairo_pattern_get_matrix(pat, &mat);
  
  cairo_matrix_rotate(&mat, angle);
  
  cairo_pattern_set_matrix(pat, &mat);
  
  return pat_;
}


