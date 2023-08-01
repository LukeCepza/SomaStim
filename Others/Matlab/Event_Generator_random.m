stim_dur = 2; %Duracion del estimulo
wait_init = 15; %

% Define your list of strings
combs = [nchoosek(flip(["33028","33029","33030","33031"]),2);...
    nchoosek(["33028","33029","33030","33031"],2)];
label_stop = "33043"

combs_rep = repelem(combs,2,1)
indexArray = randperm(length(combs_rep))
permutedList = combs_rep(indexArray,:)
fileID = fopen('Events_air.txt','w');
rng('default');
s = rng;

stim1 = wait_init+(0:12:12*max(length(combs_rep)));
stim1_off = stim1+stim_dur
stim2 = stim1+4;
stim2_off = stim2+stim_dur
feedback = stim1+6

for i = 1:length(combs_rep)
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + permutedList(i,1)+ ", " + stim1(i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_stop + ", " + stim1_off(i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + permutedList(i,2)+ ", " + stim2(i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_stop + ", " + stim2_off(i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + "33045" + ", " + feedback(i) +", 0)");
end