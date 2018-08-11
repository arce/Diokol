
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 14-13: Rotating another square

local theta2 = 0;

function setup()
  size(200, 200);
end

function draw()
   background(255);
   stroke(0);
   fill(175);
   rectMode(CENTER);

   translate(150, 150);
--   rotateY(theta2);
   rect(0, 0, 60, 60);

   theta2 = theta2 + 0.02;
end
