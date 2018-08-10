
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 4-5: Using system variables
local mouseX=0
local mouseY=0

function setup()
  size(480, 270)
  frameRate(60)
end

function draw()
  background(100)
  stroke(255)
  -- frameCount is used to color a rectangle.
  fill(frameCount() / 2)
  rectMode(CENTER)
  -- The rectangle will always be in the middle of the window 
  -- if it is located at (width/2, height/2).
  rect(width()/2,height()/2,mouseX+10,mouseY+10)
end

function keyPressed()
  print(key)
end

function mouseMoved(x,y)
  mouseX = x
  mouseY = y
end
