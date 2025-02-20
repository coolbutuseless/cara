
// #define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Core
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_new_(SEXP width_, SEXP height_, SEXP fill_);
extern SEXP cr_duplicate_(SEXP cr_);
extern SEXP cr_copy_into_(SEXP dst_, SEXP src_);
// extern SEXP cr_resize_(SEXP cr_, SEXP width_, SEXP height_);
extern SEXP cr_dim_(SEXP cr_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Conversion
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_to_nr_(SEXP cr_, SEXP nr_);
extern SEXP nr_to_cr_(SEXP nr_, SEXP cr_);

extern SEXP raster_to_cr_(SEXP ras_, SEXP cr_);
extern SEXP cr_to_raster_(SEXP cr_);

extern SEXP array_to_cr_(SEXP arr_, SEXP cr_);
extern SEXP cr_to_array_(SEXP cr_);

extern SEXP magick_to_cr_(SEXP im_data_, SEXP cr_);
extern SEXP matrix_to_cr_(SEXP mat_, SEXP palette_, SEXP fill_, SEXP min_, SEXP max_, SEXP cr_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// PNG
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_read_png_(SEXP filename_);
extern SEXP cr_write_png_(SEXP cr_, SEXP filename_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Masking
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_mask_begin_(SEXP cr_, SEXP mask_, SEXP x_, SEXP y_);
extern SEXP cr_mask_end_ (SEXP cr_);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_fill_(SEXP cr_, SEXP fill_);
extern SEXP cr_rect_(SEXP cr_, SEXP x_, SEXP y_, SEXP w_, SEXP h_, SEXP fill_, SEXP color_, SEXP hjust_, SEXP vjust_, SEXP params_);
extern SEXP cr_line_(SEXP cr_, SEXP x0_, SEXP y0_, SEXP x1_, SEXP y1_, SEXP color_, SEXP params_);
extern SEXP cr_arc_(SEXP cr_, SEXP x_, SEXP y_, SEXP r_, SEXP angle1_, SEXP angle2_, SEXP clockwise_, SEXP fill_, SEXP color_, SEXP params_);
extern SEXP cr_polyline_(SEXP cr_, SEXP x_, SEXP y_, SEXP color_, SEXP params_);
extern SEXP cr_polygon_(SEXP cr_, SEXP x_, SEXP y_, SEXP fill_, SEXP color_, SEXP params_);
extern SEXP cr_spline_(SEXP cr_, SEXP x1_, SEXP y1_, SEXP x2_, SEXP y2_, SEXP cx1_, SEXP cy1_, SEXP cx2_, SEXP cy2_, SEXP color_, SEXP params_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Global parameter setting
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_params_set_    (SEXP cr_, SEXP params_);
extern SEXP cr_params_save_   (SEXP cr_);
extern SEXP cr_params_restore_(SEXP cr_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Transforms
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_translate_(SEXP cr_, SEXP tx_, SEXP ty_);
extern SEXP cr_scale_    (SEXP cr_, SEXP sx_, SEXP sy_);
extern SEXP cr_rotate_   (SEXP cr_, SEXP angle_);
extern SEXP cr_transform_reset_(SEXP cr_);
extern SEXP cr_transform_set_(SEXP cr_, SEXP xx_, SEXP yx_, SEXP xy_, SEXP yy_, SEXP x0_, SEXP y0_);
extern SEXP cr_transform_    (SEXP cr_, SEXP xx_, SEXP yx_, SEXP xy_, SEXP yy_, SEXP x0_, SEXP y0_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Text
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_text_basic_(SEXP cr_, SEXP text_, 
                           SEXP x_, SEXP y_, 
                           SEXP family_, SEXP size_,
                           SEXP fill_, SEXP color_, 
                           SEXP weight_, SEXP slant_, 
                           SEXP hjust_, SEXP vjust_);

extern SEXP cr_text_glyph_(SEXP cr_, SEXP idx_, 
                           SEXP x_, SEXP y_, 
                           SEXP family_, SEXP size_,
                           SEXP fill_, SEXP color_, 
                           SEXP weight_, SEXP slant_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Blit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_blit_(SEXP dst_, SEXP src_, SEXP x_, SEXP y_, 
                     SEXP hjust_, SEXP vjust_, 
                     SEXP angle_, 
                     SEXP xscale_, SEXP yscale_, 
                     SEXP filter_,
                     SEXP params_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Patterns
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP create_pattern_image_(SEXP cr_, SEXP mode_, SEXP filter_);
extern SEXP create_pattern_linear_gradient_(SEXP colors_, SEXP stops_, 
                                            SEXP x1_, SEXP y1_, SEXP x2_, SEXP y2_,
                                            SEXP mode_, SEXP filter_);
extern SEXP create_pattern_radial_gradient_(SEXP colors_, SEXP stops_, 
                                            SEXP x1_, SEXP y1_, SEXP r1_, 
                                            SEXP x2_, SEXP y2_, SEXP r2, 
                                            SEXP mode_, SEXP filter_);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Pattern transforms
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern SEXP cr_pattern_transform_reset_(SEXP pat_);
extern SEXP cr_pattern_translate_(SEXP pat_, SEXP x_, SEXP y_);
extern SEXP cr_pattern_scale_(SEXP pat_, SEXP x_, SEXP y_);
extern SEXP cr_pattern_rotate_(SEXP pat_, SEXP angle_);



static const R_CallMethodDef CEntries[] = {
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Core
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_new_"      , (DL_FUNC) &cr_new_      , 3},
  {"cr_duplicate_", (DL_FUNC) &cr_duplicate_, 1},
  {"cr_copy_into_", (DL_FUNC) &cr_copy_into_, 2},
  {"cr_dim_"      , (DL_FUNC) &cr_dim_      , 1},
  {"cr_fill_"     , (DL_FUNC) &cr_fill_     , 2},
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Conversion
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_to_nr_"    , (DL_FUNC) &cr_to_nr_    , 2},
  {"nr_to_cr_"    , (DL_FUNC) &nr_to_cr_    , 2},
  
  {"cr_to_raster_", (DL_FUNC) &cr_to_raster_, 1},
  {"raster_to_cr_", (DL_FUNC) &raster_to_cr_, 2},
  
  {"cr_to_array_", (DL_FUNC) &cr_to_array_, 1},
  {"array_to_cr_", (DL_FUNC) &array_to_cr_, 2},
  
  {"magick_to_cr_", (DL_FUNC) &magick_to_cr_, 2},
  {"matrix_to_cr_", (DL_FUNC) &matrix_to_cr_, 6},
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // PNG
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_read_png_" , (DL_FUNC) &cr_read_png_ , 1},
  {"cr_write_png_", (DL_FUNC) &cr_write_png_, 2},
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Masking
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_mask_begin_", (DL_FUNC) &cr_mask_begin_, 4},
  {"cr_mask_end_"  , (DL_FUNC) &cr_mask_end_   , 1},
  
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Draw
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_rect_"    , (DL_FUNC) &cr_rect_    , 10},
  {"cr_line_"    , (DL_FUNC) &cr_line_    ,  7},
  {"cr_arc_"     , (DL_FUNC) &cr_arc_     , 10},
  {"cr_polyline_", (DL_FUNC) &cr_polyline_,  5},
  {"cr_polygon_" , (DL_FUNC) &cr_polygon_ ,  6},
  {"cr_spline_"  , (DL_FUNC) &cr_spline_  , 11},
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Global params
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_params_set_"    , (DL_FUNC) &cr_params_set_    , 2},
  {"cr_params_save_"   , (DL_FUNC) &cr_params_save_   , 1},
  {"cr_params_restore_", (DL_FUNC) &cr_params_restore_, 1},
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Transforms
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_translate_", (DL_FUNC) &cr_translate_, 3},
  {"cr_scale_", (DL_FUNC) &cr_scale_, 3},
  {"cr_rotate_", (DL_FUNC) &cr_rotate_, 2},
  {"cr_transform_reset_", (DL_FUNC) &cr_transform_reset_, 1},
  {"cr_transform_set_"  , (DL_FUNC) &cr_transform_set_  , 7},
  {"cr_transform_"      , (DL_FUNC) &cr_transform_      , 7},
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Text and Blit
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_text_basic_", (DL_FUNC) &cr_text_basic_, 12},
  {"cr_text_glyph_", (DL_FUNC) &cr_text_glyph_, 10},
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Blit
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_blit_", (DL_FUNC) &cr_blit_, 11},
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Patterns
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"create_pattern_image_", (DL_FUNC) &create_pattern_image_, 3},
  {"create_pattern_linear_gradient_", (DL_FUNC) &create_pattern_linear_gradient_,  8},
  {"create_pattern_radial_gradient_", (DL_FUNC) &create_pattern_radial_gradient_, 10},
  
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Pattern transforms
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  {"cr_pattern_transform_reset_", (DL_FUNC) &cr_pattern_transform_reset_, 1},
  {"cr_pattern_translate_", (DL_FUNC) &cr_pattern_translate_, 3},
  {"cr_pattern_scale_", (DL_FUNC) &cr_pattern_scale_, 3},
  {"cr_pattern_rotate_", (DL_FUNC) &cr_pattern_rotate_, 2},
  
  
  {NULL , NULL, 0}
};


void R_init_cara(DllInfo *info) {
  R_registerRoutines(
    info,      // DllInfo
    NULL,      // .C
    CEntries,  // .Call
    NULL,      // Fortran
    NULL       // External
  );
  R_useDynamicSymbols(info, FALSE);
}



