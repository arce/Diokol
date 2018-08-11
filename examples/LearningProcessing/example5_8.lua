
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-8: Square following edge, uses a "state" variable

local x = 0  -- x location of square
local y = 0  -- y location of square

local speed = 5  -- speed of square

-- A variable to keep track of the square's "state".  
-- Depending on the value of its state, it will either move right, down, left, or up.
local state = 0

function setup()
  size(480,270)
end

function draw()
  background(255)

  -- Display the square
  stroke(0)
  fill(175)
  rect(x,y,9,9)

  -- If the state is 0, move to the right.
  if (state == 0) then
    x = x + speed
    -- If, while the state is 0, it reaches the right side of the window, change the state to 1
    -- Repeat this same logic for all states!?
    if (x > width()-10) then
      x = width()-10
      state = 1
    end
  elseif (state == 1) then
    y = y + speed
    if (y > height()-10) then
      y = height()-10
      state = 2
    end
  elseif (state == 2) then
    x = x - speed
    if (x < 0) then
      x = 0
      state = 3
    end
  elseif (state == 3) then
    y = y - speed
    if (y < 0) then
      y = 0
      state=0
    end
  end
end
