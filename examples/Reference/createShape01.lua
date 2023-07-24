local square  -- The PShape object

function setup()
  size(100, 100);
  -- Creating the PShape as a square. The
  -- numeric arguments are similar to rect().
  square = createShape(RECT, 0, 0, 50, 50);
end

function draw()
  setFill(color(0, 0, 255));
  setStroke(false);
  shape(square, 25, 25);
end
