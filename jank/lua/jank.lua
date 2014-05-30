local function lerp(a, b, t)
  return a + (b - a) * t
end

function init()
  --require('mobdebug').start()
  print('Loading modules')
  if DEBUG_BUILD then 
    print('Loading debug modules')
    luaXinput = require "luaXinputD"
    maximua = require "maximuaD"
  else
    luaXinput = require "luaXinput"
    maximua = require "maximua"
  end
  luaXinput.enable()  
  
  oscL = maximua.oscNew();
  oscR = maximua.oscNew();
  state, leftTrigger, rightTrigger, leftX, leftY, rightX, rightY = 0, 0, 0, 0, 0, 0, 0
  print('Initialized Lua state.')
end

function getSample(isLeft)
  if(isLeft) then
    -- Only update input on left channel call (half the updates!)
    state, leftTrigger, rightTrigger, leftX, leftY, rightX, rightY = luaXinput.getState(0)
    return oscL.sinewave(lerp(220, 880, leftTrigger / 255));
  else
    return oscR.sinewave(lerp(220, 880, rightTrigger / 255));
  end
end

local maximua
local luaXinput
local oscL
local oscR
local state, leftTrigger, rightTrigger, leftX, leftY, rightX, rightY