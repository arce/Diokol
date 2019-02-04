local f;

function setup()
  size(480, 270)

  -- Step 3: Create Font
  f = loadFont("data/Vera.ttf",32)
end

function draw()
  textFont(f)
  text("word", 10, 30); 
  fill(0, 102, 153);
  text("word", 10, 60);
  fill(0, 102, 153, 51);
  text("word", 10, 90);
end
