--[[
 * Array. 
 * 
 * An array is a list of data. Each piece of data in an array 
 * is identified by an index number representing its position in 
 * the array. Arrays are zero based, which means that the first 
 * element in the array is [0], the second element is [1], and so on. 
 * In this example, an array named "coswave" is created and
 * filled with the cosine values. This data is displayed three 
 * separate ways on the screen.  
 */
]]
local width = 640;
local height = 360;
local coswave; 
function setup()
  size(width, height);
  coswave ={}
  for i = 1, width do
    local amount = map(i, 0, width, 0, PI);
    coswave[i] = math.abs(math.cos(amount));
  end
  background(255);
  noLoop();
end
function draw()
  local y1 = 0;
  local y2 = height/3;
  for i = 1, width do
    stroke(coswave[i]*255);
    line(i, y1, i, y2);
  end
  y1 = y2;
  y2 = y1 + y1;
  for i = 1, width do
    stroke(coswave[i]*255 / 4);
    line(i, y1, i, y2);
  end
  
  y1 = y2;
  y2 = height;
  for i = 1,width do
    stroke(255 - coswave[i]*255);
    line(i, y1, i, y2);
  end
  
end
