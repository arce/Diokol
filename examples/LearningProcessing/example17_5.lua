
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 17-5: Rotating text 

local f;
local message = "this text is spinning";
local theta=0;

function setup()
  size(480, 270);
  f = loadFont("data/Vera.ttf", 16);
end

function draw()

  background(255);
  fill(0);
  textFont(f);                  -- Set the font
  textAlign(CENTER);
  pushMatrix();
  translate(width()/2, height()/2); -- Translate to the center
  rotate(theta);                -- Rotate by theta

  -- The text is center aligned and displayed at (0,0) after translating and rotating. 
  -- See Chapter 14 or a review of translation and rotation.
  text(message, 0, 0); 
  popMatrix();

  -- Increase rotation
  theta = theta + 0.02;
end
