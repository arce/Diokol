
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 4-7: Filling variables with random values

local r
local g
local b
local a

local diam
local x
local y

function setup()
  size(480, 270)
  background(255)
  frameRate(30)
end

function draw()
  -- Each time through draw(), new random numbers are picked for a new ellipse.
  r = math.random(0,255)
  g = math.random(0,255)
  b = math.random(0,255)
  a = math.random(0,255)
  diam = math.random(0,20)
  x = math.random(0,width())
  y = math.random(0,height())
  
  -- Use values to draw an ellipse
  noStroke()
  fill(r,g,b,a)
  ellipse(x,y,diam,diam)
end
