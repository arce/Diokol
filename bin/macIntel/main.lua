
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 3-3: Zoog as dynamic sketch with variation

local mouseX=0
local mouseY=0

function setup()
  size(640,480)  -- Set the size of the window
--  smooth()
end

function draw()
  background(255)  -- Draw a white background 
  
  -- Set ellipses and rects to CENTER mode
  ellipseMode(CENTER)
  rectMode(CENTER)
  
  -- Draw Zoog's body
  stroke(0)
  fill(175)
  -- Zoog's body is drawn at the location (mouseX, mouseY).
  rect(mouseX,mouseY,20,100)

  -- Draw Zoog's head
  stroke(0)
  fill(255)
  -- Zoog's head is drawn above the body at the location (mouseX, mouseY - 30).
  ellipse(mouseX,mouseY-30,60,60)

  -- Draw Zoog's eyes
  fill(0)
  ellipse(81, 70, 16, 32) 
  ellipse(119, 70, 16, 32)

  -- Draw Zoog's legs
  stroke(0)
  line(90, 150, 80, 160)
  line(110, 150, 120, 160)
  exit()
end

function mouseMoved(x,y)
  mouseX = x
  mouseY = y
end
