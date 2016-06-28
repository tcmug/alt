

-- http://lua-users.org/wiki/ObjectOrientationTutorial

local preferences = {}
preferences.__index = preferences

-- Equivalent to "MyClass.new = function..."
function preferences.new(name)
  local self = setmetatable({}, preferences)
  self.name = name
  return self
end


function preferences:set(id, value)
    print(self.name)
end


local xx = preferences.new("bob")
xx:set(2, "asd")

