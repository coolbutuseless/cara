
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
// Convert RGB or RGBA array to native raster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP array_to_cr_(SEXP arr_, SEXP cr_) {
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanity check
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!Rf_isArray(arr_) || !Rf_isNumeric(arr_)) {
    Rf_error("'arr' must be an numeric array");
  }
  
  int height = Rf_nrows(arr_);
  int width  = Rf_ncols(arr_);
  
  SEXP arr_dim = PROTECT(GET_DIM(arr_));
  if (Rf_length(arr_dim) != 3) {
    Rf_error("Only 3-D arrays are accepted");
  }
  int depth = INTEGER(arr_dim)[2];
  if (depth != 3 && depth != 4) {
    Rf_error("'arr' must be RGB or RGBA. Not depth = %i", depth);
  }
  UNPROTECT(1);
  
  int has_alpha = (depth == 4);
  
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
  // Copy the data - from column major matrix to row major nativeRaster
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint32_t *cr_data = (uint32_t *)cairo_image_surface_get_data(cr_surface);
  
  double *r = REAL(arr_) + width * height * 0;
  double *g = REAL(arr_) + width * height * 1;
  double *b = REAL(arr_) + width * height * 2;
  double *a = REAL(arr_) + width * height * 3;
  
  if (has_alpha) {
    for (int col = 0; col < width; col++) {
      for (int row = 0; row < height; row++) {
        *(cr_data + row * width + col) = 
          ((uint32_t)(*(a + col * height + row) * 255) << 24) +
          ((uint32_t)(*(b + col * height + row) * 255) << 16) + 
          ((uint32_t)(*(g + col * height + row) * 255) <<  8) +
          ((uint32_t)(*(r + col * height + row) * 255)      );
      }
    }
  } else {
    for (int col = 0; col < width; col++) {
      for (int row = 0; row < height; row++) {
        *(cr_data + row * width + col) = 
          ((uint32_t)0xff << 24) +
          ((uint32_t)(*(b + col * height + row) * 255) << 16) + 
          ((uint32_t)(*(g + col * height + row) * 255) <<  8) +
          ((uint32_t)(*(r + col * height + row) * 255)      );
      }
    }
  }
  
  
  premultiply_and_swizzle((uint8_t *)cr_data, width, height);
  cairo_surface_mark_dirty(cr_surface);
  UNPROTECT(nprotect);
  return cr_;
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// nativeRaster to array (depth = 4)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_to_array_(SEXP cr_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_surface_t *surface = cairo_get_target(cr);
  int height = cairo_image_surface_get_height(surface);
  int width  = cairo_image_surface_get_width (surface);
  
  SEXP arr_ = PROTECT(Rf_alloc3DArray(REALSXP, height, width, 4));
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // First have to copy the data to a temporary buffer and demultiply the alpha
  // and swizzle the channels
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint32_t *cr_raw = (uint32_t *)cairo_image_surface_get_data(surface);
  uint32_t *cr_data = malloc((size_t)width * (size_t)height * 4);
  if (cr_data == NULL) Rf_error("Couldn't allocate cr_data in cr_to_raster()");
  memcpy(cr_data, cr_raw, width * height * 4);
  demultiply_and_swizzle((uint8_t *)cr_data, width, height);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy the data - from row major cairo raster to column major array
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  double *r = REAL(arr_) + width * height * 0;
  double *g = REAL(arr_) + width * height * 1;
  double *b = REAL(arr_) + width * height * 2;
  double *a = REAL(arr_) + width * height * 3;
  
  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      uint32_t val = *(cr_data + row * width + col);
      a[col * height + row] = (double)( (val >> 24) & 0xff ) / 255;
      b[col * height + row] = (double)( (val >> 16) & 0xff ) / 255;
      g[col * height + row] = (double)( (val >>  8) & 0xff ) / 255;
      r[col * height + row] = (double)( (val      ) & 0xff ) / 255;
    }
  }
  
  free(cr_data);
  UNPROTECT(1);
  return arr_;
}













