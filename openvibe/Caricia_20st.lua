-- this function is called when the box is initialized
function initialize(box)
	io.write("initialize has been called\n");
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua")
	--box:log("Info", string.format('begin'))
end

local stim_rest_length = 11 --duration of resting + stimulation
local tense_length = stim_rest_length -6
local stim_length = stim_rest_length	--duration of stimulation

local caricia 		= {33024, 33025, 33026, 33027}
local caricia_tense 	= {33036, 33037, 33038, 33039}
--local stop_air 		=  33043
local st  = 20	-- estimulos
local t_lag = 20

--es el numero de veces que se ha hecho el threshold.
-- this function is called when the box is uninitialized
function uninitialize(box)
	io.write("uninitialize has been calle d\n")
end

function process(box)
	io.write("process has been called\n")
         
		n=3
		for i=1, st, 1 do
			box:send_stimulation(1,caricia_tense[n], t_lag + (i-1)*stim_rest_length -6, 0)   --Start tense
			box:send_stimulation(1,caricia[n], t_lag + (i-1)*stim_length, 0)   --Start tense
			box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + (i-1)*stim_rest_length-6))
			box:log("Info", string.format('%i at %i',caricia[n], t_lag + (i-1)*stim_length))
		end	
			box:send_stimulation(1,32770, t_lag+11*stim_rest_length+stim_length+5, 0)   --mandar estimulo para terminar
	
	-- releases cpu
	box:sleep()
end