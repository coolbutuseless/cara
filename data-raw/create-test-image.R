

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Glyph layout with {textshaping} package
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ts <- textshaping::shape_text("Cara}", family = "Blessed Day", id = 1, 
                              size = 400,
                              weight = 400)  

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Prepare a canvas to render the text
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cara <- cr_new(600, 400, 0L)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Set a skew transform so the text renders at an angle
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_transform_set(cara, yx = -0.3)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Want a thick outline on the text
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_params_set(cara, params = cpar(line_width = 5))

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Draw the glyphs calculated by {textshaping}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cr_text_glyph(
  cara, 
  idx   = ts$shape$index, 
  x     = ts$shape$x_offset + 50, 
  y     = ts$shape$y_offset + 300,
  fill  = '#d20006',
  # color = 'black',
  family = "Blessed Day",
  size   = 400
)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# Render to screen
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
plot(cara, T)



canvas <- cr_new(600, 400, 0L)

pat <- create_pattern_radial_gradient(rainbow(10), seq(0, 1, length.out = 10), 
                                      x1 = 300, y1 = 200, r1 = 0,
                                      x2 = 300, y2 = 200, r2 = 300)

cr_mask_begin(canvas, cara)
cr_fill(canvas, fill = pat)
cr_mask_end(canvas)

plot(canvas, T)

cr_write_png(canvas, "inst/img/logo-rainbow.png")
