-- this function is called when the box is initialized
function initialize(box)
	io.write("initialize has been called\n");
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua")

end

local begin_th_vib_id = 0x00008128
local th_vib_id= 33061
local wait = false
local do_once_begin = true
local stop_all = false
local do_thrs = true
local th_times = 0 	--es el numero de veces que se ha hecho el threshold.

local vibracion = {33032, 33033, 33034, 33035}	--tags corresponding to vibration
local stop_vib_air =  33042
local stim_length = 3				--duration of stimulation
local stim_rest_length = 8 			--duration of stimulation (3) and resting (5)
local st = 20					--number of stimuli

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
		
		t = box:get_current_time()
		for input = 1, box:get_input_count() do
			for stimulation = 1, box:get_stimulation_count(input) do

				identifier, date, duration = box:get_stimulation(input, 1)

				if (identifier == 0x00008128) and (do_once_begin) then 
					box:log("Info", string.format('begin experiment %i at %i', 0x00008128, t))
					box:send_stimulation(1,0x00008128, t+0.01, 0)
					Do_experiment = true
					stop_all = false
					do_once_begin = false --una vez que se ejecuta ya no se vuelve a ejecutar nunca
				elseif (identifier == 0x00008125) then
					stop_all = true
				end
				box:remove_stimulation(input, 1)
			end
		end

		t_lag = box:get_current_time() --t_lag es el tiempo del ultimo estimulo

		if (Do_experiment) then

		    --t_lag = t_lag + 120 t_lag varies depending on the threshold measurement

 		    n=3
		    for i=1, st, 1 do
			box:send_stimulation(1, vibracion[n], t_lag + (i-1)*stim_rest_length, 0)		--Start stimuli
		        box:send_stimulation(1, stop_vib_air, t_lag + (i-1)*stim_rest_length+stim_length, 0) 	--Stop stimuli
			box:log("Info", string.format('%i at %i', vibracion[n], t_lag + (i-1)*stim_rest_length))
		    End

		    Do_experiment =  false
	       	    box:send_stimulation(1,32770, t_lag + (st*8)+stim_length, 0)   --mandar estimulo para terminar
		end
		
		-- releases cpu
		box:sleep()

	end
end



