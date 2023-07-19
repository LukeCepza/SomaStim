-- this function is called when the box is initialized
function initialize(box)
	io.write("initialize has been called\n");
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua")
	box:log("Info", string.format('begin'))
end

local stim_rest_length = 31 --duration of resting + stimulation
local tense_length = stim_rest_length - 6
local stim_length = stim_rest_length	--duration of stimulation

local caricia 			= {33024, 33025, 33026, 33027}
local caricia_tense = 	{33036, 33037, 33038, 33039}
local stop_air 		=  33043

local wait = false
local do_once_begin_thrs = true
local received_thrshold = false
local do_thrs = true
--es el numero de veces que se ha hecho el threshold.
-- this function is called when the box is uninitialized
function uninitialize(box)
	io.write("uninitialize has been calle d\n")
	box:log("Info", string.format('finished at %i',box:get_current_time()))
	box:send_stimulation(1,32770,box:get_current_time()+0.1,0)
end

function process(box)
t_lag = 0 --t_lag static
n=1
box:send_stimulation(1,caricia_tense[n], t_lag + 0*stim_rest_length -6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 0*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 0*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 0*stim_length))

box:send_stimulation(1,caricia_tense[n], t_lag + 1*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 1*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 1*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 1*stim_length))

box:send_stimulation(1,caricia_tense[n], t_lag + 2*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 0*stim_length, 2)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 2*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 2*stim_length))

n=2
box:send_stimulation(1,caricia_tense[n], t_lag + 3*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 3*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 3*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 3*stim_length))

box:send_stimulation(1,caricia_tense[n], t_lag + 4*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 4*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 4*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 4*stim_length))

box:send_stimulation(1,caricia_tense[n], t_lag + 5*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 5*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 5*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 5*stim_length))
n=3
box:send_stimulation(1,caricia_tense[n], t_lag + 6*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 6*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 6*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 6*stim_length))

box:send_stimulation(1,caricia_tense[n], t_lag + 7*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 7*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 7*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 7*stim_length))

box:send_stimulation(1,caricia_tense[n], t_lag + 8*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 8*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 8*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 8*stim_length))
n=4
box:send_stimulation(1,caricia_tense[n], t_lag + 9*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 9*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 9*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 9*stim_length))

box:send_stimulation(1,caricia_tense[n], t_lag + 10*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 10*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 10*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 10*stim_length))

box:send_stimulation(1,caricia_tense[n], t_lag + 11*stim_rest_length-6, 0)   --Start tense
box:send_stimulation(1,caricia[n], t_lag + 11*stim_length, 0)   --Start tense
box:log("Info", string.format('%i at %i',caricia_tense[n], t_lag + 11*stim_rest_length-6))
box:log("Info", string.format('%i at %i',caricia[n], t_lag + 11*stim_length))

box:send_stimulation(1,32770, t_lag+11*stim_rest_length+stim_length+5, 0)   --mandar estimulo para terminar
end