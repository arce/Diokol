
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-3: Rollovers

local mouseX=0
local mouseY=0

function setup()
  size(480,270)
end

function draw()
	background(255)
	stroke(0)
	line(320,0,320,360)
	line(0,180,640,180)

	-- Fill a black color
	noStroke()
	fill(0)

	-- Depending on the mouse location, a different rectangle is displayed.    
	if (mouseX < 320 and mouseY < 180) then
		rect(0,0,320,180)
	elseif (mouseX > 320 and mouseY < 180) then
		rect(320,0,320,180)
	elseif (mouseX < 320 and mouseY > 180) then
		rect(0,180,320,180)
	elseif (mouseX > 320 and mouseY > 180) then
		rect(320,180,320,180)
	end
end

function mouseMoved(x,y)
  mouseX = x
  mouseY = y
end
