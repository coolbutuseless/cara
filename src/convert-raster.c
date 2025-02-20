
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
// Convert raster to cairo raster
//
// @param ras_ Raster object. i.e. character matrix
// @param dst_ Optional pre-allocated destination nativeRaster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP raster_to_cr_(SEXP ras_, SEXP cr_) {

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanity check
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!Rf_isMatrix(ras_) || TYPEOF(ras_) != STRSXP) {
    Rf_error("'raster' must be an character matrix");
  }

  int height = Rf_nrows(ras_);
  int width  = Rf_ncols(ras_);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Prep native raster
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  int nprotect = 0;

  
  if (Rf_isNull(cr_)) {
    cr_ = PROTECT(cr_new(width, height)); nprotect++;
  }
  
  cairo_t *dst = cairo_extptr_to_cairo_t(cr_);
  cairo_surface_t *cr_surface = cairo_get_target(dst);
  int cr_width = cairo_image_surface_get_width(cr_surface);
  int cr_height = cairo_image_surface_get_height(cr_surface); 
  
  if (height != cr_height || width !=  cr_width) {
    Rf_error("Supplied 'dst' cairo raster dimensions (w:%i, h:%i) do not match source matrix (w:%i, h:%i)",
             cr_width, cr_height, width, height);
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy the data - from column major raster to row major cairo raster
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint32_t *cr_data = (uint32_t *)cairo_image_surface_get_data(cr_surface);


  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      const char *val = CHAR(STRING_ELT(ras_, col * height + row));
      *(cr_data + col * height + row) = col_to_int(val);
    }
  }

  premultiply_and_swizzle((uint8_t *)cr_data, width, height);
  cairo_surface_mark_dirty(cr_surface);

  UNPROTECT(nprotect);
  return cr_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// nativeRaster to raster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_to_raster_(SEXP cr_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_surface_t *surface = cairo_get_target(cr);
  int height = cairo_image_surface_get_height(surface);
  int width  = cairo_image_surface_get_width (surface);

  SEXP ras_ = PROTECT(Rf_allocMatrix(STRSXP, height, width));
  SEXP class_ = PROTECT(Rf_mkString("raster"));
  SET_CLASS(ras_, class_);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy the data - from row major cairo raster to column major matrix/raster
  // First have to copy the data to a temporary buffer and demultiply the alpha
  // and swizzle the channels
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint32_t *cr_raw = (uint32_t *)cairo_image_surface_get_data(surface);
  uint32_t *cr_data = malloc((size_t)width * (size_t)height * 4);
  if (cr_data == NULL) Rf_error("Couldn't allocate cr_data in cr_to_raster()");
  memcpy(cr_data, cr_raw, width * height * 4);
  demultiply_and_swizzle((uint8_t *)cr_data, width, height);

  char buf[10];
  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      int_to_col(cr_data[col * height + row], buf);
      SET_STRING_ELT(ras_, col * height + row, Rf_mkChar(buf));
    }
  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  free(cr_data);
  UNPROTECT(2);
  return ras_;
}

