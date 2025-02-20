
test_that("masks work", {
  logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
  logo <- cr_read_png(logo_file)
  img <- cr_new(600, 400)
  
  cr_params_set(img, cpar(line_width = 10))
  
  cr_mask_begin(img, mask = logo)
  cr_circle(img, 300, 200, 200, fill = 'blue', color = 'black')
  cr_circle(img, 350, 200, 100, fill = 'darkgreen', color = 'black')
  cr_mask_end(img)
  
  
  vdiffr::expect_doppelganger(
    "Masks work", 
    fig = \() plot(img, T)
  )
})
