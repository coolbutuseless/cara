

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Start/end region of masked drawing
#' 
#' These functions are used to globally set a mask.  Within the bounds of 
#' these two functions, drawing will only occur where the pixels of the
#' mask are opaque.
#' 
#' @inheritParams cr_rect
#' @param mask Image (cairo raster) to use as mask.  This image does not need 
#'        to be the same size as \code{cr}
#' @param x,y position of mask within the destinatino. Default: (0, 0) i.e. 
#'        start at the origin (top-left)
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
#' logo <- cr_read_png(logo_file)
#' plot(logo, T)
#' img <- cr_new(600, 400)
#' 
#' cr_params_set(img, cpar(line_width = 10))
#' 
#' cr_mask_begin(img, mask = logo)
#' cr_circle(img, 300, 200, 200, fill = 'blue', color = 'black')
#' cr_circle(img, 350, 200, 100, fill = 'darkgreen', color = 'black')
#' cr_mask_end(img)
#' 
#' plot(img, T)
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_mask_begin <- function(cr, mask, x = 0, y = 0) {
  invisible(
    .Call(cr_mask_begin_, cr, mask, x, y)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' @rdname cr_mask_begin
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_mask_end <- function(cr) {
  invisible(
    .Call(cr_mask_end_, cr)
  )
}

