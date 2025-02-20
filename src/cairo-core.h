
void cairo_t_finalizer(SEXP cr_);
SEXP cr_new(int width, int height);
void set_color(cairo_t *cr, uint32_t color);
cairo_t *cairo_extptr_to_cairo_t(SEXP cr_);
void cr_check_status(cairo_t *cr);
void cr_pattern_status(cairo_pattern_t *pat);


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Meta information stored in the 'tag' slot of the ExternalPointer
//
// Currently this is just information for the masking
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define META_LENGTH 4
#define META_MASK   0  // Cairo raster of the mask. If R_NilValue, then there is no mask
#define META_MASK_X 1  // (x, y) Position of mask
#define META_MASK_Y 2  //
#define META_CACHE  3  // Cached copy of the primary 'cr' surface prior to starting masked operations
