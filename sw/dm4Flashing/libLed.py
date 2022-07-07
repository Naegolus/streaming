
import RPi.GPIO as GPIO 

GPIO.setwarnings(False)
GPIO.setmode(GPIO.BCM)

pinRed = 12
pinGreen = 19
pinBlue = 13

GPIO.setup(pinRed, GPIO.OUT)
GPIO.setup(pinGreen, GPIO.OUT)
GPIO.setup(pinBlue, GPIO.OUT)

def turnOff():
	GPIO.output(pinRed, GPIO.HIGH)
	GPIO.output(pinGreen, GPIO.HIGH)
	GPIO.output(pinBlue, GPIO.HIGH)

def white():
	GPIO.output(pinRed, GPIO.LOW)
	GPIO.output(pinGreen, GPIO.LOW)
	GPIO.output(pinBlue, GPIO.LOW)

def yellow():
	GPIO.output(pinRed, GPIO.LOW)
	GPIO.output(pinGreen, GPIO.LOW)
	GPIO.output(pinBlue, GPIO.HIGH)

def red():
	GPIO.output(pinRed, GPIO.LOW)
	GPIO.output(pinGreen, GPIO.HIGH)
	GPIO.output(pinBlue, GPIO.HIGH)

def green():
	GPIO.output(pinRed, GPIO.HIGH)
	GPIO.output(pinGreen, GPIO.LOW)
	GPIO.output(pinBlue, GPIO.HIGH)

