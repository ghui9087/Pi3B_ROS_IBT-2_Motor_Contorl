#!/usr/bin/env python3

# importaing the GPIO as the PBPI for 3B+ and 4 or higher
import RPi.GPIO as GPIO
import rospy
import serial
import time

from std_msgs.msg import String, Float64

# Testing funcation
# return 'HI' as the respond for the call
def hello_callback(data):
    if data.data == "hello":
        pub_hello.publish("HI")


if __name__ == '__main__':
    rospy.init_node('listener')
    pub_hello = rospy.Publisher('hello', String, latch=True, queue_size=10)
    rospy.Subscriber("hello", String, hello_callback)

    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    # finally:
