
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 14-12: Rotating one square

local theta1 = 0;

function setup()
  size(200, 200);
end

function draw()
  background(255);
  stroke(0);
  fill(175);
  rectMode(CENTER);

  translate(50, 50);
 -- rotateZ(theta1);
  rect(0, 0, 60, 60);

  theta1 = theta1 + 0.02;
end

