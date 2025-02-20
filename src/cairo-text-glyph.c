
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

// void           cairo_show_glyph(cairo_t, cairo_glyph_t *, int num_glyphs)
// void           cairo_show_text_glyphs()  // For embedding fonts
// void           cairo_glyph_extents()     // bounding rectangle 
// cairo_glyph_t *cairo_glyph_allocate()    // Not needed. just use malloc/free
// void           cairo_glyph_free()        // Not needed. Just use malloc/free

// cairo_glyph_t = {
//   unsigned long index;
//   double x;
//   double y;
// }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cairo glyph interface
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_text_glyph_(SEXP cr_, SEXP idx_, 
                    SEXP x_, SEXP y_, 
                    SEXP family_, SEXP size_, 
                    SEXP fill_, SEXP color_, 
                    SEXP weight_, SEXP slant_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  
  if (Rf_length(idx_) != Rf_length(x_) || Rf_length(x_) != Rf_length(y_)) {
    Rf_error("cr_glyph_(): 'idx' [%i], 'x' [%i] and 'y' [%i] must all be the same length",
             Rf_length(idx_), Rf_length(x_), Rf_length(y_));
  }
  
  uint32_t fill  = single_rcolor_to_int(fill_ );
  uint32_t color = single_rcolor_to_int(color_);
  
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
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Font family/size
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  const char *family = CHAR(STRING_ELT(family_, 0));
  double size = Rf_asReal(size_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Align vector arguments
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int N = Rf_length(idx_);
  int *idx = INTEGER(idx_);
  double *x = REAL(x_);
  double *y = REAL(y_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Allocate glyphs
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_glyph_t *glyphs = malloc((size_t)N * sizeof(cairo_glyph_t));
  if (glyphs == NULL) {
    Rf_error("cr_glyph_(): Could not allocate 'glyphs'");
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy R info into glyphs
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  for (int i = 0; i < N; ++i) {
    glyphs[i].index = (unsigned long)idx[i];
    glyphs[i].x     = x[i];
    glyphs[i].y     = y[i];
  }
  
  cairo_select_font_face(cr, family, slant, weight);
  cairo_set_font_size(cr, size);
  
  // cairo_show_glyphs(cr, glyphs, N);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Define the glyph path
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_glyph_path(cr, glyphs, N);
  
  // Draw outline
  set_color(cr, color);
  cairo_stroke_preserve(cr);
  
  // Draw fill
  set_color(cr, fill);
  cairo_fill(cr);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  free(glyphs);
  cairo_restore(cr);
  return cr_;
}


// nocov end
