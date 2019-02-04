-- Click within the image to change 
-- the value of the rectangle after
-- after the mouse has been clicked

local value = 0;

function draw()
  fill(value);
  rect(25, 25, 50, 50);
end

function mouseClicked()
  if (value == 0) then
    value = 255;
  else
    value = 0;
  end
end
