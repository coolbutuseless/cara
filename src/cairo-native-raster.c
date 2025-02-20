
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
#include "cairo-pattern.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert a cairo raster to a native raster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_to_nr_(SEXP cr_, SEXP nr_) {
  
  int nprotect = 0;
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  
  cairo_surface_t *surface = cairo_get_target(cr);
  cairo_surface_flush(surface);
  int width = cairo_image_surface_get_width(surface);
  int height = cairo_image_surface_get_height(surface);
  
  
  if (Rf_isNull(nr_)) {
    nr_ = PROTECT(Rf_allocMatrix(INTSXP, height, width)); nprotect++;
    SEXP cls_ = PROTECT(Rf_mkString("nativeRaster")); nprotect++;
    Rf_setAttrib(nr_, R_ClassSymbol, cls_);
  } else {
    assert_nativeraster(nr_);
    int nr_width = Rf_ncols(nr_);
    int nr_height = Rf_nrows(nr_);
    if (nr_width != width || nr_height != height) {
      Rf_error("cr_to_nr_(): Destination 'nr' is not of the correct size");
    }
  }
  
  uint8_t *pixel = cairo_image_surface_get_data(surface);
  
  int *nr = INTEGER(nr_);
  memcpy(nr, pixel, width * height * 4);
  demultiply_and_swizzle((uint8_t *)nr, width, height);
  
  
  UNPROTECT(nprotect);
  return nr_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert a cairo raster to a native raster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP nr_to_cr_(SEXP nr_, SEXP cr_) {
  int nprotect = 0;
  assert_nativeraster(nr_);
  
  int width  = Rf_ncols(nr_);
  int height = Rf_nrows(nr_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Need to ensure that the valid stride value for this size image
  // is actually matched by the real width of the data
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
  if (stride != (size_t)width * sizeof(uint32_t)) {
    Rf_error("For compatibility with cairo, the expected width here should be %i", stride);
  }
  
  cairo_surface_t *surface = NULL;
  
  if (Rf_isNull(cr_)) {
    cr_ = PROTECT(cr_new(width, height)); nprotect++;
  }
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // User has aupplised a 'cr' to copy into
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  surface = cairo_get_target(cr);
  
  int cr_width  = cairo_image_surface_get_width (surface);
  int cr_height = cairo_image_surface_get_height(surface);
  
  if (cr_width != width || cr_height != height) {
    Rf_error("Destination 'cr' dimensions do not match 'nr'");
  }
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy nativeraster to cairo surface
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint8_t *cr_data = cairo_image_surface_get_data(surface);
  int *nr = INTEGER(nr_);
  memcpy(cr_data, nr, width * height * 4);
  premultiply_and_swizzle(cr_data, width, height);
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  UNPROTECT(nprotect);
  return cr_;
}


