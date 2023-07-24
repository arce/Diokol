-- Click on the window to give it focus,
-- and press the 'B' key.

function draw()
  if (isKeyPressed()) then
    if (key() == 'b' or key() == 'B') then
      fill(0);
    end
  else
    fill(255);
  end
  rect(25, 25, 50, 50);
end
