-- Click within the image and press
-- the left and right mouse buttons to 
-- change the value of the rectangle
function draw()
  if (isMousePressed() and (mouseButton() == LEFT)) then
    fill(0);
  elseif (mousePressed() and (mouseButton() == RIGHT)) then
    fill(255);
  else
    fill(126);
  end
  rect(25, 25, 50, 50);
end
