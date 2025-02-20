

test_that("transform works", {
  canvas <- cr_new(800, 600, fill = 'royalblue')
  
  cr_rotate(canvas, 0.1)
  cr_translate(canvas, 200, 200)
  cr_scale(canvas, 1.1)
  
  cr_rect(canvas, 0, 0, 100, 100, fill = 'grey20', color = 'black')
  
  cr_transform_reset(canvas)
  
  vdiffr::expect_doppelganger(
    "Transforms work", 
    fig = \() plot(canvas, T)
  )
})


test_that("set transform works", {
  
  canvas <- cr_new(800, 800, fill = 'royalblue')
  
  # Shear transformation
  cr_transform_set(canvas, xy = 0.3)
  
  cr_rect(canvas, x = 400, y = 400, w = 100, h = 100, hjust = 0.5, vjust = 0.5)
  
  vdiffr::expect_doppelganger(
    "Set Transform matrix works", 
    fig = \() plot(canvas, T)
  )
  
})


test_that("apply transform works", {
  
  canvas <- cr_new(800, 800, fill = 'royalblue')
  
  # Shear transformation
  cr_scale(canvas, 2)
  cr_transform(canvas, xy = 0.3)
  
  cr_rect(canvas, x = 100, y = 100, w = 100, h = 100, hjust = 0.5, vjust = 0.5)
  
  vdiffr::expect_doppelganger(
    "Apply Transform matrix works", 
    fig = \() plot(canvas, T)
  )
  
})