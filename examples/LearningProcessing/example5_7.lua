
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-7: "Bouncing" color

-- Two variables for color.
local c1 = 0
local c2 = 255

-- Start by incrementing c1.
local c1Change = 1
-- Start by decrementing c2.
local c2Change = -1

function setup()
  size(480,270)
end

function draw()
    noStroke()

    -- Draw rectangle on left
    fill(c1, 0, c2)
    rect(0, 0, 320, 360)

    -- Draw rectangle of right
    fill(c2, 0, c1)
    rect(320, 0, 320, 360)

    -- Adjust color values
    c1 = c1 + c1Change
    c2 = c2 + c2Change

    -- Instead of reaching the edge of a window, these variables reach the "edge" of color:  
    -- 0 for no color and 255 for full color.  
    -- When this happens, just like with the bouncing ball, the change is reversed.

    -- Reverse direction of color change 
    if (c1 < 0 or c1 > 255) then
      c1Change = c1Change * -1
  	end

    if (c2 < 0 or c2 > 255) then
      c2Change = c2Change * -1
	end
end
