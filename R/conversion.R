

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert \code{cairo raster} to \code{native raster}
#' 
#' @param cr \code{cairo raster}
#' @param nr Optional native raster to copy the cairo raster into. Must be same size.
#'        Default: NULL means to create a new native raster object
#' @return native raster
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
#' logo <- cr_read_png(logo_file)
#' nr <- cr_to_nr(logo)
#' class(nr)
#' logo2 <- nr_to_cr(nr)
#' plot(logo2, T)
#' @family image conversion functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_to_nr <- function(cr, nr = NULL) {
  .Call(cr_to_nr_, cr, nr)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert \code{native raster} to \code{cairo raster}
#' 
#' @param nr \code{native raster}
#' @param cr Optional cairo raster to copy the native raster into.  Must be same size.
#'        Default: NULL means to create a new cairo raster object
#' @return cairo raster
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
#' logo <- cr_read_png(logo_file)
#' nr <- cr_to_nr(logo)
#' class(nr)
#' logo2 <- nr_to_cr(nr)
#' plot(logo2, T)
#' @family image conversion functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
nr_to_cr <- function(nr, cr = NULL) {
  .Call(nr_to_cr_, nr, cr)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert standard R raster to cairo raster
#' 
#' @param ras R raster
#' @param cr optional cairo raster as a preallocation destination. If NULL (default), 
#'        then a new cairo raster will be created
#' @return cairo raster
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
#' logo <- cr_read_png(logo_file)
#' ras <- cr_to_raster(logo)
#' class(ras)
#' logo2 <- raster_to_cr(ras)
#' plot(logo2, T)
#' @family image conversion functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
raster_to_cr <- function(ras, cr = NULL) {
  res <- .Call(raster_to_cr_, ras, cr)
  if (is.null(cr)) {
    res
  } else {
    invisible(res)
  }
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert cairo raster to standard R raster
#' @inheritParams cr_rect
#' @return R raster
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
#' logo <- cr_read_png(logo_file)
#' ras <- cr_to_raster(logo)
#' class(ras)
#' logo2 <- raster_to_cr(ras)
#' plot(logo2, T)
#' @family image conversion functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_to_raster <- function(cr) {
  .Call(cr_to_raster_, cr)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert numeric array to cairo raster
#' 
#' @param arr Numeric R array
#' @param cr optional cairo raster as a preallocation destination. If NULL (default), 
#'        then a new cairo raster will be created
#' @return cairo raster
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
#' logo <- cr_read_png(logo_file)
#' arr <- cr_to_array(logo)
#' class(arr)
#' plot(as.raster(arr))
#' logo2 <- array_to_cr(arr)
#' plot(logo2, T)
#' @family image conversion functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
array_to_cr <- function(arr, cr = NULL) {
  res <- .Call(array_to_cr_, arr, cr)
  if (is.null(cr)) {
    res
  } else {
    invisible(res)
  }
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert cairo raster to numeric array
#' 
#' @inheritParams cr_rect
#' @return R array
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
#' logo <- cr_read_png(logo_file)
#' arr <- cr_to_array(logo)
#' class(arr)
#' plot(as.raster(arr))
#' logo2 <- array_to_cr(arr)
#' plot(logo2, T)
#' @family image conversion functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_to_array <- function(cr) {
  .Call(cr_to_array_, cr)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Convert a 'magick' image to \code{cairo raster}
#' 
#' @inheritParams array_to_cr
#' @param im image from the \code{magick} package
#' 
#' @return \code{cairo raster}
#' 
#' @examplesIf interactive() && requireNamespace('magick', quietly = TRUE)
#' im <- magick::logo
#' cr <- magick_to_cr(im)
#' plot(cr)
#' @family image conversion functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
magick_to_cr <- function(im, cr = NULL) {
  if (requireNamespace('magick', quietly = TRUE)) {
    im <- magick::image_convert(im, format = 'rgba', matte = TRUE)
    if (length(im) == 1) {
      .Call(magick_to_cr_, magick::image_data(im), cr)
    } else {
      stop("magick 'im' contains multiple images. Use lapply() or map().")
    }
  } else {
    stop("Please install the 'magick' package in order to use this function");
  }
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' @rdname magick_to_cr
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_to_magick <- function(cr) {
  if (requireNamespace('magick', quietly = TRUE)) {
    magick::image_read(cr_to_nr(cr))  # Magick can read native rasters
  } else {
    stop("Please install the 'magick' package in order to use this function");
  }
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Matrix to \code{cairo raster}
#' 
#' @param mat integer matrix
#' @param palette vector of colors.  For an integer matrix, this palette must 
#'        contain at least as many colors as the maximum integer value in \code{mat}. 
#'        For a numeric matrix, any length palette is allowed.
#' @param cr destination \code{cairo raster} object. If NULL (the default) a 
#'        new \code{nativeRaster} will be created  If a \code{nativeRaster} 
#'        is supplied here, it must have the exact dimensions to match the matrix        
#' @param fill Color to be used for values < 1 when input is an integer matrix.  
#'        Default: 'transparent'.
#' @param min,max assumed range for the numeric data.  values from the palette
#'        will be interpolated using this range as the extents.  An error
#'        will occur if a value lies outside this range. Default: (0, 1)
#' @return \code{nativeRaster}
#'
#' @examples
#' # Numeric matrix. 
#' m <- matrix(seq(0, 1, length.out = 30 * 40), 30, 40)
#' palette <- terrain.colors(100)
#' cr <- matrix_to_cr(m, palette)
#' plot(cr, T)
#'  
#' # Integer matrix. Values in matrix will used to lookup palette by index
#' m <- matrix(1:12, 3, 4)
#' palette <- rainbow(12)
#' cr <- matrix_to_cr(m, palette) 
#' plot(cr, T)
#' 
#' @family image conversion functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
matrix_to_cr <- function(mat, palette, fill = 'transparent', min = 0.0, max = 1.0, cr = NULL) {
  .Call(matrix_to_cr_, mat, palette, fill, min, max, cr)
}


