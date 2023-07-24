-- Drag (click and hold) your mouse across the 
-- image to change the value of the rectangle

local value = 0;

function draw()
  fill(value);
  rect(25, 25, 50, 50);
end

function mouseDragged(x,y)
  print("mouseDragged")
  value = value + 5;
  if (value > 255) then
    value = 0;
  end
end
