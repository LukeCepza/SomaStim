-- this function is called when the box is initialized
function initialize(box)
	io.write("initialize has been called\n");
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua") 
end

local vibracion 	= {33032, 33033, 33034, 33035}	--tags corresponding to vibration
local stim_length 	= 0.5				--duration of stimulation
local stim_rest_length 	= 4 				--duration of stimulation (3) and resting (5)
local st 		= 40				--number of stimuli
local stop_vib 		= 33042
local t_lag			= 20
local valid        = 33041                --flag to start the validation

-- this function is called when the box is uninitialized
function uninitialize(box)
	io.write("uninitialize has been called\n")
end

function process(box)
	io.write("process has been called\n")
	box:send_stimulation(1, valid, 5, 0)        --start validation process   
	box:log("Info", string.format('%i at %i', valid, 5))
         
		n=3
		for i=1, st, 1 do
		box:send_stimulation(1, vibracion[n], t_lag + (i-1)*stim_rest_length, 0)		--Start stimuli
		box:send_stimulation(1, stop_vib, t_lag + (i-1)*stim_rest_length+stim_length, 0) 	--Stop stimuli
		box:log("Info", string.format('%i at %i', vibracion[n], t_lag + (i-1)*stim_rest_length))
		box:log("Info", string.format('%i at %i', stop_vib, t_lag + (i-1)*stim_rest_length+stim_length))
		end

		box:send_stimulation(1,32770, t_lag + (st*8)+stim_length, 0)   --mandar estimulo para terminar
	
	-- releases cpu
	box:sleep()
end