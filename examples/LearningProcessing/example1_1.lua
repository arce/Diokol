
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 1-1: stroke and fill

function setup() 
  size(640, 480) 
end

function draw()
  background(255)
  stroke(0)
  fill(150)
  rect(50, 50, 75, 100)
end
