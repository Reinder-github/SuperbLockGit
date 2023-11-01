import serial
import time
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad

if list(serial.tools.list_ports.comports()) != []:
	arduino = serial.Serial(arduino = serial.Serial(port='COM5', baudrate=115200, timeout=.1))

def write_read(x): 
	arduino.write(x) 
	time.sleep(0.05) 
	data = arduino.readline() 
	return data 

def encrypt(key, data):
	cipher = AES.new(key, AES.MODE_ECB)
	print (cipher.encrypt(pad(data, AES.block_size, style='pkcs7')))

msg = encrypt(b'abcdefghijklmnop',b'test')
print(msg)
