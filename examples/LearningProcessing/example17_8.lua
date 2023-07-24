
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 17-8: Characters along a curve 

-- The message to be displayed
local message = "text along a curve";
local f;

-- The radius of a circle
local r = 100;

function setup()
  size(480, 270);
  f = loadFont("data/Vera.ttf", 40);
  textFont(f);
  -- The text must be centered!
  textAlign(CENTER);
end

function draw()
  background(255);

  -- Start in the center and draw the circle
  translate(width()/2, height()/2);
  noFill();
  stroke(0);
  ellipse(0, 0, r*2, r*2);

  -- We must keep track of our position along the curve
  local arclength = 0;

  -- For every box
  for i = 1,message:len() do

    -- The character and its width
    local currentChar = message:sub(i,i);
    print(currentChar);
    -- Instead of a constant width, we check the width of each character.
    local w = textWidth(currentChar);
    print(w);
    -- Each box is centered so we move half the width
    arclength = arclength + w/2;

    -- Angle in radians is the arclength divided by the radius
    -- Starting on the left side of the circle by adding PI
    local theta = PI + arclength / r;

    pushMatrix();

    -- Polar to Cartesian conversion allows us to find the point along the curve. See Chapter 13 for a review of this concept.
    translate(r*math.cos(theta), r*math.sin(theta)); 
    -- Rotate the box (rotation is offset by 90 degrees)
    rotate(theta + PI/2); 

    -- Display the character
    fill(0);
    text(currentChar, 0, 0);

    popMatrix();

    -- Move halfway again
    arclength = arclength + w/2;
  end
end

