import serial #you have to have PySerial installed for this
            #pip install pySerial
port = "COM3" #Serial port. COM3 for windows on my computer. https://en.wikipedia.org/wiki/Serial_port for more info.

try:
    ser = serial.Serial(port = port, baudrate = 9600, timeout = 1) #sets up serial port
    while(True):
        print(ser.readline().decode("utf-8"))  #prints contents of the port to the console
except serial.serialutil.SerialException:  #Simple exception to avoid errors when port isn't connected
        print("EXIT")