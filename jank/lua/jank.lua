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
  print('Initialized Lua state.')
end

function getSample(isLeft)
  local output = maximua.oscSine(440, isLeft);
	return output
end

phaseL = 0
phaseR = 0