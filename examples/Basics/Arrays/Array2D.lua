--[[/**
 * Array 2D. 
 * 
 * Demonstrates the syntax for creating a two-dimensional (2D) array.
 * Values in a 2D array are accessed through two index values.  
 * 2D arrays are useful for storing images. In this example, each dot 
 * is colored in relation to its distance from the center of the image. 
 */
]]
local width = 640;
local height = 360;
local distances;
local maxDistance;
local spacer;
function setup()
  size(width, height);
  maxDistance = dist(width/2, height/2, width, height);
  distances = {{}};
  for x = 1, width do
     distances[x] = {}
    for y = 1, height do
      local distance = dist(width/2, height/2, x, y);
      distances[x][y] = distance/maxDistance * 255;
    end
  end
  spacer = 10;
  strokeWeight(6);
  noLoop();  -- Run once and stop
end
function draw()
  background(0);
  -- This embedded loop skips over values in the arrays based on
  -- the spacer variable, so there are more values in the array
  -- than are drawn here. Change the value of the spacer variable
  -- to change the density of the points
  for y = 1, height, spacer do
    for x = 1, width, spacer do
      stroke(distances[x][y]);
      point(x + spacer/2, y + spacer/2);
    end
  end
end
function dist(x1,y1,x2,y2)
    return math.sqrt(math.pow(x2-x1,2)+math.pow(y2-y1,2));
end
