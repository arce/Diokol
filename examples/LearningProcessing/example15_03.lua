
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 15-3: Swapping images

local maxImages = 10; -- Total # of images
local imageIndex = 1; -- Initial image to be displayed is the first

-- Declaring an array of images.
local images = {}; 

function setup()
  size(200, 200);

  -- Loading the images into the array
  -- Don't forget to put the JPG files in the data folder!
  for i = 1,maxImages do
    images[i] = loadImage( "data/animal" .. (i-1) .. ".jpg" );
  end
end

function draw()
  -- Displaying one image
  image(images[imageIndex], 0, 0)
end

function mousePressed(x,y)
  -- A new image is picked randomly when the mouse is clicked
  -- Note the index to the array must be an integer!
  imageIndex = math.random(maxImages)
end
