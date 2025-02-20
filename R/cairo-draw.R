
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw points (single square pixel)
#' 
#' @inheritParams cr_fill
#' @param x,y location 
#' @param color point color. Default: 'black'
#' @param size Side length of square representing a point. Default: 1
#' @param params graphical parameters created with \code{\link{cpar}()}. Default: NULL
#'        means to use the currently defined global parameters
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' set.seed(3)
#' img <- cr_new(400, 300, 'grey20')
#' N <- 1000
#' cr_point(img, sample(400, N, TRUE), sample(300, N, TRUE), color = rainbow(N))
#' plot(img, T)
#' @family draw functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_point <- function(cr, x, y, color = 'black', size = 1, params = NULL) {
  invisible(
    .Call(cr_rect_, cr, x, y, w = size, h = size, fill = color, color = NA, hjust = 0, vjust = 0, params)
  )
}


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw rectangles
#' 
#' @inheritParams cr_fill
#' @param x,y location of rectangle
#' @param w,h width and height of rectangle
#' @param fill fill color. Default: 'black'
#' @param color border color. Default: NA (no line)
#' @param hjust,vjust justification of the rectangle relative to its \code{(x, y)}
#'        location. Default: (0, 0).  Use (0.5, 0.5) to center the rectangle 
#'        at the (x, y) coordinates
#' @param params graphical parameters created with \code{\link{cpar}()}. Default: NULL
#'        means to use the currently defined global parameters
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' set.seed(3)
#' img <- cr_new(400, 300)
#' cr_rect(img, sample(400, 3), sample(300, 3), w = c(10, 50, 100), h = 42, 
#'         fill = rainbow(3), color = 'black', params = cpar(line_width = 3))
#' plot(img, T)
#' @family draw functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_rect <- function(cr, x, y, w, h, fill = 'black', color = NA, 
                    hjust = 0, vjust = 0, params = NULL) {
  invisible(
    .Call(cr_rect_, cr, x, y, w, h, fill, color, hjust, vjust, params)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw lines 
#' 
#' @inheritParams cr_rect
#' @param x0,y0,x1,y1 coordinates of the endpoints of the lines
#' @param color line color. Default: 'black'
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' img <- cr_new(400, 300)
#' cr_params_set(img, params = cpar(line_dash = c(3, 7)))
#' cr_line(img, x0 = 0, y0 = c(0, 100, 200), x1 = 400, y1 = c(300, 200, 100), 
#'         color = rainbow(3), param = cpar(line_width = 5))
#' plot(img, T)
#' @family draw functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_line <- function(cr, x0, y0, x1, y1, color = 'black', params = NULL) {
  invisible(
    .Call(cr_line_, cr, x0, y0, x1, y1, color, params)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw cubic bezier splines
#' 
#' @inheritParams cr_line
#' @param x1,y1,x2,y2 the endpoints of the bezier
#' @param cx1,cy1,cx2,cy2 the coordinates of the control points
#' @examples
#' w <- 400
#' h <- 300
#' img <- cr_new(w, h)
#' N <- 3
#' colors <- rainbow(N)
#' x1 <- runif(N, 0, w); y1 <- runif(N, 0, h)
#' x2 <- runif(N, 0, w); y2 <- runif(N, 0, h)
#' cx1 <- runif(N, 0, w); cy1 <- runif(N, 0, h)
#' cx2 <- runif(N, 0, w); cy2 <- runif(N, 0, h)
#' 
#' cr_spline(img, x1, y1, x2, y2, cx1, cy1, cx2, cy2, color = colors)
#' plot(img, T)
#' @family draw functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_spline <- function(cr, x1, y1, x2, y2, cx1, cy1, cx2, cy2, color = 'black', params = NULL) {
  invisible(
    .Call(cr_spline_, cr, x1, y1, x2, y2, cx1, cy1, cx2, cy2, color, params)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw circles 
#' 
#' @inheritParams cr_rect
#' @param x,y,r coordinates
#' 
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' set.seed(3)
#' img <- cr_new(400, 300)
#' cr_circle(img, sample(400, 3), sample(300, 3), r= c(10, 50, 100), 
#'         fill = rainbow(3), color = 'black')
#' plot(img, T)
#' @family draw functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_circle <- function(cr, x, y, r, fill = 'black', color = NA, params = NULL) {
  invisible(
    .Call(cr_arc_, cr, x, y, r, 0, -2 * pi, TRUE, fill, color, params)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw arcs 
#' 
#' @inheritParams cr_circle
#' @param angle1,angle2 start/end angles of the arc
#' @param clockwise direction of arc from angle1 to angle2. Default: TRUE
#' 
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' set.seed(3)
#' img <- cr_new(400, 300)
#' cr_arc(img, sample(400, 3), sample(300, 3), r= c(10, 50, 100), 
#'        angle1 = 0, angle2 = 3 * pi/4, clockwise = TRUE,
#'         fill = rainbow(3), color = 'black')
#' plot(img, T)
#' @family draw functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_arc <- function(cr, x, y, r, angle1, angle2, clockwise = TRUE, fill = 'black', color = NA, params = NULL) {
  invisible(
    .Call(cr_arc_, cr, x, y, r, -angle1, -angle2, clockwise, fill, color, params)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw polylines
#' 
#' @inheritParams cr_line
#' @param x,y vectors of coordinates
#' 
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' set.seed(3)
#' w <- h <- 1000
#' cr <- cr_new(w, h, 'white')
#' x <- runif(4, 0, w)
#' y <- runif(4, 0, h)
#' cr_polyline(cr, x, y, params = cpar(line_width = 20, line_join = 'miter'))
#' plot(cr, T)
#' @family draw functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_polyline <- function(cr, x, y, color = 'black', params = NULL) {
  invisible(
    .Call(cr_polyline_, cr, x, y, color, params)
  )
}



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#' Draw polygons
#' 
#' @inheritParams cr_line
#' @param x,y vectors of coordinates
#' @param fill fill color
#' 
#' @return Invisibly return the original \code{cairo raster} object
#' @examples
#' w <- h <- 1000
#' cr <- cr_new(w, h, 'grey80')
#' set.seed(1)
#' x <- runif(5, 0, w)
#' y <- runif(5, 0, h)
#' cr_params_set(cr, params = cpar(line_dash = c(15, 20), line_width = 4))
#' cr_polygon(cr, x, y, fill = 'royalblue', color = 'black')
#' plot(cr, T)
#' @family draw functions
#' @export
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_polygon <- function(cr, x, y, fill = 'black', color = NA, params = NULL) {
  invisible(
    .Call(cr_polygon_, cr, x, y, fill, color, params)
  )
}












