

img_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
img <- cr_read_png(img_file)


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Native raster conversion
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("native raster conversion works", {
  
  x <- cr_to_nr(img)
  
  expect_true(inherits(x, 'nativeRaster'))
  vdiffr::expect_doppelganger(
    "Conversion to native raster works", 
    fig = \() grid::grid.raster(x, interpolate = FALSE)
  )
})


test_that("native raster conversion works", {
  
  x <- cr_to_nr(img)
  cara2 <- nr_to_cr(x)
  vdiffr::expect_doppelganger(
    "Conversion from native raster works", 
    fig = \() plot(cara2, T)
  )
})



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Array conversion
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("array conversion works", {

  x <- cr_to_array(img)

  expect_true(inherits(x, 'array'))
  vdiffr::expect_doppelganger(
    "Conversion to array works",
    fig = \() plot(as.raster(x))
  )
})


test_that("array conversion works", {

  x <- cr_to_array(img)
  cara2 <- array_to_cr(x)
  vdiffr::expect_doppelganger(
    "Conversion from array works",
    fig = \() plot(cara2, T)
  )
})


#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Native raster conversion
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("raster conversion works", {

  x <- cr_to_raster(img)

  expect_true(inherits(x, 'raster'))
  vdiffr::expect_doppelganger(
    "Conversion to raster works",
    fig = \() plot(x)
  )
})

test_that("raster conversion works", {

  x <- cr_to_raster(img)
  cara2 <- raster_to_cr(x)
  vdiffr::expect_doppelganger(
    "Conversion from raster works", fig = \() plot(cara2, T)
  )
})



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Magick conversion
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("magick conversion works", {

  testthat::skip_if_not_installed('magick')
  x <- cr_to_magick(img)

  expect_true(inherits(x, 'magick-image'))
})


test_that("magick conversion works", {

  testthat::skip_if_not_installed('magick')
  
  # there's a deep valgrind issue which pops up w.r.t. magick
  # OK when testing locally on on most rhub test machines.
  # it's just valgrind which is having an issue here.
  testthat::skip_on_ci()
  testthat::skip_on_cran()  

  im <- magick::logo
  cr <- magick_to_cr(im)

  vdiffr::expect_doppelganger(
    "Conversion from magick works",
    fig = \() plot(cr, T)
  )
})



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Matrix conversion
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("matrix (numeric) conversion works", {

  mat <- cr_to_array(img)[,,2] # green channel
  cr <- matrix_to_cr(mat, grey(seq(0, 1, 0.01)))

  vdiffr::expect_doppelganger(
    "Conversion from matrix works",
    fig = \() plot(cr, T)
  )
})



#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Matrix conversion
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
test_that("matrix (integer) conversion works", {

  mat <- cr_to_array(img)[,,2] # green channel
  mat[] <- as.integer(255 * mat)
  mode(mat) <- 'integer'
  cr <- matrix_to_cr(mat, grey(seq(0, 1, length.out = 256)))

  vdiffr::expect_doppelganger(
    "Conversion from matrix (integer) works",
    fig = \() plot(cr, T)
  )
})

