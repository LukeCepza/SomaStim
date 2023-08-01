[x,fs]=audioread('D:\Neuro\SomaStim\openvibe\Air\1kCalTone94.wav');
    d = designfilt('bandpassiir','FilterOrder',6,...
         'HalfPowerFrequency1',31.8,...
         'HalfPowerFrequency2',8000,...
         'SampleRate', fs,'DesignMethod','butter');
x=filtfilt(d,x);
N=length(x);
t=N/fs;
T=linspace(0,t,N);
T2=buffer(T,0.125*fs,0);
T2(3889:end,92)=nan;
T3=[T2(end,1:91) T2(3888,92)];   
y=buffer(x,0.125*fs,0);
z=rms(y,'omitnan');
v2=median(z);
v1=v2./(10.^(94/20));

[x1,fs]=audioread('D:\Neuro\SomaStim\openvibe\Air\SPLmesNEclean.wav');
x1=filtfilt(d,x1);
y1=buffer(x1,0.125*fs,0);
N=length(x1);
t=N/fs;
T=linspace(0,t,N)+1;
T2=buffer(T,0.125*fs,0);
T2(625:end,3495)=nan;
T3=[T2(end,1:3944) T2(624,3945)];
z1=rms(y1,'omitnan');
Z1=20*log10(z1/v1);

%%
EEG = pop_biosig('D:\Neuro\SomaStim\openvibe\Air\VibVal-[2023.05.26-12.59.06].gdf');
EEG = pop_resample(EEG, 8);
EEG.data(EEG.event(2).latency:(EEG.event(2).latency+length(Z1)-1)) = Z1;
EEG = pop_epoch( EEG, {  '33028',  '33029', ...
   '33030',  '33031'  }, ...
   [-.1 4],'epochinfo', 'yes');
a = [[0 0 1];[1 0 0];[0 1 0];[0 0 0]]
b = ["1";"2";"3";"4"]
t = EEG.times
figure()
subplot(3,1,3)
for stim = 1:4
    ACC_env = squeeze(EEG.data(:,:,(1+30*(stim-1)):30*stim));
    ACC_env_mean = mean(ACC_env,2);
    hold on
    plot(t,ACC_env_mean, LineWidth=1.5,Color=a(stim,:),DisplayName=b(stim));
end
xlim([-100,4000])
ylim([0 75])
%legend()

text(4200,37.5,"(c)")

xlabel("Time (ms)")
title("Air")
ylabel("dB SPL")
xline(0,Color=[0.7 0.2 0.2],LineWidth=1.5,LineStyle="--")
xline(2000,Color=[0.5 0 1],LineWidth=1.5,LineStyle="--")

%%
EEG = pop_biosig('D:\Neuro\SomaStim\openvibe\Validation\VibVal-[2023.05.19-14.06.27].edf')
EEG = pop_select( EEG, 'channel',{'Channel 9','Channel 10','Channel 11'});
EEG = pop_epoch( EEG, {  'OVTK_StimulationId_Label_12',  'OVTK_StimulationId_Label_13', ...
   'OVTK_StimulationId_Label_14',  'OVTK_StimulationId_Label_15'  }, ...
   [-1 4],'epochinfo', 'yes');
EEG = pop_rmbase(EEG, [-1000 0] ,[]);  
t = EEG.times
subplot(3,1,1)
set(gcf,'color','w');
%tiledlayout(1,1);
%subplot(4,1,1:4)
ylabel("Amplitude (mV)")
%nexttile
a = [[0 0 1];[1 0 0];[0 1 0];[0 0 0]]
b = ["1";"2";"3";"4"]
for stim = 1:4  
    %subplot(4,1,stim)
    ACC = squeeze(sum(abs(EEG.data(:,:,(1+30*(stim-1)):30*stim)),1));
    ACC_std = std(ACC,1,2);
    ACC_mean= mean(ACC,2);
    hold on
    plot(t,ACC_mean, LineWidth=1.5,Color=a(stim,:),DisplayName=b(stim))
    xlim([-100,4000])
    ylim([0,1.7])
end
text(1,1.8,"Onset",Color=[0.7 0.2 0.2])
text(501,1.8,"Offset",Color=[0.5 0 1])

text(3800,2,"Intensity")

text(4200,0.9,"(a)")

xline(0,Color=[0.7 0.2 0.2],LineWidth=1.5,LineStyle="--")
xline(500,Color=[0.5 0 1],LineWidth=1.5,LineStyle="--")

legend("1","2","3","4")
title("Vibration")

%%
EEG = pop_biosig('D:\sharegit\SomaStim\openvibe\caress\Validation\BuenosRegistros\caressVal-record-[2023.06.09-21.01.46].edf')
EEG = pop_select( EEG, 'channel',{'Channel 9','Channel 10','Channel 11'});
EEG = pop_epoch( EEG, {  'OVTK_StimulationId_Label_00',  'OVTK_StimulationId_Label_01', ...
   'OVTK_StimulationId_Label_02',  'OVTK_StimulationId_Label_03'  }, ...
   [-1 4],'epochinfo', 'yes');
EEG = pop_rmbase(EEG, [-1000 0] ,[]);  
t = EEG.times
%
subplot(3,1,2)
a = [[0 0 1];[1 0 0];[0 1 0];[0 0 0]]
b = ["1";"2";"3";"4"]
for stim = 1:4  
    %subplot(4,1,stim)
    ACC = squeeze(sum(abs(EEG.data(:,:,(1+30*(stim-1)):30*stim)),1));
    ACC_std = std(ACC,1,2);
    ACC_mean= mean(ACC,2);
    hold on
    plot(t,ACC_mean,LineWidth=1.5,Color=a(stim,:),DisplayName=b(stim))
    %plot(t,ACC_mean-ACC_std)
    %plot(t,ACC_mean+ACC_std)
    %title("Magnitude of summed amplitude in X, Y, and Z for stim "+stim)
    ylim([0,0.05])
    xlim([-100,4000])
end
text(4200,0.025,"(b)")

xline(0,Color=[0.7 0.2 0.2],LineWidth=1.5,LineStyle="--")
xline(1000,Color=[0.5 0 1],LineWidth=1.5,LineStyle="--")
title("Touch")
ylabel("Amplitude (mV)")

%legend()