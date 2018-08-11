
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-5: Button as switch
local button = false

local x = 50
local y = 50
local w = 100
local h = 75

function setup()
  size(480,270)
end

function draw()
	if (button) then
	    background(255)
	    stroke(0)
	else
	    background(0)
	    stroke(255)
	end
  
	fill(175)
	rect(x,y,w,h)
end

function mousePressed()
  if (mouseX > x and mouseX < x+w and mouseY > y and mouseY < y+h) then
    button = not button
  end  
end

function mouseMoved(x,y)
  mouseX = x
  mouseY = y
end
