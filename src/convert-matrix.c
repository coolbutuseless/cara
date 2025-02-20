
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
// Convert a numeric matrix to a native raster using a palette
//
// @param mat_ numeric matrix
// @param palette_ color palette
// @param min,max the limits of the matrix (used for inteprolating the value to a color)
// @param cr optional pre-allocated destination
//
// @return native raster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP numeric_matrix_to_cr_(SEXP mat_, SEXP palette_, SEXP min_, SEXP max_, SEXP cr_) {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanity check
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!Rf_isMatrix(mat_) || !Rf_isReal(mat_)) {
    Rf_error("'mat' must be an numeric matrix");
  }
  
  int N = Rf_length(palette_); // Maximum palette color
  
  int height = Rf_nrows(mat_);
  int width  = Rf_ncols(mat_);
  
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
  // Copy the data - from column major matrix to row major nativeRaster
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  double *mat = REAL(mat_);
  
  bool freepal = false;
  uint32_t *palette = multi_rcolors_to_ints(palette_, N, &freepal);
  
  double min = Rf_asReal(min_);
  double max = Rf_asReal(max_);
  double range_scale = 1.0 / (max - min);
  
  uint32_t *cr_data = (uint32_t *)cairo_image_surface_get_data(cr_surface);
  
  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      double val = *(mat + col * height + row);
      size_t idx = (size_t)round((val - min) * range_scale * (N - 1));
      if (idx >= N || idx < 0) {
        Rf_error("Value in matrix out of range (%.1f, %.1f): %.3f [%zu/%i]", min, max, val, idx, N);
      }
      *(cr_data + row * width + col) = palette[idx];
    }
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Mark surface as dirty
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  premultiply_and_swizzle((uint8_t *)cr_data, width, height);
  cairo_surface_mark_dirty(cr_surface);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freepal) free(palette);
  UNPROTECT(nprotect);
  return cr_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert a matrix to a native raster using a palette
//
// @param mat_ numeric matrix
// @param palette_ color palette
// @param min,max the limits of the matrix (used for inteprolating the value to a color)
// @param cr optional pre-allocated destination
//
// @return native raster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP matrix_to_cr_(SEXP mat_, SEXP palette_, SEXP fill_, SEXP min_, SEXP max_, SEXP cr_) {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Sanity check
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (!Rf_isMatrix(mat_)) {
    Rf_error("'mat' must be an matrix");
  }
  
  if (Rf_isReal(mat_)) {
    return numeric_matrix_to_cr_(mat_, palette_, min_, max_, cr_);
  } 
  
  if (!Rf_isInteger(mat_)) {
    Rf_error("'mat' must be a numeric or integer matrix");
  }
  
  
  int N = Rf_length(palette_); // Maximum palette color
  
  int height = Rf_nrows(mat_);
  int width  = Rf_ncols(mat_);
  
  uint32_t fill = single_rcolor_to_int(fill_);
  
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
  // Copy the data - from column major matrix to row major nativeRaster
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint32_t *cr_data = (uint32_t *)cairo_image_surface_get_data(cr_surface);
  int *mat = INTEGER(mat_);
  
  bool freepal = false;
  uint32_t *palette = multi_rcolors_to_ints(palette_, N, &freepal);
  
  for (int col = 0; col < width; col++) {
    for (int row = 0; row < height; row++) {
      int val = *(mat + col * height + row);
      if (val > N) {
        Rf_error("not enough colors in palette. Need at least %i but have only %i", val, N);
      }
      if (val <= 0) {
        *(cr_data + row * width + col) = fill;  
      } else {
        *(cr_data + row * width + col) = palette[val - 1];
      }
    }
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Mark surface as dirty
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  premultiply_and_swizzle((uint8_t *)cr_data, width, height);
  cairo_surface_mark_dirty(cr_surface);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (freepal) free(palette);
  UNPROTECT(nprotect);
  return cr_;
}


