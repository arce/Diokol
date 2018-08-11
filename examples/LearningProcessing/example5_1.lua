
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-1: Conditionals

local mouseX = 0
local mouseY = 0
local width = 480
local height = 270

-- Variables
local r = 150
local g = 0
local b = 0

function setup()
  size(width, height)
end

function draw()
  -- Draw stuff
  background(r,g,b)
  stroke(255)
  line(width/2,0,width/2,height)


  -- If the mouse is on the right side of the screen is equivalent to 
  -- "if mouseX is greater than width divided by 2."
  if (mouseX > width/2) then
    r = r + 1 
  else
    r = r - 1
  end

  -- If r is greater than 255, set it back to 255.  
  -- If r is less than 0, set it back to 0.
  if (r > 255) then
    r = 255
  elseif (r < 0) then
    r = 0
  end
end

function mouseMoved(x,y)
  mouseX = x
  mouseY = y
end

function windowResized(w,h)
	width = w
	height = h
end
