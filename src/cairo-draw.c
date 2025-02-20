
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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Draw rectangles
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_rect_(SEXP cr_, SEXP x_, SEXP y_, SEXP w_, SEXP h_, SEXP fill_, SEXP color_, 
              SEXP hjust_, SEXP vjust_, SEXP params_) {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Unpack cairo_t
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  cr_params_set(cr, params_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Setup vectorised arguments
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  double hjust = Rf_asReal(hjust_);
  double vjust = Rf_asReal(vjust_);
  
  int N = calc_max_length(4, x_, y_, w_, h_);
  bool freex = false, freey = false, freew = false, freeh = false;
  double *x = as_dbl_vec(x_, N, &freex);
  double *y = as_dbl_vec(y_, N, &freey);
  double *w = as_dbl_vec(w_, N, &freew);
  double *h = as_dbl_vec(h_, N, &freeh);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Setup color and fill/pattern
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  bool freecolor = false, freefill = false;
  uint32_t *color = multi_rcolors_to_ints(color_, N, &freecolor);
  
  uint32_t *fill = NULL;
  cairo_pattern_t *pat = NULL;
  if (Rf_inherits(fill_, "cairo_pattern_t")) {
    pat = cairo_pattern_extptr_to_cairo_pattern_t(fill_);
  } else {
    fill  = multi_rcolors_to_ints(fill_ , N, &freefill);
  }
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Apply drawing operation
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < N; i++) {
    if (fill == NULL) {
      cairo_set_source(cr, pat);
    } else {
      set_color(cr, fill[i]);
    }
    cairo_rectangle(cr, x[i] - hjust * w[i], y[i] - vjust * h[i], w[i], h[i]);
    cairo_fill_preserve(cr);
    set_color(cr, color[i]);
    cairo_stroke(cr);
  }
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freex) free(x);
  if (freey) free(y);
  if (freew) free(w);
  if (freeh) free(h);
  if (freecolor) free(color);
  if (freefill) free(fill);
  
  cairo_restore(cr);
  cr_check_status(cr);
  return cr_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Draw line segments 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_line_(SEXP cr_, SEXP x0_, SEXP y0_, SEXP x1_, SEXP y1_, SEXP color_, SEXP params_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  cr_params_set(cr, params_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Vectorise coords
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int N = calc_max_length(4, x0_, y0_, x1_, y1_);
  bool freex0 = false, freey0 = false, freex1 = false, freey1 = false;
  double *x0 = as_dbl_vec(x0_, N, &freex0);
  double *y0 = as_dbl_vec(y0_, N, &freey0);
  double *x1 = as_dbl_vec(x1_, N, &freex1);
  double *y1 = as_dbl_vec(y1_, N, &freey1);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Vectorise colors
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  bool freecol = false;
  uint32_t *color = multi_rcolors_to_ints(color_, N, &freecol);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Draw lines
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < N; i++) {
    set_color(cr, color[i]);
    cairo_move_to(cr, x0[i], y0[i]);
    cairo_line_to(cr, x1[i], y1[i]);
    cairo_stroke(cr);
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freex0) free(x0);
  if (freey0) free(y0);
  if (freex1) free(x1);
  if (freey1) free(y1);
  if (freecol) free(color);
  
  cairo_restore(cr);
  cr_check_status(cr);
  return cr_;
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Draw line segments 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_spline_(SEXP cr_, SEXP x1_, SEXP y1_, SEXP x2_, SEXP y2_, SEXP cx1_, 
                SEXP cy1_, SEXP cx2_, SEXP cy2_, SEXP color_, SEXP params_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  cr_params_set(cr, params_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Vectorise coords
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int N = calc_max_length(8, x1_, y1_, x2_, y2_, cx1_, cy1_, cx2_, cy2_);
  bool freex1 = false, freey1 = false, freex2 = false, freey2 = false;
  bool freecx1 = false, freecy1 = false, freecx2 = false, freecy2 = false;
  double *x1 = as_dbl_vec(x1_, N, &freex1);
  double *y1 = as_dbl_vec(y1_, N, &freey1);
  double *x2 = as_dbl_vec(x2_, N, &freex2);
  double *y2 = as_dbl_vec(y2_, N, &freey2);
  
  double *cx1 = as_dbl_vec(cx1_, N, &freecx1);
  double *cy1 = as_dbl_vec(cy1_, N, &freecy1);
  double *cx2 = as_dbl_vec(cx2_, N, &freecx2);
  double *cy2 = as_dbl_vec(cy2_, N, &freecy2);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Vectorise colors
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  bool freecol = false;
  uint32_t *color = multi_rcolors_to_ints(color_, N, &freecol);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Draw lines
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < N; i++) {
    set_color(cr, color[i]);
    cairo_move_to(cr, x1[i], y1[i]);
    cairo_curve_to(cr, cx1[i], cy1[i], cx2[i], cy2[i], x2[i], y2[i]);
    cairo_stroke(cr);
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freex1) free(x1);
  if (freey1) free(y1);
  if (freex2) free(x2);
  if (freey2) free(y2);
  if (freecx1) free(cx1);
  if (freecy1) free(cy1);
  if (freecx2) free(cx2);
  if (freecy2) free(cy2);
  if (freecol) free(color);
  
  cairo_restore(cr);
  cr_check_status(cr);
  return cr_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Draw circles
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_arc_(SEXP cr_, SEXP x_, SEXP y_, SEXP r_, SEXP angle1_, SEXP angle2_, SEXP clockwise_, SEXP fill_, SEXP color_, SEXP params_) {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Setup cairo_t
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  cr_params_set(cr, params_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Setup vectorised values
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int N = calc_max_length(5, x_, y_, r_, angle1_, angle2_);
  bool freex = false, freey = false, freer = false;
  bool freeangle1 = false, freeangle2 = false;
  double *x = as_dbl_vec(x_, N, &freex);
  double *y = as_dbl_vec(y_, N, &freey);
  double *r = as_dbl_vec(r_, N, &freer);
  double *angle1 = as_dbl_vec(angle1_, N, &freeangle1);
  double *angle2 = as_dbl_vec(angle2_, N, &freeangle2);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Setup color and fill/patern
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  bool freecolor = false, freefill = false;
  uint32_t *color = multi_rcolors_to_ints(color_, N, &freecolor);
  
  uint32_t *fill = NULL;
  cairo_pattern_t *pat = NULL;
  if (Rf_inherits(fill_, "cairo_pattern_t")) {
    pat = cairo_pattern_extptr_to_cairo_pattern_t(fill_);
  } else {
    fill  = multi_rcolors_to_ints(fill_ , N, &freefill);
  }  

  bool clockwise = Rf_asInteger(clockwise_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // 2. Apply drawing operation
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < N; i++) {
    if (fill == NULL) {
      cairo_set_source(cr, pat);
    } else {
      set_color(cr, fill[i]);
    }
    
    if (clockwise) {
      cairo_arc_negative(cr, x[i], y[i], r[i], angle1[i], angle2[i]);
    } else {
      cairo_arc(cr, x[i], y[i], r[i], angle1[i], angle2[i]);
    }
    cairo_fill_preserve(cr);
    set_color(cr, color[i]);
    cairo_stroke(cr);
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freex) free(x);
  if (freey) free(y);
  if (freer) free(r);
  if (freeangle1) free(angle1);
  if (freeangle2) free(angle2);
  if (freecolor) free(color);
  if (freefill) free(fill);
  
  cairo_restore(cr);
  cr_check_status(cr);
  return cr_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Draw line segments 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_polyline_(SEXP cr_, SEXP x_, SEXP y_, SEXP color_, SEXP params_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  cr_params_set(cr, params_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Vectorise coords
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int N = calc_max_length(2, x_, y_);
  if (N < 2) return(cr_);
  bool freex = false, freey = false;
  double *x = as_dbl_vec(x_, N, &freex);
  double *y = as_dbl_vec(y_, N, &freey);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // single color only
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint32_t color = single_rcolor_to_int(color_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Draw lines
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  set_color(cr, color);
  cairo_move_to(cr, x[0], y[0]);
  for (int i = 1; i < N; i++) {
    cairo_line_to(cr, x[i], y[i]);
  }
  cairo_stroke(cr);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freex) free(x);
  if (freey) free(y);
  
  cairo_restore(cr);
  cr_check_status(cr);
  return cr_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Draw line segments 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_polygon_(SEXP cr_, SEXP x_, SEXP y_, SEXP fill_, SEXP color_, SEXP params_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  cr_params_set(cr, params_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Vectorise coords
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int N = calc_max_length(2, x_, y_);
  if (N < 2) return(cr_);
  bool freex = false, freey = false;
  double *x = as_dbl_vec(x_, N, &freex);
  double *y = as_dbl_vec(y_, N, &freey);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // single color only
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint32_t color = single_rcolor_to_int(color_);
  
  uint32_t fill = 0;
  cairo_pattern_t *pat = NULL;
  if (Rf_inherits(fill_, "cairo_pattern_t")) {
    pat = cairo_pattern_extptr_to_cairo_pattern_t(fill_);
  } else {
    fill  = single_rcolor_to_int(fill_);
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Draw lines + fill
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_move_to(cr, x[0], y[0]);
  for (int i = 1; i < N; i++) {
    cairo_line_to(cr, x[i], y[i]);
  }
  cairo_close_path(cr);
  if (pat != NULL) {
    cairo_set_source(cr, pat);
  } else {
    set_color(cr, fill);
  }
  cairo_fill_preserve(cr);
  set_color(cr, color);
  cairo_stroke(cr);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freex) free(x);
  if (freey) free(y);
  
  cairo_restore(cr);
  cr_check_status(cr);
  return cr_;
}





