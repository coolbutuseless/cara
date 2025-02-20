

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Copy (blit) an image into another image with the given position, scaling,
#' rotation etc.
#' 
#' Single source can be blitted to multiple locations.
#' 
#' @inheritParams cr_rect
#' @param dst,src destination and source cairo raster objects
#' @param x,y destination coordinates at which to place the source image
#' @param hjust,vjust handle position on the source image. Default (0.5, 0.5) 
#'        means that the image handle is in the centre of the image
#' @param angle Rotation angle in radians. default: 0 (no rotation)
#' @param xscale,yscale Scale factor in x and y dimensions. Default: 1 (no scaling).
#'        If \code{yscale} is not specified it will be set to the same as
#'        \code{xscale}.
#' @param filter Set the filter to be used when rendering 
#'        Default: NULL (use 'good' filter).  Valid values: 'fast', 'good', 
#'        'best', 'nearest', 'bilinear'
#'        \describe{
#'          \item{\code{fast}}{high performance filter with quality similar to 'nearest'}
#'          \item{\code{good}}{reasonable performance filter with quality similar to 'bilinear'}
#'          \item{\code{best}}{highest quality available. Performance may not be suitable for interactive use}
#'          \item{\code{nearest}}{Nearest-neighbour filtering}
#'          \item{\code{bilinear}}{Linear interpolation in two dimensions}
#'        }
#' @return Original \code{dst} cairo raster is returned invisibly
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = "cara")
#' logo <- cr_read_png(logo_file)
#' canvas <- cr_new(1200, 800)
#' cr_blit(dst = canvas, src = logo, x = c(100, 400, 800), y = 400, 
#'         xscale = seq(0.1, 0.5, length.out = 3), filter = 'best')
#' plot(canvas)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_blit <- function(dst, src, x = 0, y = 0, hjust = 0.5, vjust = 0.5, angle = 0, xscale = 1, yscale = xscale, filter = NULL, params = NULL) {
  invisible(
    .Call(cr_blit_, dst, src, x, y, hjust, vjust, angle, xscale, yscale, filter, params)
  )
}

