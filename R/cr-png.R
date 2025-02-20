

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Read PNG as a cairo raster
#' 
#' @param filename PNG filename
#' @return cairo raster
#' @examples
#' filename <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
#' cr <- cr_read_png(filename)
#' plot(cr, T)  
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_read_png <- function(filename) {
  .Call(cr_read_png_, filename)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Write cairo raster as PNG
#' 
#' @param cr cairo raster
#' @param filename output filename
#' @return None
#' @examples
#' cr <- cr_new(800, 600, 'hotpink')
#' cr_write_png(cr, tempfile())
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_write_png <- function(cr, filename) {
  invisible(
    .Call(cr_write_png_, cr, filename)
  )
}

