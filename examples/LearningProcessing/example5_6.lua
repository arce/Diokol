
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-6: Bouncing Ball
local x = 0
local speed = 2

function setup()
  size(480, 270);
end

function draw()
    background(255)

    -- Add the current speed to the x location.
    x = x + speed

    -- Remember, || means "or."
    if ((x > width()) or (x < 0)) then
      -- If the object reaches either edge, multiply speed by -1 to turn it around.
      speed = speed * -1
	end

    -- Display circle at x location
    stroke(0)
    fill(175)
    ellipse(x,100,32,32)
end
