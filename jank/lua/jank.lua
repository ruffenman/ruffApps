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
  local freq = 0;
  local notes = { 27.5, 29.14, 30.87, 32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 55 }
  local keyNotes = { 1, 3, 5, 6, 8, 10, 12, 13 }
  if(isLeft) then
    -- Only update input on left channel call (half the updates!)
    state, leftTrigger, rightTrigger, leftX, leftY, rightX, rightY = luaXinput.getState(0)
    
    -- Convert input to 0 - 1
    local input = leftTrigger / 255
    -- Get octave of note by dividing input in half
    local octave = (input >= 0.5 and 4) or 3
    -- Scale notes to octave
    for i, v in ipairs(notes) do
      notes[i] = v * 2 ^ octave
    end
    -- Calc index of note    
    local step
    if(input >= 0.5) then
      step = lerp(1, 8, (input - 0.5) / 0.5)
    else
      step = lerp(1, 8, input / 0.5)
    end
    step = math.floor(step)
    freq = notes[keyNotes[step]]
    return oscL.sinewave(freq);
  else
    -- Convert input to 0 - 1
    local input = rightTrigger / 255
    -- Get octave of note by dividing input in half
    local octave = (input >= 0.5 and 5) or 4
    -- Scale notes to octave
    for i, v in ipairs(notes) do
      notes[i] = v * 2 ^ octave
    end
    -- Calc index of note    
    local step
    if(input >= 0.5) then
      step = lerp(1, 8, (input - 0.5) / 0.5)
    else
      step = lerp(1, 8, input / 0.5)
    end
    step = math.floor(step)
    freq = notes[keyNotes[step]]
    return oscR.sinewave(freq);
  end
end

local maximua
local luaXinput
local oscL
local oscR
local state, leftTrigger, rightTrigger, leftX, leftY, rightX, rightY