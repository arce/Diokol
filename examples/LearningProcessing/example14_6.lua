
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 14-6: rotateZ
local theta = 0.0;

function setup()
  size(200, 200);
end

function draw()
	background(255);
	stroke(0);
	fill(175);
	translate(width()/2, height()/2);
	rotate(theta);
	rectMode(CENTER);
	rect(0, 0, 100, 100);
	theta = theta + 0.02;
end
