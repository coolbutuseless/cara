
test_that("PNG read/write works", {
  
  
  logo_file <- system.file("img/logo-rainbow.png", package = "cara")
  logo <- cr_read_png(logo_file)
  
  tmp <- tempfile()
  cr_write_png(logo, tmp)
  logo2 <- cr_read_png(tmp)
  
  vdiffr::expect_doppelganger(
    "PNG read/write works", 
    fig = \() plot(logo2, T)
  )
  
})
