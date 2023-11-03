#!/usr/bin/env python3

import rospy
import serial
import time

from std_msgs.msg import String, Float64MultiArray

serBaudrate = 9600
motorPower = 100

# Testing funcation
# return 'HI' as the respond for the call
def hello_callback(data):
    if data.data == "hello":
        pub_hello.publish("HI")
    return 1


def motorPower(data):
    input_str = data.data
    if input_str.startwith("speed"):
        int(input_str.lstrip('speed'))
    return 1


def tesitngIBT2(data):

    motor1Speed = data.data[0] * motorPower
    motor2Speed = data.data[1] * motorPower

    ser.write(b"status\n")
    lister()

    bin_strM1 = bin("speed1" + motor1Speed + "\n")
    ser.write(bin_strM1)
    lister()

    bin_strM2 = bin("speed2" + motor2Speed + "\n")
    ser.write(bin_strM2)
    lister()

    return 1


def lister():
    input_str = ser.readline().decode("utf-8").strip()
    if input_str == "":
        pub_speed.publish(".")
    else:
        pub_speed.publish("Read input back: " + input_str)

    return 1


if __name__ == '__main__':
    ser = serial.Serial(timeout=5)
    try:
        ser = serial.Serial('/dev/ttyUSB0')
    except SerialException:
        ser = serial.Serial('/dev/ttyACM0')

    ser.baudrate = serBaudrate

    ser.write(b"status\n")
    input_str = ser.readline().decode("utf-8").strip()

    rospy.init_node('listener')
    pub_hello = rospy.Publisher('hello', String, latch=True, queue_size=10)
    pub_speed = rospy.Publisher(
        'speed', Float64MultiArray, latch=True, queue_size=10)
    rospy.Subscriber("hello", String, hello_callback)
    rospy.Subscriber("motorcontorl", Float64MultiArray, tesitngIBT2)

    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
