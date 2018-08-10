
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 4-4: Many variables

-- We've got 8 variables now!  All  of type float.
local circleX = 0
local circleY = 0
local circleW = 50
local circleH = 100
local circleStroke = 255
local circleFill = 0
local backgroundColor = 255
local change = 0.5

-- Your basic setup
function setup()
  size(480, 270)
end

function draw()
  -- Draw the background and the ellipse
  -- Variables are used for everything: background, stroke, fill, location, and size.
  background(backgroundColor)
  stroke(circleStroke)
  fill(circleFill)
  ellipse(circleX,circleY,circleW,circleH)

  -- Change the values of all variables
  -- The variable change is used to increment and decrement the other variables.
  circleX = circleX + change
  circleY = circleY + change
  circleW = circleW + change
  circleH = circleH - change
  circleStroke = circleStroke - change
  circleFill = circleFill + change
end
