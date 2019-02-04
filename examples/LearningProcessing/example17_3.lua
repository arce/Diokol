
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 17-3: Scrolling headlines 

-- An array of news headlines
local headlines = {
  "Processing downloads break downloading record.", 
  "New study shows computer programming lowers cholesterol.",
};

local f; -- Global font variable
local x; -- Horizontal location
local index = 1;

function setup()
  size(480, 270);
  f = loadFont("data/Vera.ttf", 16);

  -- Initialize headline offscreen
  x = width();
end

function draw()
  background(255);
  fill(0);

  -- Display headline at x location
  textFont(f);
  textAlign (LEFT);

  -- A specific String from the array is displayed according to the value of the "index" variable.
  text(headlines[index], x, height()-20); 

  -- Decrement x
  x = x - 3;

  -- If x is less than the negative width, then it is off the screen
  -- textWidth() is used to calculate the width of the current String.
  local w = textWidth(headlines[index]);
  if (x < -w) then
    x = width;
    -- index is incremented when the current String has left the screen in order to display a new String.
    index = (index + 1) % table.getn(headlines);
  end
end
