import sys; sys.path.append('./pylsl')
from pylsl import StreamInlet, resolve_bypred, StreamInfo, StreamOutlet
import time
import serial

ArduinoCOM = str(sys.argv)

lastTime=0



try:
    print("Attempting connection to Arduino")
    arduino=serial.Serial('COM3',baudrate=500000)
except Exception as e:
    print(e)
    print("Unrecognized port")

try:
    arduino.close()
except:
    print("Closing aruino instance")

try:
    arduino.open()
except Exception as e:
    print(e)
    print("Could not oppen port or already in use")

print("Looking for OpenVibe Marker Stream...")
streams = resolve_bypred('name', 'openvibeMarkers',timeout=1)
while(len(streams) == 0):
    print("OpenVibe Connection failed. Attempting again ...")
    time.sleep(2)  
    streams = resolve_bypred('name', 'openvibeMarkers',timeout=1)
      
print("OpenVibe Connection Stablished :D")

inlet = StreamInlet(streams[0])

while True:
    marker, timestamp = inlet.pull_sample()
    if marker!=0:
        for i in range(1):
            data = str(marker[0]).encode('utf-8')
            bytest = arduino.write(str(marker[0]).encode('utf-8'))
        print(marker[0], "at t = ", timestamp)
        print("Sent ", data, "; Marker is ", data)
        time.sleep(0.01)

    if marker == 32770:
        print('Program finished. Closing Arduino port')
        print('Closing Arduino port')
        try:
            arduino.close()
        except:
            print("Port closed already") 
            exit()
        break
