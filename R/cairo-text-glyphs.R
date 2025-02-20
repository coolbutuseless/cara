
# nocov start

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw glyphs 
#' 
#' This is a low level function which needs some other package to determine 
#' actual glyph IDs and drawing locations. One example of a package which 
#' does this layout is \code{textshaping}.
#' 
#' @inheritParams cr_text_basic
#' @param idx glyph indices in specified font
#' @return Invisibly return the original \code{cairo raster} object
#' @examplesIf interactive() && requireNamespace('textshaping')
#' # cairo_select_font_face() within Cairo using 'fontconfig' lib may 
#' # not be releasing memory automatically through the glyph interface.
#' ts <- textshaping::shape_text(
#'     "hello", family = "sans-serif", id = 1, size = 80, weight = 400
#' )
#' glyphs <- ts$shape 
#' img <- cr_new(200, 200)
#' cr_text_glyph(
#'    cr     = img, 
#'    idx    = glyphs$index, 
#'    x      = glyphs$x_offset + 10, 
#'    y      = glyphs$y_offset + 100, 
#'    family = 'sans-serif', 
#'    size   = 80, 
#'    weight = 'bold', 
#'    slant  = 'oblique'
#' )
#' plot(img, T)
#' @family text functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_text_glyph <- function(cr, idx, x, y, 
                          family = 'sans-serif', size = 100, 
                          fill = 'black', color = NA, 
                          weight = "normal", slant = "normal") {
  invisible(
    .Call(
      cr_text_glyph_, 
      cr, idx, x, y, 
      family, size, 
      fill, color,
      weight, slant
    )
  )
}

# nocov end
