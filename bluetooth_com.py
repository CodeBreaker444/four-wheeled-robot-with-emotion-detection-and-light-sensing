import time
import bluetooth
from serial import Serial
from gtts import gTTS
import time
import pyttsx3
engine = pyttsx3.init()
engine.setProperty('rate', 180)
serial_com="/dev/cu.HC-06-DevB"
control_cb_command_cv='1' # 1 for computer vision and 2 for light following
sad_speak = 'Dont be sad, everything will be okay. One must not let oneself be overwhelmed by sadness.'
anger_speak = 'Im sorry if i had done anything wrong. Holding on to anger is like grasping a hot coal with the intent of throwing it at someone else; you are the one who gets burned. '
happy_speak='hurray, your smile makes me happy. Dont forget to smile everyday no matter what!'
language='en'
class cb_bluetooth_communicator:
    def cb_ard_com(command):
    # # Serial port parameters
        serial_speed = 9600
        serial_port = serial_com  # bluetooth shield hc-06
        print("conecting to serial port ...")
        if control_cb_command_cv=='2':
            command='2'
        try:
            ser = Serial(serial_port, serial_speed, timeout=1)
            print("sending message to turn on PIN 13 ...")
            ser.write(str.encode(command))
        except:
            print('Cannot Connect to the Blutooth module..')
        if command=='h':
            pyttsx3.speak(happy_speak)
        elif command=='s':
            pyttsx3.speak(sad_speak)
        elif command=='a':
            print('hello')
            pyttsx3.speak(anger_speak)

        print("recieving message from arduino ...")
        data = ser.readline()

        if (data != ""):
            print("arduino says: %s" % data)
        else:
            print("arduino doesnt respond")

        time.sleep(4)
        print("finish program and close connection!")

#cb_bluetooth_communicator.cb_ard_com('a')
