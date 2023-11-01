import serial
import time
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
import serial.tools.list_ports

def write_read(x):
    arduino = serial.Serial(port="COM5", baudrate=9600, timeout=1)
    time.sleep(3)
    arduino.write(x) 
    time.sleep(1)
    data = arduino.readline()
    return data   

def encrypt(key, data):
    cipher = AES.new(key, AES.MODE_ECB)
    return (cipher.encrypt(pad(data, AES.block_size, style='pkcs7')))

write_read(encrypt(b"abcdefghijklmnop",b'uid'))