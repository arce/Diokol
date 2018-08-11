
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 5-10: Zoog and conditionals
local x = 100
local y = 100
local w = 60
local h = 60
local eyeSize = 16

-- Zoog has variables for speed in the horizontal and vertical direction.
local xspeed = 3
local yspeed = 1

function setup()
  size(480,270)
end

function draw()
    -- Change the location of Zoog by speed
    x = x + xspeed
    y = y + yspeed

    -- An IF statement with a logical OR determines if Zoog has reached either the right or left edge of the screen.  
    -- When this is true, we multiply speed by -1, reversing Zoog's direction!
    -- Identical logic is applied to the y direction as well.
    if ((x > width()) or (x < 0)) then
      xspeed = xspeed * -1
  	end

    if ((y > height()) or (y < 0)) then
      yspeed = yspeed * -1
  	end

    background(255)
    ellipseMode(CENTER)
    rectMode(CENTER)

    -- Draw Zoog's body
    stroke(0)
    fill(150)
    rect(x,y,w/6,h*2)

    -- Draw Zoog's head
    fill(255)
    ellipse(x,y-h/2,w,h)

    -- Draw Zoog's eyes
    fill(0)
    ellipse(x-w/3+1,y-h/2,eyeSize,eyeSize*2)
    ellipse(x+w/3-1,y-h/2,eyeSize,eyeSize*2)

    -- Draw Zoog's legs
    stroke(0)
    line(x-w/12,y+h,x-w/4,y+h+10)
    line(x+w/12,y+h,x+w/4,y+h+10)
end
