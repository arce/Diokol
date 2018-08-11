-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-4: Hold down the button
local button = false

local x = 50
local y = 50
local w = 100
local h = 75

local mouseX = 0
local mouseY = 0

function setup()
  size(480,270)
end

function draw()
	-- The button is pressed if (mouseX,mouseY) is inside the rectangle and mousePressed is true.
	if (mouseX > x and mouseX < x+w and mouseY > y and mouseY < y+h and isMousePressed()) then
	  button = true
	else
	  button = false
  	end

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

function mouseMoved(x,y)
  mouseX = x
  mouseY = y
end
