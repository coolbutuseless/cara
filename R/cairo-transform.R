

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Translate
#' 
#' @inheritParams cr_rect
#' @param x,y translation offsets
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' canvas <- cr_new(800, 800, fill = 'royalblue')
#' cr_translate(canvas, 400, 400)
#' 
#' # Draw a square at the origin of this transformed envionrment
#' cr_rect(canvas, x = 0, y = 0, w = 100, h = 100, hjust = 0.5, vjust = 0.5)
#' 
#' plot(canvas, T)
#' @family drawing transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_translate <- function(cr, x, y) {
  invisible(
    .Call(cr_translate_, cr, x, y)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Scale
#' 
#' @inheritParams cr_rect
#' @param x,y scale factors
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' canvas <- cr_new(800, 800, fill = 'royalblue')
#' cr_translate(canvas, 400, 400)
#' cr_scale(canvas, x = 1, y = 2)
#' 
#' # Draw a square in the transformed environment
#' cr_rect(canvas, x = 0, y = 0, w = 100, h = 100, hjust = 0.5, vjust = 0.5)
#' 
#' plot(canvas, T)
#' @family drawing transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_scale <- function(cr, x, y = x) {
  invisible(
    .Call(cr_scale_, cr, x, y)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Rotate
#' 
#' @inheritParams cr_rect
#' @param angle rotation angle in radians
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' canvas <- cr_new(800, 800, fill = 'royalblue')
#' cr_translate(canvas, 400, 400)
#' cr_rotate(canvas, pi/8)
#' cr_rect(canvas, x = 0, y = 0, w = 100, h = 100, hjust = 0.5, vjust = 0.5)
#' plot(canvas, T)
#' @family drawing transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_rotate <- function(cr, angle) {
  invisible(
    .Call(cr_rotate_, cr, angle)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Reset transformation
#' 
#' @inheritParams cr_rect
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' canvas <- cr_new(800, 800, fill = 'royalblue')
#' cr_translate(canvas, 400, 400)
#' cr_rotate(canvas, pi/8)
#' cr_transform_reset(canvas)
#' 
#' cr_rect(canvas, x = 0, y = 0, w = 100, h = 100, hjust = 0.5, vjust = 0.5)
#' plot(canvas, T)
#' @family drawing transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_transform_reset <- function(cr) {
  invisible(
    .Call(cr_transform_reset_, cr)
  )
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Set transformation matrix
#' 
#' Clear the current transformation matrix and set it to these values.
#' 
#' \code{x_new = xx * x + xy * y + x0;}
#' \code{y_new = yx * x + yy * y + y0;}
#' 
#' @inheritParams cr_rect
#' @param xx,yx,xy,yy,x0,y0 params
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' canvas <- cr_new(800, 800, fill = 'royalblue')
#' 
#' # Shear transformation
#' cr_transform_set(canvas, xy = 0.3)
#' 
#' cr_rect(canvas, x = 400, y = 400, w = 100, h = 100, hjust = 0.5, vjust = 0.5)
#' plot(canvas, T)
#' @family drawing transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_transform_set <- function(cr, xx = 1, yx = 0, xy = 0, yy = 1, x0 = 0, y0 = 0) {
  invisible(
    .Call(cr_transform_set_, cr, xx, yx, xy, yy, x0, y0)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Apply transformation matrix
#' 
#' Multiply the current transformation matrix by these values
#' 
#' \code{x_new = xx * x + xy * y + x0;}
#' \code{y_new = yx * x + yy * y + y0;}
#' 
#' @inheritParams cr_transform_set
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' canvas <- cr_new(800, 800, fill = 'royalblue')
#' 
#' # Shear transformation
#' cr_scale(canvas, 2)
#' cr_transform(canvas, xy = 0.3)
#' 
#' cr_rect(canvas, x = 100, y = 100, w = 100, h = 100, hjust = 0.5, vjust = 0.5)
#' plot(canvas, T)
#' @family drawing transformation functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_transform <- function(cr, xx = 1, yx = 0, xy = 0, yy = 1, x0 = 0, y0 = 0) {
  invisible(
    .Call(cr_transform_, cr, xx, yx, xy, yy, x0, y0)
  )
}

