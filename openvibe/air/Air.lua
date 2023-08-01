-- this function is called when the box is initialized
function initialize(box)
	io.write("initialize has been called\n");
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua")
	box:log("Info", string.format('begin'))
end
local stim_length = 5	--duration of stimulation
local stim_rest_length = 30 --duration of resting + stimulation

local aire 			= {33028, 33029, 33030, 33031}
local stop_air_th 	=  33062
local stop_air 		=  33043
local begin_air_th 	= 33065

local wait = false
local do_once_begin_thrs = true
local received_thrshold = false
local do_thrs = true
local th_times = 0 --es el numero de veces que se ha hecho el threshold.
-- this function is called when the box is uninitialized
function uninitialize(box)
	io.write("uninitialize has been called\n")
	box:log("Info", string.format('finished at %i',box:get_current_time()))
	box:send_stimulation(1,32770,box:get_current_time()+0.1,0)
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

				if (identifier == 0x00008129) and (do_once_begin_thrs) then 
					box:log("Info", string.format('begin thds %i at %i',0x00008129,t))
					box:send_stimulation(1,0x00008129,t+0.01,0)
					Do_th = true
					received_thrshold = false
					do_once_begin_thrs = false --una vez que se ejecuta ya no se vuelve a ejecutar nunca
				elseif (identifier == 0x00008126) then
					received_thrshold = true
				end
				box:remove_stimulation(input, 1)
			end
		end

		if (Do_th) then --Si se dio la orden de hacer threshold 
					
			if (not wait) then --Comenzar el threshold
				box:log("Info", string.format('begin thds %i at %i',0x00008129,t))

				box:send_stimulation(1,begin_air_th,t,0)
				t_lag = box:get_current_time() --t_lag es el tiempo del ultimo estimulo
				wait = true
				do_thrs = true
			elseif (t >= t_lag+50) then --25 --Queremos que se ejecute 20 segundos despues de que comienza el threshold
				box:log("Info", string.format('exceded time'))
				th_times = th_times + 1
				wait = false
			elseif (received_thrshold) and (do_thrs) then --Si recibe estimulo de threshold lo envia y sigue esperando los 20 segundos
				box:log("Info", string.format('THDS %i at %i',0x00008126,t))
				box:send_stimulation(1,stop_air_th,t,0)
				received_thrshold = false
				do_thrs = false
			else
				received_thrshold = false;
			end
			if (th_times >= 3) then --Si se hace 3 veces, salir de rutina de threshold y hacer experimento
				box:log("Info", string.format('FINISHED THDS'))
				Do_th = false
				Do_experiment = true
				t_lag = box:get_current_time() --t_lag es el tiempo del ultimo estimulo
			end
		end

		if (Do_experiment) then

			t_lag = t_lag + 120 --t_lag varies depending on the threshold measurement
			n=1
			box:send_stimulation(1,aire[n], t_lag + 0*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 0*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 0*stim_rest_length))
		    box:send_stimulation(1,aire[n], t_lag + 1*stim_rest_length, 0)   --Start air
			box:send_stimulation(1,stop_air, t_lag + 1*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 1*stim_rest_length))
			box:send_stimulation(1,aire[n], t_lag + 2*stim_rest_length, 0)   --Start air
			box:send_stimulation(1,stop_air, t_lag + 2*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 2*stim_rest_length))
		    n=2
			box:send_stimulation(1,aire[n], t_lag + 3*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 3*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 3*stim_rest_length))
		    box:send_stimulation(1,aire[n], t_lag + 4*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 4*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 4*stim_rest_length))
			box:send_stimulation(1,aire[n], t_lag + 5*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 5*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 5*stim_rest_length))
		    n=3
			box:send_stimulation(1,aire[n], t_lag + 6*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 6*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 6*stim_rest_length))
		    box:send_stimulation(1,aire[n], t_lag + 7*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 7*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 7*stim_rest_length))
			box:send_stimulation(1,aire[n], t_lag + 8*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 8*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 8*stim_rest_length))
		    n=4
			box:send_stimulation(1,aire[n], t_lag + 9*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 9*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 9*stim_rest_length))
		    box:send_stimulation(1,aire[n], t_lag + 10*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 10*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 10*stim_rest_length))
			box:send_stimulation(1,aire[n], t_lag + 11*stim_rest_length, 0)   --Start air
		    box:send_stimulation(1,stop_air, t_lag + 11*stim_rest_length+stim_length, 0)   --Stop air
			box:log("Info", string.format('%i at %i',aire[n], t_lag + 11*stim_rest_length))

			Do_experiment =  false
			box:send_stimulation(1,32770, t_lag+11*stim_rest_length+stim_length+5, 0)   --mandar estimulo para terminar
		end
		-- releases cpu
		box:sleep()

	end
end


