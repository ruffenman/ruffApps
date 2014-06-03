local maximua
local luaXinput
local osc1, osc2, osc3, osc4
local state, leftTrigger, rightTrigger, leftX, leftY, rightX, rightY
-- Chromatic notes (starting with C0)
local notes = { 16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.5, 29.14, 30.87 }

-- Index of chromatic note that defines the root of the key
local key = 10
-- Which octave is the root note in
local octave = 3
-- Define scale by indexing chromatic notes (as if in key of C)
local scale = { 1, 3, 5, 6, 8, 10, 12 } -- Major scale
-- Define voicing by indexing scale notes (as in normal chord charts, numbers are relative to root of chord)
local voicing = { 1, 3, 5, 7, 8, 10, 12, 14, 15 }

local outputL = 0
local outputR = 0

local function lerp(a, b, t)
  return a + (b - a) * t
end

local function getFreq(root, octave, step)
  local keyStep = root + step - 1
  while keyStep > #notes do
    keyStep = keyStep - #notes
    octave = octave + 1
  end
  while keyStep < 1 do
    keyStep = keyStep + #notes
    octave = octave - 1
  end
  return notes[keyStep] * 2 ^ octave
end

local function innerToOuterStep(inner, outer, step)
  local multiplier = 0
  while step > #inner do
    step = step - #inner
    multiplier = multiplier + 1
  end
  while step < 1 do
    step = step + #inner
    multiplier = multiplier - 1
  end
  return inner[step] + multiplier * #outer
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
  
  osc1 = maximua.oscNew();
  osc2 = maximua.oscNew();
  osc3 = maximua.oscNew();
  osc4 = maximua.oscNew();
    
  state, leftTrigger, rightTrigger, leftX, leftY, rightX, rightY = 0, 0, 0, 0, 0, 0, 0
  print('Initialized Lua state.')
end

function getSample(isLeft)
  local freq = 0;
  
  --[=[
  if(isLeft) then
    -- Only update input on left channel call (half the updates!)
    state, leftTrigger, rightTrigger, leftX, leftY, rightX, rightY = luaXinput.getState(0)
    
    -- Convert input to 0 - 1
    local input = leftTrigger / 255
    -- Calc step of note    
    local step = lerp(1, #voicing, input)
    step = math.floor(step)
    freq = getFreq(key, octave, innerToOuterStep(scale, notes, innerToOuterStep(voicing, scale, step)))
    return 0.5 * osc1.sinewave(freq);
  else
    -- Convert input to 0 - 1
    local input = rightTrigger / 255
    -- Calc step of note
    local step = lerp(1, #scale, input)
    step = math.floor(step)
    freq = getFreq(key, octave, innerToOuterStep(scale, notes, step))
    return 0.5 * osc2.sinewave(freq);
  end
  --]=]  
  
  ---[=[
  if(isLeft) then
    -- Only update on left channel call (half the updates!)
    state, leftTrigger, rightTrigger, leftX, leftY, rightX, rightY = luaXinput.getState(0)
    
    -- Convert input to 0 - 1
    local inputL = leftTrigger / 255
    local inputR = rightTrigger / 255
    -- Calc step of note    
    local scaleStep = lerp(1, #scale + 1, inputL)
    local voiceStep = lerp(1, #voicing, inputR)
    voiceStep = math.floor(voiceStep)
    scaleStep = math.floor(scaleStep)
    
    freq = getFreq(key, octave, innerToOuterStep(scale, notes, scaleStep))
    outputL = 0.5 * osc1.sinewave(freq)
    
    freq = getFreq(key, octave, innerToOuterStep(scale, notes, scaleStep - 1 + innerToOuterStep(voicing, scale, voiceStep)))
    outputR = 0.5 * osc2.sinewave(freq)
    
    return outputL
  else
    return outputR
  end
  --]=]
  
  --[=[
  if(isLeft) then
    local sample = osc1.sinewave(getFreq(key, octave, innerToOuterStep(scale, notes, voicing[1]))) 
      + osc2.sinewave(getFreq(key, octave, innerToOuterStep(scale, notes, voicing[2])))
      + osc3.sinewave(getFreq(key, octave, innerToOuterStep(scale, notes, voicing[3])))
      + osc4.sinewave(getFreq(key, octave, innerToOuterStep(scale, notes, voicing[4])))
    sample = sample * 0.125
    
    outputL = sample
  end
  return outputL
  --]=]
end