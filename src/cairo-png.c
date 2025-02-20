
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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// cairo_t * cairo_create (cairo_surface_t *target)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_read_png_(SEXP filename_)  {
  
  int nprotect = 0;
  const char *filename = CHAR(STRING_ELT(filename_, 0));
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Create a surface 
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_surface_t *surface = cairo_image_surface_create_from_png(filename);
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
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // tidy and return
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  UNPROTECT(nprotect);
  return cr_;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Save a cairo raster to a PNG
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SEXP cr_write_png_(SEXP cr_, SEXP filename_) {
  
  cairo_t *cr = cairo_extptr_to_cairo_t(cr_);
  const char *filename = CHAR(STRING_ELT(filename_, 0));
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Fetch the surface 
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cairo_surface_t *surface = cairo_get_target(cr);
  cairo_status_t status = cairo_surface_status(surface);
  if (status != CAIRO_STATUS_SUCCESS) {
    Rf_error("Cairo surface failure: %s", cairo_status_to_string(status));
  }
  
  status = cairo_surface_write_to_png(surface, filename);
  if (status != CAIRO_STATUS_SUCCESS) {
    Rf_error("Cairo PNG failure: %s", cairo_status_to_string(status));
  }
  
  return cr_;
}







