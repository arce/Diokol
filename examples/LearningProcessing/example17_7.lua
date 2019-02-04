
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 17-7: Boxes along a curve 

-- The radius of a circle
local r = 100;

-- The width and height of the boxes
local w = 40;
local h = 40;

function setup()
  size(480, 270);
end

function draw()
  background(255);

  -- Start in the center and draw the circle
  translate(width()/2, height()/2);
  noFill();
  stroke(0);
  -- Our curve is a circle with radius r in the center of the window.
  ellipse(0, 0, r*2, r*2); 
  -- 10 boxes along the curve
  local totalBoxes = 10;
  -- We must keep track of our position along the curve
  local arclength = 0;
  -- For every box
  for i = 0,totalBoxes do
    -- Each box is centered so we move half the width
    arclength = arclength + w/2; 

    -- Angle in radians is the arclength divided by the radius
    local theta = arclength / r;

    pushMatrix();
    -- Polar to cartesian coordinate conversion
    translate(r*math.cos(theta), r*math.sin(theta));
    -- Rotate the box
    rotate(theta);

    -- Display the box
    fill(0, 100);
    rectMode(CENTER);
    rect(0, 0, w, h);
    popMatrix();

    -- Move halfway again
    arclength = arclength + w/2;
  end
end
