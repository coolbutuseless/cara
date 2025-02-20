
test_that("text-glyph works", {
  
  testthat::skip_if_not_installed("textshaping")
  testthat::skip_on_ci()
  testthat::skip_on_cran()
  
  ts <- textshaping::shape_text(
      "hello", family = "sans-serif", id = 1, size = 80, weight = 400
  )
  glyphs <- ts$shape 
  img <- cr_new(200, 200)
  cr_text_glyph(
     cr     = img, 
     idx    = glyphs$index, 
     x      = glyphs$x_offset + 10, 
     y      = glyphs$y_offset + 100, 
     family = 'sans-serif', 
     size   = 80, 
     weight = 'bold', 
     slant  = 'oblique'
  )
  
  vdiffr::expect_doppelganger(
    "text-glyph",
    fig = \() plot(img, T)
  )
  
})

