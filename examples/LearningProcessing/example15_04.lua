
-- Learning Processing
-- Daniel Shiffman
-- http://www.learningprocessing.com

-- Example 15-4: Image sequence

local maxImages = 10; -- Total # of images
local imageIndex = 0; -- Initial image to be displayed is the first

-- Declaring an array of images.
local images = {}; 

function setup()
  size(200, 200);

  -- Loading the images into the array
  -- Don't forget to put the JPG files in the data folder!
  for i = 1,maxImages do
    images[i] = loadImage( "data/animal" .. (i-1) .. ".jpg" );
  end
  frameRate(5)
end

function draw()
  background(0)
  image(images[imageIndex+1], 0, 0)
  imageIndex = (imageIndex + 1) % maxImages
  print(imageIndex)
end
