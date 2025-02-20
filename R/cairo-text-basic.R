
# nocov start

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw text in a simple manner
#' 
#' This function displays text but does nothing in terms of font layout e.g.
#' it doesn't do any kerning.  This will look OK for monospaced fonts, but 
#' will not be great for most other fonts.  
#' 
#' @inheritParams cr_rect
#' @param x,y text location
#' @param text text to render
#' @param fill,color text color
#' @param size font size in pixels. default: 100
#' @param family font family name. Default: "sans-serif".
#'        Use \code{systemfonts::system_fonts()$family} 
#'        to see fonts available on the current system.  The following 
#'        generic names are also guaranteed to work: "serif", "sans-serif", 
#'        "cursive", "fantasy", "monospace"
#' @param weight "normal" (default) or "bold"
#' @param slant "normal" (default), "italic" or "oblique"
#' @param hjust,vjust justification of the text relative to the (x, y) 
#'        location. Default: (0, 0). Use (0.5, 0.5) to center the text 
#'        at the given (x, y) coordinates
#' @return Invisibly return the original \code{cairo raster} object
#' @examplesIf interactive()
#' # cairo_select_font_face() within Cairo using 'fontconfig' lib may 
#' # not be releasing memory automatically through the glyph interface.
#' img <- cr_new(200, 100, 'grey80')
#' cr_text_basic(img, "Hello", 10, 95, size = 80, family = 'sans-serif')
#' plot(img, T)
#' @family text functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_text_basic <- function(cr, text, x, y, 
                          family = "sans-serif", size = 100, 
                          fill = 'black', color = NA, 
                          weight = "normal", slant = "normal", 
                          hjust = 0, vjust = 0) {
  invisible(
    .Call(
      cr_text_basic_, cr, text, x, y, 
      family, size,
      fill, color, 
      weight, slant,
      hjust, vjust
    )
  )
}

# nocov end
