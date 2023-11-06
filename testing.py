#!/usr/bin/env python3

# importaing the GPIO as the PBPI for 3B+ and 4 or higher
import RPi.GPIO as GPIO
import rospy

from std_msgs.msg import String, Float64

MOTOR1_F_PWM_PIN = 3
MOTOR1_R_PWM_PIN = 5
MOTOR2_F_PWM_PIN = 3
MOTOR2_R_PWM_PIN = 4

# Motor 1
GPIO.setmode(GPIO.BCM)    
GPIO.setup(MOTOR1_F_PWM_PIN,GPIO.OUT)
GPIO.setup(MOTOR1_R_PWM_PIN,GPIO.OUT)
motor1_F_pwm = GPIO.PWM(MOTOR1_F_PWM_PIN, 20000)  # 4000 Hz frequency
motor1_R_pwm = GPIO.PWM(MOTOR1_R_PWM_PIN, 20000)  # 4000 Hz frequency
motor1_F_pwm.start(0)
motor1_R_pwm.start(0)
# Motor 2
GPIO.setup(MOTOR2_F_PWM_PIN,GPIO.OUT)
GPIO.setup(MOTOR2_R_PWM_PIN,GPIO.OUT)
motor2_F_pwm = GPIO.PWM(MOTOR2_F_PWM_PIN, 20000)  # 4000 Hz frequency
motor2_R_pwm = GPIO.PWM(MOTOR2_R_PWM_PIN, 20000)  # 4000 Hz frequency
motor2_F_pwm.start(0)
motor2_R_pwm.start(0)

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
    global cMotor1Speed, cMotor2Speed
    motor1Speed = data.data[0] * 100
    motor2Speed = data.data[1] * 100

    pub_speed.publish("Speed: M1 = %d , M2 = %d", motor1Speed, motor2Speed)
    if motor1Speed >= 0:
        motor1_R_pwm.ChangeDutyCycle(0)
        motor1_F_pwm.ChangeDutyCycle(abs(motor1Speed))
    else:
        motor1_F_pwm.ChangeDutyCycle(0)
        motor1_R_pwm.ChangeDutyCycle(abs(motor1Speed))

    if motor2Speed >= 0:
        motor2_R_pwm.ChangeDutyCycle(0)
        motor2_F_pwm.ChangeDutyCycle(abs(motor2Speed))
    else:
        motor2_F_pwm.ChangeDutyCycle(0)
        motor2_R_pwm.ChangeDutyCycle(abs(motor2Speed))

    # Update the speed to global
    cMotor1Speed = motor1Speed
    cMotor2Speed = motor2Speed

def clearUP():
    motor1_F_pwm.stop()
    motor1_R_pwm.stop()
    motor2_F_pwm.stop()
    motor2_R_pwm.stop()

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
        clearUP()
        GPIO.cleanup()
