rng('default');
s = rng;

stim_dur_air = 2; %Duracion del estimulo
srim_rest_air = 2;
wait_initino = 15; %

%% Air
posible_combs = perms(["33028","33029","33030","33031"])
label_stop = "33043"
feedback = "2000"
% Mix rows
combs_shuffle= posible_combs(randperm(size(posible_combs,1)),:)

epochs_diff = (0:24:480)+15
times = [0,2, 4,6, 8,10, 12,14, 16]

fileID = fopen('Events_KevinThesis.txt','w');

for i = 0:19
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,1)+ "," + num2str(0 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(2 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,2)+ "," + num2str(4 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(6 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,3)+ "," + num2str(8 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(10 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,4)+ "," + num2str(12 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(14 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + feedback + "," + num2str(16 + epochs_diff(i+1)) +",0)");
end

%% Vibration
posible_combs = perms(["33032","33033","33034","33035"])
label_stop = "33042"
feedback = "2000"
% Mix rows
combs_shuffle= posible_combs(randperm(size(posible_combs,1)),:)
epochs_diff = (480:24:960)+15

for i = 0:19
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,1)+ "," + num2str(0 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(2 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,2)+ "," + num2str(4 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(6 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,3)+ "," + num2str(8 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(10 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,4)+ "," + num2str(12 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(14 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + feedback + "," + num2str(16 + epochs_diff(i+1)) +",0)");
end

%% Caress
posible_combs = perms(["33024","33025","33026","33027"])
label_stop = "2001"
feedback = "2000"
% Mix rows
combs_shuffle= posible_combs(randperm(size(posible_combs,1)),:)

epochs_diff = (960:24:1440)+15

for i = 0:19
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,1)+ "," + num2str(0 + epochs_diff(i+1)) +",0)");
    %fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(2 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,2)+ "," + num2str(4 + epochs_diff(i+1)) +",0)");
    %fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(6 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,3)+ "," + num2str(8 + epochs_diff(i+1)) +",0)");
    %fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(10 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + combs_shuffle(i+1,4)+ "," + num2str(12 + epochs_diff(i+1)) +",0)");
    %fprintf(fileID,'%s\n',"box:send_stimulation(1," + label_stop + "," + num2str(14 + epochs_diff(i+1)) +",0)");
    fprintf(fileID,'%s\n',"box:send_stimulation(1," + feedback + "," + num2str(16 + epochs_diff(i+1)) +",0)");
end
