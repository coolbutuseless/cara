

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Create a new \code{\link{cairo raster}}
#' 
#' @param width,height dimensions in pixels
#' @param fill background color
#'
#' @return A \code{cairo raster} object. This is an external pointer to 
#'         a \code{cairo_t} pointer in C
#' @examples
#' img <- cr_new(800, 600, 'hotpink')
#' plot(img)
#' 
#' @import colorfast
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_new <- function(width, height, fill = 'white') {
  .Call(cr_new_, width, height, fill)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Plot a \code{cairo raster} 
#' 
#' @param x \code{cairo raster}
#' @param y any argument here will cause \code{grid::grid.newpage()} to 
#'        be called prior to drawing the \code{cairo raster} 
#' @param ... other arguments passed to \code{grid::grid.raster()}
#'
#' @return None.
#' 
#' @examples
#' cr <- cr_new(200, 100, 'hotpink')
#' plot(cr)
#' 
#' @import grid
#' @importFrom grDevices dev.flush dev.hold
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
plot.cairo_t <- function(x, y, ...) {
  if (!missing(y)) grid::grid.newpage()
  nr <- cr_to_nr(x)
  grDevices::dev.hold()
  grid::grid.raster(nr, interpolate = FALSE, ...)
  grDevices::dev.flush()
  invisible(x)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Fill an image with a color or pattern
#' 
#' @param cr \code{cairo raster} object as created by \code{\link{cr_new}()}
#' @param fill color (as hex, colorname or integer) or pattern object 
#'        returned by \code{create_pattern_*} family of functions
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' img <- cr_new(800, 600, 'hotpink')
#' plot(img, T)
#' 
#' cr_fill(img, '#0000ff')
#' plot(img, T)
#' 
#' logo_file <- system.file("img/logo-rainbow.png", package = "cara")
#' logo <- cr_read_png(logo_file)
#' logo <- cr_resize(logo, 150, 100)
#' pat <- create_pattern_image(logo)
#' cr_fill(img, pat)
#' plot(img, T)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_fill <- function(cr, fill) {
  invisible(
    .Call(cr_fill_, cr, fill)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Create a new \code{cairo raster} image with the contents from an existing image
#'
#' @inheritParams cr_fill
#' @return A new \code{cairo raster} object containing a copy of the image from the
#'         original
#' 
#' @examples
#' cr1 <- cr_new(200, 200, 'blue')
#' cr2 <- cr_duplicate(cr1)
#' plot(cr2, T)
#' 
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_duplicate <- function(cr) {
  .Call(cr_duplicate_, cr)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Copy the contents of one \code{cairo raster} into a pre-existing raster
#' 
#' The source and destination rasters must have the same dimensions
#' 
#' @param src,dst Source and destination \code{cairo raster} images. These
#'        image must be the same size.
#' @return Invisibly return the original \code{dst} \code{cairo raster} object
#' @examples
#' cr1 <- cr_new(200, 200, 'blue')
#' cr2 <- cr_duplicate(cr1)
#' cr_fill(cr1, 'green')
#' cr_copy_into(cr2, cr1)
#' plot(cr2, T)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_copy_into <- function(dst, src) {
  invisible(
    .Call(cr_copy_into_, dst, src)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Resize current image into a new image
#' 
#' @inheritParams cr_rect
#' @param width,height new dimensions
#' @return New cairo raster object
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = "cara")
#' logo <- cr_read_png(logo_file)
#' plot(logo, T)
#' cr_dim(logo)
#' tiny_logo <- cr_resize(logo, 200, 50)
#' cr_dim(tiny_logo)
#' plot(tiny_logo, T)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_resize <- function(cr, width, height) {
  
  current_dim <- cr_dim(cr)
  current_height <- current_dim[1]
  current_width  <- current_dim[2]
  
  if (current_height == 0 || current_width == 0) stop("Cannot resize")
  
  xscale <- width/current_width
  yscale <- height/current_height
  
  result <- cr_new(width, height, 0L)
  cr_blit(result, cr, hjust = 0, vjust = 0, xscale = xscale, yscale = yscale)
  
  result
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Dimensions of \code{cairo raster}
#' 
#' @inheritParams cr_rect
#' @return integer vector of dimensions. i.e. height, width
#' @examples
#' img <- cr_new(120, 50)
#' cr_dim(img)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_dim <- function(cr) {
  .Call(cr_dim_, cr)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Print method
#' 
#' @param x cairo_t object
#' @param ... ignored
#' @return None
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = "cara")
#' logo <- cr_read_png(logo_file)
#' print(logo)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
print.cairo_t <- function(x, ...) {
  d <- cr_dim(x);
  cat(sprintf("<cairo_t> raster object: Width = %i, Height = %i\n", d[2], d[2]))
}

