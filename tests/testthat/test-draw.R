
test_that("drawing works", {
  
  img <- cr_new(800, 600, 'white')
  
  set.seed(1)
  N <- 10
  x <- runif(N, 0, 800)
  y <- runif(N, 0, 600)
  w <- runif(N, 10, 20)
  h <- runif(N,  5, 15)
  
  
  cr_circle(img, x = x, y = y, r = 50, fill = 'grey80')
  cr_polyline(img, x = x + 20, y = y + 20)
  cr_polygon(img, x = x, y = y, fill = '#ff000060')
  
  
  cr_rect(img, x = x, y = y, w = w, h = h, fill = sample(colors(), N))
  cr_line(img, x0 = 0, y0 = 0, x1 = 800, y1 = 600, color = 'blue')
  
  vdiffr::expect_doppelganger(
    "Drawing works", 
    fig = \() plot(img, T)
  )
  
  
})
