	
function initialize(box) 
 	box:log("Info", "initialize has been called")
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua")
end
    
local numEstim={33024,33025,33026,33027} --Colocar codigo de etiquetas {2N,3N,4N,6N}
local t0={60, 248,436,624}
local n=1
local s=0

function process(box)
	
	box:send_stimulation(1,1408, 0, 0)   --mandar estimulo

	n=1
	s=0
	box:send_stimulation(1,numEstim[n], t0[n], 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	s=1
	box:send_stimulation(1,numEstim[n], t0[n] + s*31, 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	s=2	
	box:send_stimulation(1,numEstim[n], t0[n] + s*31, 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	
	n=2
	s=0
	box:send_stimulation(1,numEstim[n], t0[n], 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	s=1
	box:send_stimulation(1,numEstim[n], t0[n] + s*31, 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	s=2	
	box:send_stimulation(1,numEstim[n], t0[n] + s*31, 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	
	n=3
	s=0
	box:send_stimulation(1,numEstim[n], t0[n], 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	s=1
	box:send_stimulation(1,numEstim[n], t0[n] + s*31, 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	s=2	
	box:send_stimulation(1,numEstim[n], t0[n] + s*31, 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	
	n=4
	s=0
	box:send_stimulation(1,numEstim[n], t0[n], 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	s=1
	box:send_stimulation(1,numEstim[n], t0[n] + s*31, 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	s=2	
	box:send_stimulation(1,numEstim[n], t0[n] + s*31, 0)   --mandar estimulo
	box:log("Info", string.format('%i at %i',numEstim[n], t0[n] + s*31))
	
end


