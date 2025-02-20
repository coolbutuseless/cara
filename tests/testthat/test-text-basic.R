test_that("text-basic works", {
  
  testthat::skip_on_ci()
  testthat::skip_on_cran()  # valgind sees memory leak within 'fontconfig' within 'cairo'
  
  img <- cr_new(200, 100, 'grey80')
  cr_text_basic(img, "Hello", 10, 95, size = 80, family = 'sans-serif')
  
  vdiffr::expect_doppelganger(
    "text-glyph",
    fig = \() plot(img, T)
  )
  
})
