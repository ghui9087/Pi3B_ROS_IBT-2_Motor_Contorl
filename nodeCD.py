#!/usr/bin/env python3

import rospy
import serial
import time

from std_msgs.msg import String, Float64

serPort = 'COM1'
serBaudrate = 9600
# Testing funcation
# return 'HI' as the respond for the call
def hello_callback(data):
    if data.data == "hello":
        pub_hello.publish("HI")

    return 0


# When sending the data to the Ardurino The style is all in one Int
# Example: 019810
# Need to be looking like :
#  01      98      1       0
#  Speed   Speed   Dir     Dir
#  Motor1  Motor2  Motor1  Motor2
def tesitngIBT2(data, testing = {1, 0, 0}):
    motor1Speed = testing[1]
    motor2Speed = testing[2]
    if(testing[0] == 1):
        motor1Speed = data.data[0] * 100
        motor2Speed = data.data[1] * 100
    
    motor1Dir = True
    motor2Dir = True
    ser = serial.Serial()
    
    ser.baudrate = serBaudrate
    ser.port = serPort

    if motor1Speed < 0:
        motor1Dir = False
    if motor2Speed < 0:
        motor2Dir = False

    dataSent = abs(motor1Speed) * 100
    dataSent += abs(motor2Speed)
    dataSent = dataSent * 100 + motor1Dir * 10 + motor2Dir

    # Sending the messgae to the Arduino From path UART


    return 0



if __name__ == '__main__':
    rospy.init_node('listener')
    pub_hello = rospy.Publisher('hello', String, latch=True, queue_size=10)
    pub_speed = rospy.Publisher('speed', Float64, latch=True, queue_size=10)
    rospy.Subscriber("hello", String, hello_callback)
    rospy.Subscriber("motorcontorl", Float64, tesitngIBT2)


    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    # finally:
