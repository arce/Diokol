
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 15-1: "Hello World" images

-- Declaring a variable of type image
-- PImage is a class available from the Processing core library.
local img;

function setup()
  size(320, 240);
  -- Make a new instance of a PImage by loading an image file
  img = loadImage('data/runde_bird_cliffs.jpg');
end

function draw()
  background(0);
  -- The image() function displays the image at a location
  -- in this case the point (0,0).
  image(img, 0, 0, width(), height());
end
