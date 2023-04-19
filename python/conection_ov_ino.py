import sys; sys.path.append('./pylsl')
from pylsl import StreamInlet, resolve_bypred, StreamInfo, StreamOutlet
import time
import serial

if len(sys.argv) > 1:
    ArduinoCOM = sys.argv[1]
else:
    ArduinoCOM = 'COM3'
# Connect thorugh serial to arduino
arduinoCOM = False
while (not arduinoCOM): #Repeat until successful connection #Try if len(ArduinoCOM == 0)
    try:
        print("Attempting connection to Arduino.")
        arduino=serial.Serial(ArduinoCOM,baudrate=500000)
        arduino.close()
        print("Connection successful.")
        arduinoCOM = True

    except Exception as e:
        print("Attempting again in 2s")
        time.sleep(2)

# Open serial communication thorugh serial to arduino
try:
    arduino.open()
    print("Arduino communication port opened.")


except Exception as e:
    print(e)
    print("Could not open port or alreaady in use.")
    
time.sleep(3)
arduino.write(str(b'33083').encode('utf-8'))
time.sleep(3)

# Lookup for OpenVibe Marker data Stream
print("Looking for OpenVibe Marker Stream ...")
streams = resolve_bypred('name', 'openvibeMarkers',timeout=1,)
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
        try:
            arduino.close()
            exit()
        except:
            print("Port closed already") 
            exit()
        break


