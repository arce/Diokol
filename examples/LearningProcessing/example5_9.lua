
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-9: Simple Gravity

local x = 240   -- x location of square
local y = 0     -- y location of square

local speed = 0   -- speed of square

-- A new variable, for gravity (i.e. acceleration).   
-- We use a relatively small number (0.1) because this accelerations accumulates over time, increasing the speed.   
-- Try changing this number to 2.0 and see what happens.
local gravity = 0.1

function setup()
  size(480,270)
end

function draw()
  background(255)

  -- Display the square
  fill(175)
  stroke(0)
  rectMode(CENTER)
  rect(x, y, 10, 10)

  -- Add speed to location.
  y = y + speed

  -- Add gravity to speed.
  speed = speed + gravity

  -- If square reaches the bottom
  -- Reverse speed
  if (y > height()) then
    -- Multiplying by -0.95 instead of -1 slows the square down each time it bounces (by decreasing speed).  
    -- This is known as a "dampening" effect and is a more realistic simulation of the real world (without it, a ball would bounce forever).
    speed = speed * -0.95
    y = height()
  end
end
