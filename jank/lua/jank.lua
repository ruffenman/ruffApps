function init()
  --require('mobdebug').start()
  
  local luaXinput
  local maximua
  if DEBUG_BUILD then 
    luaXinput = require "luaXinputD"
    maximua = require "maximuaD"
  else
    luaXinput = require "luaXinput"
    maximua = require "maximua"
  end
  luaXinput.enable()
  
  for k, v in pairs(maximua) do print(k, v) end
  
end

function getSampleLeft()
  local output = math.sin(phaseL * (2 * math.pi))
	if ( phaseL >= 1.0 ) then
    phaseL = phaseL - 1.0
  end
	phaseL = phaseL + (1 / (44100 / 440))
	return output
end

function getSampleRight()
  local output = math.sin(phaseR * (2 * math.pi))
	if ( phaseR >= 1.0 ) then
    phaseR = phaseR - 1.0
  end
	phaseR = phaseR + (1 / (44100 / 220))
	return output
end

phaseL = 0
phaseR = 0