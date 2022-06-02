import sys; sys.path.append('./pylsl')
from pylsl import StreamInlet, resolve_stream, StreamInfo, StreamOutlet
import time
import serial
import keyboard
lastTime=0
arduino=serial.Serial('COM15',baudrate=500000)
try:
    arduino.close()
except:
    print("Port alread closed")
arduino.open()

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
            data = str(marker[0]).encode('utf-8')
            bytest = arduino.write(str(marker[0]).encode('utf-8'))
        print(marker[0], "at t = ", timestamp)
        print("se envio", data, "bytes ; Etiqueta", data)
        time.sleep(0.01)

    if marker == 32770:
        print('Program finised')
        try:
            arduino.close()
        except:
            print("Port closed already") 
        break
