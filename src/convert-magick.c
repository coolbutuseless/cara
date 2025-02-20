
#define R_NO_REMAP

#include <R.h>
#include <Rinternals.h>
#include <Rdefines.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "include/cairo.h"

#include <colorfast.h>

#include "color.h"
#include "cairo-core.h"
#include "nr-utils.h"


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert magick image to cairo raster
//
// @param im_data_ the data part of a magick image: magick::image_data(im)
// @param cr_ optional pre-allocated destination nativeRaster
//
// @return cr_
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP magick_to_cr_(SEXP im_data_, SEXP cr_) {
  
  if (TYPEOF(im_data_) != RAWSXP) {
    Rf_error("Must be a raw vector");
  }
  
  SEXP dim_ = GET_DIM(im_data_);
  if (Rf_length(dim_) != 3) {
    Rf_error("Must be a 3D array");
  }
  
  if (INTEGER(dim_)[0] != 4) {
    Rf_error("Must be an RGBA image");
  }
  
  int width  = INTEGER(dim_)[1];
  int height = INTEGER(dim_)[2];
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Prep native raster
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int nprotect = 0;
  
  if (Rf_isNull(cr_)) {
    cr_ = PROTECT(cr_new(width, height)); nprotect++;
  } 
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_surface_t *cr_surface = cairo_get_target(cr);
  int cr_width = cairo_image_surface_get_width(cr_surface);
  int cr_height = cairo_image_surface_get_height(cr_surface); 
  
  if (height != cr_height || width !=  cr_width) {
    Rf_error("Supplied 'cr' cairo raster dimensions (w:%i, h:%i) do not match source matrix (w:%i, h:%i)",
             cr_width, cr_height, width, height);
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy the RGBA bytes directly into the nativeRaster
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint8_t *cr_data = cairo_image_surface_get_data(cr_surface);
  memcpy(cr_data, RAW(im_data_), (size_t)width * (size_t)height * sizeof(uint32_t));
  
  premultiply_and_swizzle((uint8_t *)cr_data, width, height);
  cairo_surface_mark_dirty(cr_surface);
  
  UNPROTECT(nprotect);
  return cr_;
}

