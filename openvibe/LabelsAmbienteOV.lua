function initialize(box)
	dofile(box:get_config("${Path_Data}") .. "/plugins/stimulation/lua-stimulator-stim-codes.lua")
end
    local numEstim={33024,33025,33026} --Colocar codigo de etiquetas
	local t=1
function process(box)
	-- manages trials
    t=05
    box:send_stimulation(1,numEstim[1], t, 0)   --mandar estimulo
    t=10
    box:send_stimulation(1,numEstim[2], t, 0)   --mandar estimulo
    t=15
    box:send_stimulation(1,numEstim[3], t, 0)   --mandar estimulo
end