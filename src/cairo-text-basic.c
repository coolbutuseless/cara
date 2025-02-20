
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

// nocov start

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Fill a cairo object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_text_basic_(SEXP cr_, SEXP text_, 
                    SEXP x_, SEXP y_,
                    SEXP family_, SEXP size_,
                    SEXP fill_, SEXP color_, 
                    SEXP weight_, SEXP slant_, 
                    SEXP hjust_, SEXP vjust_) {
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Font name
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  const char *family = CHAR(STRING_ELT(family_, 0));
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Colours
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint32_t color = single_rcolor_to_int(color_);
  uint32_t fill  = single_rcolor_to_int(fill_);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Weight: "normal" or "bold"
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_font_weight_t weight = CAIRO_FONT_WEIGHT_NORMAL;
  if (!Rf_isNull(weight_)) {
    const char *weight_str = CHAR(STRING_ELT(weight_, 0));
    if (strcmp(weight_str, "bold") == 0) {
      weight = CAIRO_FONT_WEIGHT_BOLD;
    }
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // vectorise
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int N = calc_max_length(3, text_, x_, y_);
  if (Rf_length(text_) != N) {
    Rf_error("cr_text_basic_(): length of 'text' must match coordinates");
  }
  
  bool freex = false, freey = false;
  double *x = as_dbl_vec(x_, N, &freex);
  double *y = as_dbl_vec(y_, N, &freey);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Slant: "normal", "italic" or "oblique"
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_font_slant_t slant = CAIRO_FONT_SLANT_NORMAL;
  if (!Rf_isNull(slant_)) {
    const char *slant_str = CHAR(STRING_ELT(slant_, 0));
    if (strcmp(slant_str, "italic") == 0) {
      slant = CAIRO_FONT_SLANT_ITALIC;
    } else if (strcmp(slant_str, "oblique") == 0) {
      slant = CAIRO_FONT_SLANT_OBLIQUE;
    }
  }
  
  double hjust = Rf_asReal(hjust_);
  double vjust = Rf_asReal(vjust_);
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Select font and move to start location
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_select_font_face(cr, family, slant, weight);
  cairo_set_font_size(cr, Rf_asReal(size_));
  
  for (int i = 0; i < N; ++i) {
    const char *str = CHAR(STRING_ELT(text_, i));
    
    // Calculate size of text
    cairo_text_extents_t extents = { 0 };
    cairo_text_extents(cr, str, &extents);
    
    // Adjust position with justification
    cairo_move_to(cr, x[i] - hjust * extents.width, y[i] + vjust * extents.height);
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // Fill and stroke path
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    // cairo_show_text(cr, str);
    cairo_text_path(cr, str);
    
    
    set_color(cr, color);
    cairo_stroke_preserve(cr);
    
    set_color(cr, fill);
    cairo_fill(cr);
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freex) free(x);
  if (freey) free(y);
  
  cairo_restore(cr);
  return cr_;
}


// nocov end
