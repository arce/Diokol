
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 15-2: Image "sprite"

local head; -- A variable for the image file
local x,y;   -- Variables for image location
local rot;   -- A variable for image rotation

function setup()
  size(200,200);
  
  -- Load image, initialize variables
  head = loadImage("data/face.jpg");
  x = 0.0;
  y = width()/2.0;
  rot = 0.0;
end

function draw()
  background(255);
  
  -- Translate and rotate
  translate(x,y);
  rotate(rot);
  
  -- Images can be animated just like regular shapes using variables, translate(), rotate(), and so on.
  imageMode(CENTER);
  image(head,0,0); 
  
  -- Adjust variables for animation
  x = x + 1.0;
  rot = rot + 0.02;
  if (x > width()+imageWidth(head)) then
    x = -imageWidth(head);
  end
end
