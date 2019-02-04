
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 17-1: Simple displaying text

-- Step 2: Declare PFont variable
local f

function setup()
  size(480, 270)

  -- Step 3: Create Font
  f = loadFont("data/Vera.ttf",16)
end

function draw()
  background(255)
  textFont(f)	-- Step 4: Specify font to be used
  fill(0)		-- Step 5: Specify font color

  -- Step 6: Display Text
  text("Mmmmm ... Strings ...", 10, height()/2)
end
