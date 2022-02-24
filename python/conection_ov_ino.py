import sys; sys.path.append('./pylsl')
from pylsl import StreamInlet, resolve_stream, StreamInfo, StreamOutlet
import time
import serial
import keyboard
lastTime=0
arduino=serial.Serial('COM6',baudrate=115200)

time.sleep(1.8)

streams = resolve_stream('name', 'openvibeMarkers')
if len(streams) == 0:
    print("Cant find markers stream")
else: 
    print("Markers")
inlet = StreamInlet(streams[0])

while True:
    marker, timestamp = inlet.pull_sample()
    if marker!=0:
        for i in range(1):
            bytest = arduino.write(str(marker[0]).encode('utf-8'))
        print(marker[0], "at t = ", timestamp)
        print("se envio", bytest)
        time.sleep(0.01)

    try:  # used try so that if user pressed other than the given key error will not be shown
        if keyboard.is_pressed('q'):  # if key 'q' is pressed 
          break  # finishing the loop
    except:
        break  # if user pressed a key other than the given key the loop will break
#arduino.close()