

test_that("cairo-core works", {
  
  img <- cr_new(200, 100, fill = 0L)
  expect_equal(cr_dim(img), c(100, 200))
  nr <- cr_to_nr(img)
  expect_equal(nr[1, 1], 0)
  
  
  cr_fill(img, '#FF0000')
  nr <- cr_to_nr(img)
  red <- -16776961L # colorfast::col_to_int('red')
  expect_equal(nr[1, 1], red)
  
  img2 <- cr_duplicate(img)
  expect_identical(cr_dim(img), cr_dim(img2))
  
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # Fill overwrites without taking alpha into account
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cr_fill(img2, 0L)
  # plot(img2, T)
  nr <- cr_to_nr(img2)
  expect_equal(nr[1, 1], 0)
  
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # Copy into overwrites without taking alpha into account
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  cr_copy_into(img, img2)
  nr <- cr_to_nr(img)
  expect_equal(nr[1, 1], 0)
  
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  # Resize
  #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  img3 <- cr_resize(img, 100, 50)
  expect_equal(cr_dim(img3), c(50, 100))

})
