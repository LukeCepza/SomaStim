-- this function is called when the box is initialized
function initialize(box)
end

-- this function is called when the box is uninitialized
function uninitialize(box)
	io.write("uninitialize has been called\n")
end

function process(box)
	io.write("process has been called\n")
	box:send_stimulation(1,33084,2,0)	--Start LSL communication
		
	-- stimuli labels
	-- 33028, air 1/4
	-- 33029, air 2/4
	-- 33030, air 3/4
	-- 33031, air 4/4
	-- 33043, stop stimulus
	-- 33045, lsl feedback
	
	-- programming stimuli paradigm in here
	box:send_stimulation(1,33029,15,0)
	box:send_stimulation(1,33043,17,0)
	box:send_stimulation(1,33031,19,0)
	box:send_stimulation(1,33043,21,0)
	box:send_stimulation(1,33045,21.5,0)

	box:send_stimulation(1,33031,27,0)
	box:send_stimulation(1,33043,29,0)
	box:send_stimulation(1,33028,31,0)
	box:send_stimulation(1,33043,33,0)
	box:send_stimulation(1,33045,33.5,0)

	box:send_stimulation(1,33031,39,0)
	box:send_stimulation(1,33043,41,0)
	box:send_stimulation(1,33029,43,0)
	box:send_stimulation(1,33043,45,0)
	box:send_stimulation(1,33045,45.5,0)

	box:send_stimulation(1,33028,51,0)
	box:send_stimulation(1,33043,53,0)
	box:send_stimulation(1,33030,55,0)
	box:send_stimulation(1,33043,57,0)
	box:send_stimulation(1,33045,57.5,0)

	box:send_stimulation(1,33029,63,0)
	box:send_stimulation(1,33043,65,0)
	box:send_stimulation(1,33028,67,0)
	box:send_stimulation(1,33043,69,0)
	box:send_stimulation(1,33045,69.5,0)

	box:send_stimulation(1,33030,75,0)
	box:send_stimulation(1,33043,77,0)
	box:send_stimulation(1,33029,79,0)
	box:send_stimulation(1,33043,81,0)
	box:send_stimulation(1,33045,81.5,0)

	box:send_stimulation(1,33028,87,0)
	box:send_stimulation(1,33043,89,0)
	box:send_stimulation(1,33031,91,0)
	box:send_stimulation(1,33043,93,0)
	box:send_stimulation(1,33045,93.5,0)

	box:send_stimulation(1,33028,99,0)
	box:send_stimulation(1,33043,101,0)
	box:send_stimulation(1,33029,103,0)
	box:send_stimulation(1,33043,105,0)
	box:send_stimulation(1,33045,105.5,0)

	box:send_stimulation(1,33030,111,0)
	box:send_stimulation(1,33043,113,0)
	box:send_stimulation(1,33029,115,0)
	box:send_stimulation(1,33043,117,0)
	box:send_stimulation(1,33045,117.5,0)

	box:send_stimulation(1,33031,123,0)
	box:send_stimulation(1,33043,125,0)
	box:send_stimulation(1,33028,127,0)
	box:send_stimulation(1,33043,129,0)
	box:send_stimulation(1,33045,129.5,0)

	box:send_stimulation(1,33029,135,0)
	box:send_stimulation(1,33043,137,0)
	box:send_stimulation(1,33031,139,0)
	box:send_stimulation(1,33043,141,0)
	box:send_stimulation(1,33045,141.5,0)

	box:send_stimulation(1,33029,147,0)
	box:send_stimulation(1,33043,149,0)
	box:send_stimulation(1,33030,151,0)
	box:send_stimulation(1,33043,153,0)
	box:send_stimulation(1,33045,153.5,0)

	box:send_stimulation(1,33028,159,0)
	box:send_stimulation(1,33043,161,0)
	box:send_stimulation(1,33030,163,0)
	box:send_stimulation(1,33043,165,0)
	box:send_stimulation(1,33045,165.5,0)

	box:send_stimulation(1,33031,171,0)
	box:send_stimulation(1,33043,173,0)
	box:send_stimulation(1,33030,175,0)
	box:send_stimulation(1,33043,177,0)
	box:send_stimulation(1,33045,177.5,0)

	box:send_stimulation(1,33030,183,0)
	box:send_stimulation(1,33043,185,0)
	box:send_stimulation(1,33031,187,0)
	box:send_stimulation(1,33043,189,0)
	box:send_stimulation(1,33045,189.5,0)

	box:send_stimulation(1,33031,195,0)
	box:send_stimulation(1,33043,197,0)
	box:send_stimulation(1,33030,199,0)
	box:send_stimulation(1,33043,201,0)
	box:send_stimulation(1,33045,201.5,0)

	box:send_stimulation(1,33031,207,0)
	box:send_stimulation(1,33043,209,0)
	box:send_stimulation(1,33029,211,0)
	box:send_stimulation(1,33043,213,0)
	box:send_stimulation(1,33045,213.5,0)

	box:send_stimulation(1,33028,219,0)
	box:send_stimulation(1,33043,221,0)
	box:send_stimulation(1,33031,223,0)
	box:send_stimulation(1,33043,225,0)
	box:send_stimulation(1,33045,225.5,0)

	box:send_stimulation(1,33030,231,0)
	box:send_stimulation(1,33043,233,0)
	box:send_stimulation(1,33031,235,0)
	box:send_stimulation(1,33043,237,0)
	box:send_stimulation(1,33045,237.5,0)

	box:send_stimulation(1,33028,243,0)
	box:send_stimulation(1,33043,245,0)
	box:send_stimulation(1,33029,247,0)
	box:send_stimulation(1,33043,249,0)
	box:send_stimulation(1,33045,249.5,0)

	box:send_stimulation(1,33030,255,0)
	box:send_stimulation(1,33043,257,0)
	box:send_stimulation(1,33028,259,0)
	box:send_stimulation(1,33043,261,0)
	box:send_stimulation(1,33045,261.5,0)

	box:send_stimulation(1,33029,267,0)
	box:send_stimulation(1,33043,269,0)
	box:send_stimulation(1,33030,271,0)
	box:send_stimulation(1,33043,273,0)
	box:send_stimulation(1,33045,273.5,0)

	box:send_stimulation(1,33030,279,0)
	box:send_stimulation(1,33043,281,0)
	box:send_stimulation(1,33028,283,0)
	box:send_stimulation(1,33043,285,0)
	box:send_stimulation(1,33045,285.5,0)

	box:send_stimulation(1,33029,291,0)
	box:send_stimulation(1,33043,293,0)
	box:send_stimulation(1,33028,295,0)
	box:send_stimulation(1,33043,297,0)
	box:send_stimulation(1,33045,297.5,0)

	box:send_stimulation(1, 32770, 303, 0)	-- End paradigm
	box:send_stimulation(1, 32770, 305, 0) 	
	
	box:sleep()				-- releases cpu
	io.write("process has been called\n")

end