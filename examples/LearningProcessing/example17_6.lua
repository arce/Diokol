
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 17-6: Text breaking up 

local f;
local message = "click mouse to shake it up";

-- An array of Letter objects
local letters = {};

function setup()
  size(480, 270);

  -- Load the font
  f = loadFont("data/Vera.ttf", 16);
  textFont(f);

  -- Create the array the same size as the String
  letters = new Letter[message.length()];

  -- Initialize Letters at the correct x location
  int x = 125;
  for i = 0,message.length do
    -- Letter objects are initialized with their location within the String as well as what character they should display.
    letters[i] = new Letter(x, 140, message.charAt(i)); 
    x += textWidth(message.charAt(i));
  end
end

function draw()
  background(255);
  for i=0,letters.length do

    -- Display all letters
    letters[i].display();

    -- If the mouse is pressed the letters shake
    -- If not, they return to their original location
    if (mousePressed) then
      letters[i].shake();
    else
      letters[i].home();
    end
  end
end
