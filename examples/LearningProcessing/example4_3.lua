
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 4-3: Varying variables

-- Declare and initialize two integer variables at the top of the code.
local circleX = 0
local circleY = 100

function setup()
  size(480, 270)
end

function draw()
  background(255)
  stroke(0)
  fill(175)
  -- Use the variables to specify the location of an ellipse.
  ellipse(circleX,circleY,50,50)

  -- An assignment operation that increments the value of circleX by 1.
  circleX = circleX + 1
end

