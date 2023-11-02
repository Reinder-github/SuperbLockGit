import serial
import time

def modify_uids(x):
    try:
        arduino = serial.Serial(port="COM5", baudrate=9600, timeout=1)
        time.sleep(3)
        arduino.write(bytes(x,'utf-8')) 
        time.sleep(1)
        data = arduino.readline()
        return data
    except:
        print('No Arduino, could not transmit')


def encrypt(text):
    result = ""
    shift = 1
    
    for char in text:
        if char.isalpha():
            is_upper = char.isupper()
            char = char.lower()
            char_code = ord(char)
            char_code = (char_code - ord('a') + shift) % 26
            char_code = char_code + ord('a')
            
            if is_upper:
                char = chr(char_code).upper()
            else:
                char = chr(char_code)
        elif char.isdigit():
            char = str((int(char) - shift) % 10)
        
        result += char

    return result

