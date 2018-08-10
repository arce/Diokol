
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 3-2: mouseX and mouseY

local mouseX=0
local mouseY=0

function setup()
  size(480, 270)
end

function draw()
  -- Try moving background() to setup() and see the difference!
  background(255)

  -- Body
  stroke(0)
  fill(175)
  rectMode(CENTER)
  
  -- mouseX is a keyword that the sketch replaces with the horizontal position of the mouse.
  -- mouseY is a keyword that the sketch replaces with the vertical position of the mouse.
  rect(mouseX,mouseY,50,50)
end

function mouseMoved(x,y)
  mouseX = x
  mouseY = y
end
