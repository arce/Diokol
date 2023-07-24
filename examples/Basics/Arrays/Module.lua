Module = {}

  -- Contructor
function Module:new(xOffsetTemp, yOffsetTemp, xTemp, yTemp, speedTemp, tempUnit)
  local m = {
    xOffset = xOffsetTemp,
    yOffset = yOffsetTemp,
    x = xTemp,
    y = yTemp,
    speed = speedTemp,
    unit = tempUnit,
    xDirection = 1,
    yDirection = 1
  };
  setmetatable(m,self);
  self.__index = self;
  return m;
end

 -- Custom method for updating the variables
function Module:update()
  self.x = self.x + (self.speed * self.xDirection);
  if (self.x >= self.unit or self.x <= 0) then
    self.xDirection = self.xDirection * -1;
    self.x = self.x + (1 * self.xDirection);
    self.y = self.y + (1 * self.yDirection);
  end
  if (self.y >= self.unit or self.y <= 0) then
      self.yDirection = self.yDirection * -1;
      self.y = self.y + (1 * self.yDirection);
  end
end
  
  -- Custom method for drawing the object
function Module:display()
  fill(255);
  ellipse(self.xOffset + self.x, self.yOffset + self.y, 6, 6);
end
