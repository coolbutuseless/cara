void assert_nativeraster(SEXP nr_);

int calc_max_length(int count, ...);
double *as_dbl_vec(SEXP vec_, int N, bool *do_free);

void premultiply_and_swizzle(uint8_t *pixel, int width, int height);
void  demultiply_and_swizzle(uint8_t *pixel, int width, int height);
