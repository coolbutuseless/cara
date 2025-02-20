
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
// Begin masked drawing
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_mask_begin_(SEXP cr_, SEXP mask_, SEXP x_, SEXP y_) {
  
  int nprotect = 0;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Mask is sane
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_t *mask = cairo_extptr_to_cairo_t(mask_);
  cr_check_status(mask);
  
  if (!Rf_isNumeric(x_) || !Rf_isNumeric(y_)) {
    Rf_error("cr_mask_begin_(): 'x' and 'y' must be numeric");
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Unpack the meta information and ensure sanity
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  SEXP meta_  = R_ExternalPtrProtected(cr_);
  if (!Rf_isNewList(meta_) || Rf_length(meta_) != META_LENGTH) {
    Rf_error("'prot' on 'cr_' is expected to be a list()");
  }
  SEXP mask_cur = VECTOR_ELT(meta_, META_MASK);
  if (!Rf_isNull(mask_cur)) {
    Rf_error("cr_mask_begin_() called, but a mask is already present");
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // basic surface information
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_surface_t *cr_surface   = cairo_get_target(cr);
  cairo_surface_flush(cr_surface);
  int cr_width  = cairo_image_surface_get_width (cr_surface);
  int cr_height = cairo_image_surface_get_height(cr_surface);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Do we have a cache?  If not, create one
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP cache_ = VECTOR_ELT(meta_, META_CACHE);
  if (Rf_isNull(cache_)) {
    cache_ = PROTECT(cr_new(cr_width, cr_height)); nprotect++;
    SET_VECTOR_ELT(meta_, META_CACHE, cache_);
  }
  cairo_t *cache = cairo_extptr_to_cairo_t(cache_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy the current 'cr' surface into a cache
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_surface_t *cache_surface = cairo_get_target(cache);
  uint8_t *cache_data = cairo_image_surface_get_data(cache_surface);
  uint8_t *cr_data    = cairo_image_surface_get_data(cr_surface);
  
  memcpy(cache_data, cr_data, cr_width * cr_height * 4);
  cairo_surface_mark_dirty(cache_surface);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Store 'mask', 'x' and 'y'
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SET_VECTOR_ELT(meta_, META_MASK  , mask_);
  SET_VECTOR_ELT(meta_, META_MASK_X, x_);
  SET_VECTOR_ELT(meta_, META_MASK_Y, y_);
  
  
  UNPROTECT(nprotect);
  return cr_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// End masked drawing
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_mask_end_(SEXP cr_) {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Unpack the cairo raster
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cr_check_status(cr);
  cairo_surface_t *cr_surface = cairo_get_target(cr);
  cairo_surface_flush(cr_surface);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Unpack the meta information and ensure sanity
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP meta_  = R_ExternalPtrProtected(cr_);
  if (!Rf_isNewList(meta_) || Rf_length(meta_) != META_LENGTH) {
    Rf_error("'prot' on 'cr_' is expected to be a list()");
  }
  SEXP mask_ = VECTOR_ELT(meta_, META_MASK);
  if (Rf_isNull(mask_)) {
    Rf_error("cr_mask_end_() called, but no mask is present");
  }
  double x = Rf_asReal(VECTOR_ELT(meta_, META_MASK_X));
  double y = Rf_asReal(VECTOR_ELT(meta_, META_MASK_Y));
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Unpack Mask Surface
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_t *mask = cairo_extptr_to_cairo_t(mask_);
  cr_check_status(mask);
  cairo_surface_t *mask_surface = cairo_get_target(mask);
  cairo_surface_flush(mask_surface);
  int cr_width  = cairo_image_surface_get_width (cr_surface);
  int cr_height = cairo_image_surface_get_height(cr_surface);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Unpack Cache surface
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP cache_ = VECTOR_ELT(meta_, META_CACHE);
  cairo_t *cache = cairo_extptr_to_cairo_t(cache_);
  cr_check_status(cache);
  cairo_surface_t *cache_surface = cairo_get_target(cache);
  cairo_surface_flush(cache_surface);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Masked Paint from 'cr' -> 'cache' (using 'mask')
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_set_source_surface(cache, cr_surface, 0, 0); // Paint from 'cr' to 'cache'
  cairo_mask_surface(cache, mask_surface, x, y);     // using the 'mask'
  cairo_surface_flush(cache_surface);  
    
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy 'cache_data' to 'cr_data'
  // set 'cr_surface' as dirty
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  uint8_t *cache_data = cairo_image_surface_get_data(cache_surface);
  uint8_t *cr_data    = cairo_image_surface_get_data(cr_surface);
  
  memcpy(cr_data, cache_data, cr_width * cr_height * 4);
  
  cairo_surface_mark_dirty(cr_surface);
  
  // Remove the mask
  SET_VECTOR_ELT(meta_, META_MASK, R_NilValue);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  return cr_;
}





