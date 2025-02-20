
test_that("blitting works", {
  logo_file <- system.file("img/logo-rainbow.png", package = "cara")
  logo <- cr_read_png(logo_file)
  canvas <- cr_new(1200, 800)
  cr_blit(dst = canvas, src = logo, x = c(100, 400, 800), y = 400, 
          xscale = seq(0.1, 0.5, length.out = 3))
  
  vdiffr::expect_doppelganger("blitting", fig = \() plot(canvas))
})
