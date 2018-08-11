
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 14-11: A growing rectangle, using scale()

local r = 0.0;

function setup()
  size(200, 200);
end

function draw()
  background(255);
  -- Translate to center of window
  translate(width()/2, height()/2);

  -- scale() increases the dimensions of an object relative to the origin by a percentage (1.0 = 100%). 
  -- Notice how in this example the scaling effect causes the outline of the shape to become thicker.
  scale(r); 

  -- Display a rectangle in the middle of the screen
  stroke(0);
  fill(175);
  rectMode(CENTER);
  rect(0, 0, 10, 10);

  -- Increase the scale variable
  r = r + 0.2;

  -- Retstart r
  if (r > 20) then
    r = 0;
  end
end
