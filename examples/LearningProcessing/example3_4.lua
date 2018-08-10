
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 3-4: Drawing a continuous line
local mouseX=0
local mouseY=0
local pmouseX=0
local pmouseY=0

function setup()
  size(200, 200)
  background(255)
  smooth()
end

function draw()
  stroke(0)
  -- Draw a line from previous mouse location to current mouse location.
  line(pmouseX, pmouseY, mouseX, mouseY)
end

function mouseMoved(x,y)
  pmouseX = mouseX
  pmouseY = mouseY
  mouseX = x
  mouseY = y
end