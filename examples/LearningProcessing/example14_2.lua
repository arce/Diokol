
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 14-2: Multiple translations

local mouseX = 0
local mouseY = 0

function setup()
  size(200, 200);
  frameRate(10);
end

function draw()
  background(255);
  stroke(0);
  fill(175);

  -- Grab mouse coordinates, constrained to window
  local mx = constrain(mouseX, 0, width());
  local my = constrain(mouseY, 0, height());

  -- Translate to the mouse location
  translate(mx, my);
  ellipse(0, 0, 8, 8);

  -- Translate 100 pixels to the right
  translate(100, 0);
  ellipse(0, 0, 8, 8);

  -- Translate 100 pixels down
  translate(0, 100);
  ellipse(0, 0, 8, 8);

  -- Translate 100 pixels left
  translate(-100, 0);
  ellipse(0, 0, 8, 8);
end

function mouseMoved(x,y)
  mouseX = x
  mouseY = y
end
