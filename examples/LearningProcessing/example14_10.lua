
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 14-10: Pyramid

local theta = 0.0;

function setup()
  size(480, 240, P3D);
end

function draw()
  background(255);
  theta = theta + 0.01
  
  translate(width()/2, height()/2, 0);
--  rotateX(theta);
--  rotateY(theta);
  drawPyramid(50);

  -- translate the scene again
  translate(100, 100, 20);
  -- call the pyramid drawing function
  drawPyramid(10);
end

-- The function sets the vertices for the pyramid around the centerpoint at a flexible distance, 
-- depending on the number passed in as an argument.
function drawPyramid(t) 

  stroke(0);

  -- this pyramid has 4 sides, each drawn as a separate triangle
  -- each side has 3 vertices, making up a triangle shape
  -- the parameter " t " determines the size of the pyramid
  beginShape(TRIANGLES);

  fill(255, 150); -- Note that each polygon can have its own color.
  vertex(-t, -t, -t);
  vertex( t, -t, -t);
  vertex( 0, 0, t);

  fill(150, 150);
  vertex( t, -t, -t);
  vertex( t, t, -t);
  vertex( 0, 0, t);

  fill(255, 150);
  vertex( t, t, -t);
  vertex(-t, t, -t);
  vertex( 0, 0, t);

  fill(150, 150);
  vertex(-t, t, -t);
  vertex(-t, -t, -t);
  vertex( 0, 0, t);

  endShape();
end
