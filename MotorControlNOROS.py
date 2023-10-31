# import serial

def tesitngIBT2(data, testing = [1, 0, 0]):
    serPort = 'COM1'
    serBaudrate = 9600
    motor1Speed = testing[1]
    motor2Speed = testing[2]
    if(testing[0] == 1):
        motor1Speed = data.data[0] * 100
        motor2Speed = data.data[1] * 100
    
    motor1Dir = True
    motor2Dir = True
    # ser = serial.Serial()
    
    # ser.baudrate = serBaudrate
    # ser.port = serPort

    if motor1Speed < 0:
        motor1Dir = False
    if motor2Speed < 0:
        motor2Dir = False

    dataSent = abs(motor1Speed) * 100
    dataSent += abs(motor2Speed)
    dataSent = dataSent * 100 + motor1Dir * 10 + motor2Dir
    a = str(dataSent)
    if len(a) == 5:
        a = "0" + a
    print(a)
    # Sending the messgae to the Arduino From path UART

    # ser.write(bin(dataSent))
    input_str = ser.readline().decode("utf-8").strip()
    if input_str == "":
        print(".")
    else:
        print("Read input back: " + input_str)

    return 0

tesitngIBT2(0, [0, 2,-2])