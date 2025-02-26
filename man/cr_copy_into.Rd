% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/cairo-core.R
\name{cr_copy_into}
\alias{cr_copy_into}
\title{Copy the contents of one \code{cairo raster} into a pre-existing raster}
\usage{
cr_copy_into(dst, src)
}
\arguments{
\item{src, dst}{Source and destination \code{cairo raster} images. These
image must be the same size.}
}
\value{
Invisibly return the original \code{dst} \code{cairo raster} object
}
\description{
The source and destination rasters must have the same dimensions
}
\examples{
cr1 <- cr_new(200, 200, 'blue')
cr2 <- cr_duplicate(cr1)
cr_fill(cr1, 'green')
cr_copy_into(cr2, cr1)
plot(cr2, T)
}
