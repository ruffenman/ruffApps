function getSample ()
  local output = math.sin(phase * (2 * math.pi))
	if ( phase >= 1.0 ) then
    phase = phase - 1.0
  end
	phase = phase + (1 / (44100 / 440))
	return output
end

phase = 0