
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 14-15: Rotating many things using objects

-- An array of Rotater objects

local theta1 = 0;
local theta2 = 0;

function setup()
  size(200, 200);
end

function draw()
   background(255);
   stroke(0);
   fill(175);
   rectMode(CENTER);

   -- Save the current transformation matrix. 
   -- This is where we started, with (0,0) in the top left corner of the window and no rotation.
   pushMatrix(); 

   -- Translate and rotate the first rectangle.
   translate(50, 50); 
 --  rotateZ(theta1);
   -- Display the first rectangle.
   rect(0, 0, 60, 60); 
   -- Restore matrix from Step 1 so that it isn't affected by Steps 2 and 3!
   popMatrix(); 

   pushMatrix();
   -- Translate and rotate the second rectangle.
   translate(150, 150); 
--   rotateY(theta2);
   -- Display the second rectangle.
   rect(0, 0, 60, 60); 
   popMatrix();

   theta1 = theta1 + 0.02;
   theta2 = theta2 + 0.02;
end
