import ASUSPi.GPIO as GPIO
import unittest   
import time     
# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.RK)

# set up GPIO output channel, we set GPIO4 (Pin 7) to OUTPUT


GPIO.setup(9, GPIO.OUT)
GPIO.setup(11, GPIO.OUT)
GPIO.output(11,GPIO.LOW)
GPIO.output(9,GPIO.LOW)
GPIO.setup(8, GPIO.IN)
def cb(channel):
	print "cb called"
	GPIO.output(11,GPIO.HIGH)


GPIO.add_event_detect(8, GPIO.RISING)
GPIO.add_event_callback(8, callback=cb)
#GPIO.wait_for_edge(8, GPIO.RISING)
GPIO.output(9,GPIO.LOW)
GPIO.output(9,GPIO.HIGH)
GPIO.output(9,GPIO.LOW)
while True:
	time.sleep(1)








#GPIO.setup(8, GPIO.OUT)
#GPIO.setup(1, GPIO.OUT)
#GPIO.setup(9, GPIO.OUT)
#GPIO.setup(8, GPIO.IN)
#GPIO.input(8)

#GPIO.output(9,GPIO.HIGH)
#GPIO.input(8)
#GPIO.output(9,GPIO.LOW)
#GPIO.input(8)
#GPIO.output(9,GPIO.HIGH)
#GPIO.input(8)
#GPIO.output(9,GPIO.LOW)
#GPIO.input(8)


