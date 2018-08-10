
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 1-2: noFill

function setup() 
  size(360, 270) 
end

function draw()
  background(255)

  -- noFill() leaves the shape with only an outline.
  noFill()
  stroke(0)
  ellipse(60, 60, 100, 100)
end
