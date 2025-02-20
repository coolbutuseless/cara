
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
// Blit with rotozoom
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_blit_(SEXP dst_, SEXP src_, SEXP x_, SEXP y_, 
              SEXP hjust_, SEXP vjust_, 
              SEXP angle_, 
              SEXP xscale_, SEXP yscale_,
              SEXP filter_,
              SEXP params_) {
  
  cairo_t *dst = cairo_extptr_to_cairo_t(dst_);
  cairo_t *src = cairo_extptr_to_cairo_t(src_);
  
  cairo_save(dst);
  cr_params_set(dst, params_);
  cairo_filter_t filter = parse_filter(filter_);
  
  int N = calc_max_length(4, x_, y_, angle_, xscale_, yscale_);
  bool freex = false, freey = false;
  bool freeangle = false, freexscale= false, freeyscale = false;
  double *x = as_dbl_vec(x_, N, &freex);
  double *y = as_dbl_vec(y_, N, &freey);
  double *angle = as_dbl_vec(angle_, N, &freeangle);
  double *xscale = as_dbl_vec(xscale_, N, &freexscale);
  double *yscale = as_dbl_vec(yscale_, N, &freeyscale);

  double hjust = Rf_asReal(hjust_);
  double vjust = Rf_asReal(vjust_);
  
  cairo_surface_t *src_surface = cairo_get_target(src);
  int width  = cairo_image_surface_get_width (src_surface);
  int height = cairo_image_surface_get_height(src_surface);
  
  cairo_matrix_t mat;
  cairo_get_matrix(dst, &mat);
  // translate(x, y)
  // scale(scale)
  // rotate(angle)
  // blit offset by hjust/vjust
  for (int i = 0; i < N; ++i) {
    cairo_set_matrix(dst, &mat);
    cairo_translate(dst, x[i], y[i]);
    cairo_scale(dst, xscale[i], yscale[i]);
    cairo_rotate(dst, -angle[i]);
    cairo_set_source_surface(dst, src_surface, -width * hjust, -height * vjust);
    cairo_pattern_set_filter (cairo_get_source (dst), filter);
    cairo_paint(dst);
  }
  
  cairo_set_matrix(dst, &mat);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freex) free(x);
  if (freey) free(y);
  if (freeangle) free(angle);
  if (freexscale) free(xscale);
  if (freeyscale) free(yscale);
  
  cairo_restore(dst);
  cr_check_status(dst);
  return dst_;
}




