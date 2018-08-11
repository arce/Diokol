
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-2: More conditionals

-- Three variables for the background color.
local r = 0
local b = 0
local g = 0

local mouseX = 0
local mouseY = 0

function setup()
  size(480,270)
end

function draw()
    -- Color the background and draw lines to divide the window in quadrants.
    background(r,g,b)
    stroke(255)
    line(width()/2,0,width()/2,height())
    line(0,height()/2,width(),height()/2)
  
    -- If the mouse is on the right hand side of the window, increase red.  
    -- Otherwise, it is on the left hand side and decrease red.
    if (mouseX > width()/2) then
      r = r + 1
    else
      r = r - 1
    end
  
    -- If the mouse is on the bottom of the window, increase blue.  
    -- Otherwise, it is on the top and decrease blue.
    if (mouseY > height()/2) then
      b = b + 1
    else
      b = b - 1
    end
  
    -- If the mouse is pressed (using the system variable mousePressed)
    if (isMousePressed()) then
      g = g + 1
    else
      g = g - 1
    end
  
    -- Constrain all color values to between 0 and 255.
    r = constrain(r,0,255)
    g = constrain(g,0,255)
    b = constrain(b,0,255)
end

function mouseMoved(x,y)
  mouseX = x
  mouseY = y
end
