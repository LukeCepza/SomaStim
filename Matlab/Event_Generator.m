Reps = 30; %Repeticiones
rest_avg = 2; %duracion del estimulo
rest_std = 0; %desviacion estandar 
stim_dur = 1; %Duracion del estimulo
wait_init = 15; %
label_act = "33024"
label_stop = "33043"

fileID = fopen('Event_ThesisKevin.txt','w');
rng('default');
s = rng;
rand_onset = normrnd(rest_avg,rest_std/3,Reps,1);
epoch_total_dur = rand_onset + stim_dur;
stop_stim = wait_init + cumsum(epoch_total_dur)-epoch_total_dur+stim_dur;
start_stim = stop_stim - stim_dur;

disp("Max duration would be " + (wait_init+(rest_avg+rest_std+stim_dur)*Reps) +" seconds or " ...
    + (rest_avg+rest_std)*Reps/60 + " minutes" )

%fprintf(fileID,'%s\n',"box:send_stimulation(1, 33041, "  + wait_init +", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_act + ", " + start_stim +", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, "+label_stop+", " + stop_stim + ", 0)");
%fprintf(fileID,'%s\n',"box:send_stimulation(1, 32770, " + (wait_init+(rest_avg+rest_std+stim_dur)*(Reps)) + ", 0)");


prev = wait_init+(rest_avg+rest_std+stim_dur)*Reps
wait_init = prev; %
label_act = "33025"
label_stop = "33043"

rng('default');
s = rng;
rand_onset = normrnd(rest_avg,rest_std/3,Reps,1);
epoch_total_dur = rand_onset + stim_dur;
stop_stim = wait_init + cumsum(epoch_total_dur)-epoch_total_dur+stim_dur;
start_stim = stop_stim - stim_dur;

disp("Max duration would be " + (wait_init+(rest_avg+rest_std+stim_dur)*Reps) +" seconds or " ...
    + (rest_avg+rest_std)*Reps/60 + " minutes" )

%fprintf(fileID,'%s\n',"box:send_stimulation(1, 33041, "  + wait_init +", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_act + ", " + start_stim +", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, "+label_stop+", " + stop_stim + ", 0)");
%fprintf(fileID,'%s\n',"box:send_stimulation(1, 32770, " + (wait_init+(rest_avg+rest_std+stim_dur)*(Reps)) + ", 0)");

prev = wait_init+(rest_avg+rest_std+stim_dur)*Reps
wait_init = prev; %
label_act = "33026"
label_stop = "33043"

rng('default');
s = rng;
rand_onset = normrnd(rest_avg,rest_std/3,Reps,1);
epoch_total_dur = rand_onset + stim_dur;
stop_stim = wait_init + cumsum(epoch_total_dur)-epoch_total_dur+stim_dur;
start_stim = stop_stim - stim_dur;

disp("Max duration would be " + (wait_init+(rest_avg+rest_std+stim_dur)*Reps) +" seconds or " ...
    + (rest_avg+rest_std)*Reps/60 + " minutes" )

%fprintf(fileID,'%s\n',"box:send_stimulation(1, 33041, "  + wait_init +", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_act + ", " + start_stim +", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, "+label_stop+", " + stop_stim + ", 0)");
%fprintf(fileID,'%s\n',"box:send_stimulation(1, 32770, " + (wait_init+(rest_avg+rest_std+stim_dur)*(Reps)) + ", 0)");

prev = wait_init+(rest_avg+rest_std+stim_dur)*Reps
wait_init = prev; %
label_act = "33027"
label_stop = "33043"

rng('default');
s = rng;
rand_onset = normrnd(rest_avg,rest_std/3,Reps,1);
epoch_total_dur = rand_onset + stim_dur;
stop_stim = wait_init + cumsum(epoch_total_dur)-epoch_total_dur+stim_dur;
start_stim = stop_stim - stim_dur;

disp("Max duration would be " + (wait_init+(rest_avg+rest_std+stim_dur)*Reps) +" seconds or " ...
    + (rest_avg+rest_std)*Reps/60 + " minutes" )

%fprintf(fileID,'%s\n',"box:send_stimulation(1, 33041, "  + wait_init +", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, " + label_act + ", " + start_stim +", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, "+label_stop+", " + stop_stim + ", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, 32770, " + (wait_init+(rest_avg+rest_std+stim_dur)*(Reps)) + ", 0)");
fprintf(fileID,'%s\n',"box:send_stimulation(1, 32770, " + (wait_init+(rest_avg+rest_std+stim_dur)*(Reps)+2) + ", 0)");

fclose(fileID);


