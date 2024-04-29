#Alexander Merry, ID:2173596
from pynput.keyboard import Key, Controller
import serial
import time
import msvcrt
keyboard = Controller()
#start arduino connection, chang eCOM port accordingly
arduino = serial.Serial(port='COM5', baudrate=9600,timeout=0.0000000000000001)
def write_read(x): #writes to the arduino and returns the value given in serial monitor
    arduino.write(bytes(x, 'utf-8')) 
    data = arduino.readline() 
    return data 
while True:
    num = msvcrt.getch()
    print(num)
    # Taking input from user 
    value = write_read(str(num)) 
    print(value) # printing the value 
