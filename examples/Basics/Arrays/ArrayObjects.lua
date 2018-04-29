require "Module"

local width = 640;
local height = 360;
local mods;

function setup()
  size(width, height);
  noStroke();
  local unit = 40;
  local wideCount = width / unit;
  local highCount = height / unit;
  local count = wideCount * highCount;
  mods = {};

  local index = 0;
  for y = 0, highCount do
    for x = 0, wideCount do
      index = index + 1
      mods[index] = Module:new(x*unit, y*unit, unit/2, unit/2, math.random(5,80)/100, unit);
    end
  end
end

function draw()
  background(0);
  for i,mod in ipairs(mods) do
    mod:update();
    mod:display();
  end
end
