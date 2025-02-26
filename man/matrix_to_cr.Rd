% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/conversion.R
\name{matrix_to_cr}
\alias{matrix_to_cr}
\title{Matrix to \code{cairo raster}}
\usage{
matrix_to_cr(mat, palette, fill = "transparent", min = 0, max = 1, cr = NULL)
}
\arguments{
\item{mat}{integer matrix}

\item{palette}{vector of colors.  For an integer matrix, this palette must 
contain at least as many colors as the maximum integer value in \code{mat}. 
For a numeric matrix, any length palette is allowed.}

\item{fill}{Color to be used for values < 1 when input is an integer matrix.
Default: 'transparent'.}

\item{min, max}{assumed range for the numeric data.  values from the palette
will be interpolated using this range as the extents.  An error
will occur if a value lies outside this range. Default: (0, 1)}

\item{cr}{destination \code{cairo raster} object. If NULL (the default) a 
new \code{nativeRaster} will be created  If a \code{nativeRaster} 
is supplied here, it must have the exact dimensions to match the matrix}
}
\value{
\code{nativeRaster}
}
\description{
Matrix to \code{cairo raster}
}
\examples{
# Numeric matrix. 
m <- matrix(seq(0, 1, length.out = 30 * 40), 30, 40)
palette <- terrain.colors(100)
cr <- matrix_to_cr(m, palette)
plot(cr, T)
 
# Integer matrix. Values in matrix will used to lookup palette by index
m <- matrix(1:12, 3, 4)
palette <- rainbow(12)
cr <- matrix_to_cr(m, palette) 
plot(cr, T)

}
\seealso{
Other image conversion functions: 
\code{\link{array_to_cr}()},
\code{\link{cr_to_array}()},
\code{\link{cr_to_nr}()},
\code{\link{cr_to_raster}()},
\code{\link{magick_to_cr}()},
\code{\link{nr_to_cr}()},
\code{\link{raster_to_cr}()}
}
\concept{image conversion functions}
