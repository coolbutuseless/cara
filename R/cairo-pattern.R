

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Create a pattern specification using an image
#' 
#' Patterns can be used as the \code{fill} argument to any of the drawing 
#' operations.
#' 
#' @inheritParams cr_rect
#' @inheritParams cr_blit
#' @param extend How will the pattern be handled outside the patterns natural area?
#'        Default: "repeat".  Valid options: "none", "repeat", "reflect", or "pad"
#'        \describe{
#'          \item{\code{none}}{pixels outside of the source pattern are fully transparent }
#'          \item{\code{repeat}}{the pattern is tiled by repeating}
#'          \item{\code{reflect}}{the pattern is tiled by reflecting at the edges}
#'          \item{\code{pad}}{pixels outside of the pattern copy the closest pixel from the source}
#'        }
#' @return pattern object
#' @examples
#' logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
#' logo   <- cr_read_png(logo_file)
#' 
#' canvas <- cr_new(1024, 726, fill = 'royalblue')
#' 
#' 
#' pat <- create_pattern_image(logo, extend = "repeat") |>
#'   cr_pattern_scale(0.2) |>
#'   cr_pattern_rotate(0.1 * pi)
#' 
#' cr_rect(canvas, 20, 20, 400, 400, fill = pat, color = 'black')
#' 
#' plot(canvas, T)
#' @family pattern creation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
create_pattern_image <- function(cr, extend = 'repeat', filter = NULL) {
  .Call(create_pattern_image_, cr, extend, filter)
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Create a pattern specification for a radial gradient
#' 
#' @inheritParams create_pattern_image
#' @param colors,stops color stops
#' @param x1,y1,r1,x2,y2,r2 radial spec
#' @return pattern object
#' @examples
#' canvas <- cr_new(1024, 726, fill = 'royalblue')
#' 
#' pat <- create_pattern_radial_gradient(rainbow(10), seq(0, 1, length.out = 10), 
#'         x1 = 0, y1 = 0, r1 = 10, x2 = 1024, y2 = 726, r2 = 1000) 
#' 
#' cr_rect(canvas, 20, 20, 1000, 400, fill = pat)
#' 
#' plot(canvas, T)
#' @family pattern creation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
create_pattern_radial_gradient <- function(colors, stops, x1, y1, r1, x2, y2, r2, extend = 'repeat', filter = NULL) {
  .Call(
    create_pattern_radial_gradient_,
    colors, stops,
    x1, y1, r1,
    x2, y2, r2,
    extend, filter
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Create a pattern specification for a radial gradient
#' 
#' @inheritParams cr_rect
#' @inheritParams create_pattern_image
#' @param colors,stops color stops
#' @param x1,y1,x2,y2 endpoints
#' @return pattern object
#' @examples
#' canvas <- cr_new(1024, 726, fill = 'royalblue')
#' 
#' pat <- create_pattern_linear_gradient(rainbow(10), seq(0, 1, length.out = 10), 
#'         x1 = 0, y1 = 0, x2 = 200, y2 = 726) 
#' 
#' cr_rect(canvas, 20, 20, 1000, 400, fill = pat)
#' 
#' plot(canvas, T)
#' @family pattern creation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
create_pattern_linear_gradient <- function(colors, stops, x1, y1, x2, y2, extend = 'repeat', 
                                           filter = NULL) {
  .Call(
    create_pattern_linear_gradient_,
    colors, stops,
    x1, y1, 
    x2, y2, 
    extend, filter
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Translate a pattern
#' 
#' @param pat pattern object created by \code{create_pattern_image()} etc
#' @param x,y translation offsets
#' @return Invisibly return the original \code{cairo pattern} object
#' @examplesIf interactive()
#' cr_pattern_translate(pat, 2, 2)
#' @family pattern transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_pattern_translate <- function(pat, x, y) {
  invisible(
    .Call(cr_pattern_translate_, pat, x, y)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Scale
#' 
#' @inheritParams cr_pattern_translate
#' @param x,y scale factors
#' @return Invisibly return the original \code{cairo pattern} object
#' @examplesIf interactive()
#' cr_pattern_scale(pat, 2, 2)
#' @family pattern transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_pattern_scale <- function(pat, x, y = x) {
  invisible(
    .Call(cr_pattern_scale_, pat, x, y)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Rotate
#' 
#' @inheritParams cr_pattern_translate
#' @param angle rotation angle in radians
#' @return Invisibly return the original \code{cairo pattern} object
#' @examplesIf interactive()
#' cr_pattern_rotate(pat, pi)
#' @family pattern transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_pattern_rotate <- function(pat, angle) {
  invisible(
    .Call(cr_pattern_rotate_, pat, angle)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Reset transformation
#' 
#' @inheritParams cr_pattern_translate
#' @return Invisibly return the original \code{cairo pattern} object
#' @examplesIf interactive()
#' cr_pattern_transform_reset(pat)
#' @family pattern transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_pattern_transform_reset <- function(pat) {
  invisible(
    .Call(cr_pattern_transform_reset_, pat)
  )
}

