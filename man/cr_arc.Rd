% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/cairo-draw.R
\name{cr_arc}
\alias{cr_arc}
\title{Draw arcs}
\usage{
cr_arc(
  cr,
  x,
  y,
  r,
  angle1,
  angle2,
  clockwise = TRUE,
  fill = "black",
  color = NA,
  params = NULL
)
}
\arguments{
\item{cr}{\code{cairo raster} object as created by \code{\link{cr_new}()}}

\item{x, y, r}{coordinates}

\item{angle1, angle2}{start/end angles of the arc}

\item{clockwise}{direction of arc from angle1 to angle2. Default: TRUE}

\item{fill}{fill color. Default: 'black'}

\item{color}{border color. Default: NA (no line)}

\item{params}{graphical parameters created with \code{\link{cpar}()}. Default: NULL
means to use the currently defined global parameters}
}
\value{
Invisibly return the original \code{cairo raster} object
}
\description{
Draw arcs
}
\examples{
set.seed(3)
img <- cr_new(400, 300)
cr_arc(img, sample(400, 3), sample(300, 3), r= c(10, 50, 100), 
       angle1 = 0, angle2 = 3 * pi/4, clockwise = TRUE,
        fill = rainbow(3), color = 'black')
plot(img, T)
}
\seealso{
Other draw functions: 
\code{\link{cr_circle}()},
\code{\link{cr_line}()},
\code{\link{cr_point}()},
\code{\link{cr_polygon}()},
\code{\link{cr_polyline}()},
\code{\link{cr_rect}()},
\code{\link{cr_spline}()}
}
\concept{draw functions}
