#!/usr/bin/env python

# importaing the GPIO as the PBPI for 3B+ and 4 or higher
import RPi.GPIO as GPIO
import rospy

from std_msgs.msg import String, Float64

# Testing funcation
# return 'HI' as the respond for the call
def hello_callback(data):
    if data.data == "hello":
        pub_hello.publish("HI")
# Led Pin Testing funcation
# THis is only workijng if you have connect the Led to the GPIO 1 And connect other end to GND
def light_callback(data):
    if data.data == "on":
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(1, GPIO.OUT)
        GPIO.output(1, GPIO.HIGH)
        pub_light.publish("LED is On")

def motorContorl(data):
    float motor1Speed = data.data[0]
    float motor2Speed = data.data[1]
    GPIO.setmode(GPIO.BCM)
    # Motor 1
    GPIO.setup(1,GPIO.OUT)
    GPIO.setup(2,GPIO.OUT)
    # Motor 2
    GPIO.setup(3,GPIO.OUT)
    GPIO.setup(4,GPIO.OUT)
    pub_speed.publish("Speed: M1 = %s , M2 = %s", motor1Speed, motor2Speed)
    if
    


if __name__ == '__main__':
    rospy.init_node('listener')
    pub_hello = rospy.Publisher('hello', String, latch=True, queue_size=10)
    pub_light = rospy.Publisher('light', String, latch=True, queue_size=10)
    pub_speed = rospy.Publisher('speed', Float64, latch=True, queue_size=10)
    rospy.Subscriber("hello", String, hello_callback)
    rospy.Subscriber("light", String, light_callback)
    rospy.Subscriber("motorcontorl", Float64, motorContorl)

    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    finally:
        GPIO.cleanup()
