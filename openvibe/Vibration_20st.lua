-- this function is called when the box is initialized
function initialize(box)
	io.write("initialize has been called\n");
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua")

end

local vibracion 	= {33032, 33033, 33034, 33035}	--tags corresponding to vibration
local stim_length 	= 3				--duration of stimulation
local stim_rest_length 	= 8 				--duration of stimulation (3) and resting (5)
local st 		= 20				--number of stimuli
local valid		= 33040				--flag to start the validation

-- this function is called when the box is uninitialized
function uninitialize(box)
	io.write("uninitialize has been called\n")
end

function process(box)
	io.write("process has been called\n")

	-- loop until box:keep_processing() returns zero
	-- cpu will be released with a call to sleep
	-- at the end of the loop
	while box:keep_processing() do
	            box:send_stimulation(1, valid, 1, 0)		--start validation process		    
		    t_lag = 20

 		    n=3
		    for i=1, st, 1 do
			box:send_stimulation(1, vibracion[n], t_lag + (i-1)*stim_rest_length, 0)		--Start stimuli
		        box:send_stimulation(1, stop_vib_air, t_lag + (i-1)*stim_rest_length+stim_length, 0) 	--Stop stimuli
			box:log("Info", string.format('%i at %i', vibracion[n], t_lag + (i-1)*stim_rest_length))
		    End

	       	    box:send_stimulation(1,32770, t_lag + (st*8)+stim_length, 0)   --mandar estimulo para terminar
		
		-- releases cpu
		box:sleep()

	end
end