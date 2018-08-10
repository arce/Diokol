
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 4-6: Ellipse with variables

-- Declare and initialize your variables!
local r = 100
local g = 150
local b = 200
local a = 200
  
local diam = 20
local x = 100
local y = 100
  
function setup()
  size(480, 270)
  background(255)
end

function draw()
  -- Use those variables to draw an ellipse
  stroke(0)
  fill(r,g,b,a)  -- (Remember, the fourth argument for a color is transparency.
  ellipse(x,y,diam,diam)  
end
