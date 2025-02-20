
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Used internally when within R the 'externalpointer' object gets
// garbage collected
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void cairo_t_finalizer(SEXP cr_) {
  cairo_t *cr = (cairo_t *)R_ExternalPtrAddr(cr_);
  // Rprintf("gc(): cairo_t (0)\n");
  if (cr != NULL) {
    // Rprintf("gc(): cairo_t (1)\n");                    
    
    cairo_surface_t *surface = cairo_get_target(cr);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    
    R_ClearExternalPtr(cr_);
  }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Cairo external pointer to 'cairo_t'
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cairo_t *cairo_extptr_to_cairo_t(SEXP cr_) {
  if (!Rf_inherits(cr_, "cairo_t")) {
    Rf_error("Not a 'cairo_t' context");
  }
  cairo_t *cr = TYPEOF(cr_) != EXTPTRSXP ? NULL : (cairo_t *)R_ExternalPtrAddr(cr_);
  if (cr == NULL) Rf_error("'cairo_t * cr' pointer is invalid/NULL");
  return cr;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_new(int width, int height) {
  
  int nprotect = 0;
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create a surface 
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_surface_t *surface = cairo_image_surface_create(
    CAIRO_FORMAT_ARGB32,
    width,
    height
  );
  cairo_status_t status = cairo_surface_status(surface);
  if (status != CAIRO_STATUS_SUCCESS) {
    Rf_error("Cairo surface failure: %s", cairo_status_to_string(status));
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // cairo_create() never returns a NULL, but instead will return a special 
  // object with a special status if its out of memory
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_t * cr = cairo_create(surface);
  if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    Rf_error("Cairo 'cairo_t' failure: %s", cairo_status_to_string(status));
  }
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // list() to be stored in TAG for external pointer
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP meta_ = PROTECT(Rf_allocVector(VECSXP, META_LENGTH)); nprotect++;
  for (int i = 0; i < META_LENGTH; ++i) {
    SET_VECTOR_ELT(meta_, i, R_NilValue);
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create external pointer
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  SEXP cr_ = PROTECT(R_MakeExternalPtr(cr, R_NilValue, meta_)); nprotect++;
  SEXP cls_ = PROTECT(Rf_mkString("cairo_t")); nprotect++;
  SET_CLASS(cr_, cls_);
  R_RegisterCFinalizer(cr_, cairo_t_finalizer);
  
  
  UNPROTECT(nprotect);
  return cr_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// cairo_t * cairo_create (cairo_surface_t *target)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_new_(SEXP width_, SEXP height_, SEXP fill_)  {
  
  int width  = Rf_asInteger(width_);
  int height = Rf_asInteger(height_);
  
  SEXP cr_ = PROTECT(cr_new(width, height));
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Fill 
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  if (Rf_inherits(fill_, "cairo_pattern_t")) {
    cairo_pattern_t *pat = cairo_pattern_extptr_to_cairo_pattern_t(fill_);
    cairo_set_source(cr, pat);
  } else {
    uint32_t fill = single_rcolor_to_int(fill_);
    set_color(cr, fill);
  }
  cairo_paint(cr);

  UNPROTECT(1);
  return cr_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert a cairo raster to a native raster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_duplicate_(SEXP cr_) {
  int nprotect = 0;
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  
  cairo_surface_t *src_surface = cairo_get_target(cr);
  int width  = cairo_image_surface_get_width (src_surface);
  int height = cairo_image_surface_get_height(src_surface);
  
  SEXP dst_ = PROTECT(cr_new(width, height)); nprotect++;
  cairo_t *dst = cairo_extptr_to_cairo_t(dst_);
  cairo_surface_t *dst_surface = cairo_get_target(dst);
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy nativeraster to cairo surface
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_surface_flush(src_surface);
  uint8_t *src_pixels = cairo_image_surface_get_data(src_surface);
  uint8_t *dst_pixels = cairo_image_surface_get_data(dst_surface);
  
  memcpy(dst_pixels, src_pixels, width * height * 4);
  cairo_surface_mark_dirty(dst_surface);
  
  UNPROTECT(nprotect);
  return dst_;
}






//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Convert a cairo raster to a native raster
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_copy_into_(SEXP dst_, SEXP src_) {
  
  cairo_t *src = cairo_extptr_to_cairo_t(src_);
  cairo_surface_t *src_surface = cairo_get_target(src);
  int src_width  = cairo_image_surface_get_width (src_surface);
  int src_height = cairo_image_surface_get_height(src_surface);
  
  cairo_t *dst = cairo_extptr_to_cairo_t(dst_);
  cairo_surface_t *dst_surface = cairo_get_target(dst);
  int dst_width  = cairo_image_surface_get_width (dst_surface);
  int dst_height = cairo_image_surface_get_height(dst_surface);
  
  if (dst_width != src_width || dst_height != src_height) {
    Rf_error("cr_copy_into_(): Dimensions do not match");
  }
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Copy source into destination
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_surface_flush(src_surface);
  uint8_t *src_pixels = cairo_image_surface_get_data(src_surface);
  uint8_t *dst_pixels = cairo_image_surface_get_data(dst_surface);
  memcpy(dst_pixels, src_pixels, src_width * src_height * 4);
  cairo_surface_mark_dirty(dst_surface);
  
  return dst_;
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Fill a cairo object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_fill_(SEXP cr_, SEXP fill_) {
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  cairo_save(cr);
  
  
  if (Rf_inherits(fill_, "cairo_pattern_t")) {
    cairo_pattern_t *pat = cairo_pattern_extptr_to_cairo_pattern_t(fill_);
    cairo_set_source(cr, pat);
  } else {
    uint32_t fill = single_rcolor_to_int(fill_);
    set_color(cr, fill);
  }
  
  cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
  cairo_paint(cr);

  
  cairo_restore(cr);
  cr_check_status(cr);
  return cr_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//  Resize
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// SEXP cr_resize_(SEXP cr_, SEXP width_, SEXP height_) {
//   return R_NilValue;
// }


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// DImensions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_dim_(SEXP cr_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  
  cairo_surface_t *src_surface = cairo_get_target(cr);
  int width  = cairo_image_surface_get_width (src_surface);
  int height = cairo_image_surface_get_height(src_surface);
  
  SEXP dim_ = PROTECT(Rf_allocVector(INTSXP, 2)); 
  INTEGER(dim_)[0] = height;
  INTEGER(dim_)[1] = width;
  
  UNPROTECT(1);
  return dim_;
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Set the color from a uint32_t packed RGBA color
//
// Note: cairo internlly uses pre-multiplied alpha color, but
//       `cairo_set_source_rgba()` wants color values which are
//       NOT premultiplied
// See: https://nigeltao.github.io/blog/2022/premultiplied-alpha.html
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void set_color(cairo_t *cr, uint32_t color) {
  cairo_set_source_rgba(
    cr, 
    ( color        & 0xff) / 255.0,  // R
    ((color >>  8) & 0xff) / 255.0,  // G
    ((color >> 16) & 0xff) / 255.0,  // B
    ((color >> 24) & 0xff) / 255.0   // A
  );
}



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Check the cairo status
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void cr_check_status(cairo_t *cr) {
  cairo_status_t status = cairo_status(cr);
  
  if (status == CAIRO_STATUS_SUCCESS) return;
  
  Rf_error("Cairo failure: %s", cairo_status_to_string(status));
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Check the status of a pattern object
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void cr_pattern_status(cairo_pattern_t *pat) {
  cairo_status_t status = cairo_pattern_status(pat);
  
  if (status == CAIRO_STATUS_SUCCESS) return;
  
  Rf_error("Cairo pattern failure: %s", cairo_status_to_string(status));
}







