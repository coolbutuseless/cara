
test_that("pattern image works", {
  logo_file <- system.file("img/logo-rainbow.png", package = 'cara', mustWork = TRUE)
  logo   <- cr_read_png(logo_file)
  
  canvas <- cr_new(1024, 726, fill = 'royalblue')
  
  
  pat <- create_pattern_image(logo, extend = "repeat") |>
    cr_pattern_scale(0.2) |>
    cr_pattern_translate(0.1, 0.1) |>
    cr_pattern_rotate(0.1 * pi)
  
  cr_rect(canvas, 20, 20, 400, 400, fill = pat, color = 'black')

  cr_pattern_transform_reset(pat)
  
  vdiffr::expect_doppelganger(
    "Pattern Image works", 
    fig = \() plot(canvas, T)
  )
})



test_that("pattern linear gradient works", {
  
  canvas <- cr_new(800, 600, fill = 'royalblue')
  
  
  colors <- rainbow(10)
  stops  <- seq(0, 1, length.out = 10)
  pat <- create_pattern_linear_gradient(colors, stops, 0, 0, 800, 600)
  
  cr_rect(canvas, 20, 20, 400, 400, fill = pat, color = 'black')
  
  
  vdiffr::expect_doppelganger(
    "Pattern Linear Gradient works", 
    fig = \() plot(canvas, T)
  )
})



test_that("pattern radial gradient works", {
  
  canvas <- cr_new(800, 600, fill = 'royalblue')
  
  
  colors <- rainbow(10)
  stops  <- seq(0, 1, length.out = 10)
  pat <- create_pattern_radial_gradient(colors, stops, 0, 0, 10, 800, 600, 400)
  
  cr_rect(canvas, 20, 20, 400, 400, fill = pat, color = 'black')
  
  
  vdiffr::expect_doppelganger(
    "Pattern Radial Gradient works", 
    fig = \() plot(canvas, T)
  )
})


