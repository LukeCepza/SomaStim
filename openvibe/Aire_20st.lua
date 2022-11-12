-- this function is called when the box is initialized
function initialize(box)
	io.write("initialize has been called\n");
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua")

end

local aire 			= {33028, 33029, 33030, 33031}
local stim_length 	= 3				--duration of stimulation
local stim_rest_length 	= 8 				--duration of stimulation (3) and resting (5)
local st 		    = 20				--number of stimuli
local stop_air 		=  33043
local t_lag			= 20

-- this function is called when the box is uninitialized
function uninitialize(box)
	io.write("uninitialize has been called\n")
end

function process(box)
	io.write("process has been called\n")
         
		n=3
		for i=1, st, 1 do
		box:send_stimulation(1, aire[n], t_lag + (i-1)*stim_rest_length, 0)		--Start stimuli
		box:send_stimulation(1, stop_air, t_lag + (i-1)*stim_rest_length+stim_length, 0) 	--Stop stimuli
		box:log("Info", string.format('%i at %i', aire[n], t_lag + (i-1)*stim_rest_length))
		box:log("Info", string.format('%i at %i', stop_air, t_lag + (i-1)*stim_rest_length+stim_length))
		end

		box:send_stimulation(1,32770, t_lag + (st*8)+stim_length, 0)   --mandar estimulo para terminar
	
	-- releases cpu
	box:sleep()
end