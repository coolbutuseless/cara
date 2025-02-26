% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/cairo-core.R
\name{cr_new}
\alias{cr_new}
\title{Create a new \code{\link{cairo raster}}}
\usage{
cr_new(width, height, fill = "white")
}
\arguments{
\item{width, height}{dimensions in pixels}

\item{fill}{background color}
}
\value{
A \code{cairo raster} object. This is an external pointer to 
        a \code{cairo_t} pointer in C
}
\description{
Create a new \code{\link{cairo raster}}
}
\examples{
img <- cr_new(800, 600, 'hotpink')
plot(img)

}
