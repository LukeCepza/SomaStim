stim_dur_air = 2; %Duracion del estimulo
srim_rest_air = 2;
wait_initino = 15; %

posible_combs = perms(["33028","33029","33030","33031"])
label_stop = "33043"
feedback = "2000"
% Mix rows
combs_shuffle= posible_combs(randperm(size(posible_combs,1)),:)

epochs_diff = 0:24:470
times = [0,2,4,6,8,10,12,14,16]

fileID = fopen('Events_KevinThesis.txt','w');
rng('default');
s = rng;

for i = 0:19
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + combs_shuffle(i+1,1)+ ", " + (0 + epochs_diff*i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_stop + ", " + (2 + epochs_diff*i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + combs_shuffle(i+1,2)+ ", " + (4 + epochs_diff*i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_stop + ", " + (6 + epochs_diff*i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + combs_shuffle(i+1,3)+ ", " + (8 + epochs_diff*i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_stop + ", " + (10 + epochs_diff*i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + combs_shuffle(i+1,4)+ ", " + (12 + epochs_diff*i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_stop + ", " + (14 + epochs_diff*i) +", 0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1, " + "33045" + ", " + (0 + epochs_diff*i) +", 0)");

end


