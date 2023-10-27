/*
IBT-2 Motor Control Board driven by Arduino.

Speed and direction controlled by a potentiometer attached to analog input 0.
One side pin of the potentiometer (either one) to ground; the other side pin to +5V

Connection to the IBT-2 board:
IBT-2 pin 1 (RPWM) to Arduino pin 5(PWM)
IBT-2 pin 2 (LPWM) to Arduino pin 6(PWM)
IBT-2 pins 3 (R_EN), 4 (L_EN), 7 (VCC) to Arduino 5V pin
IBT-2 pin 8 (GND) to Arduino GND
IBT-2 pins 5 (R_IS) and 6 (L_IS) not connected
*/

#include <Arduino.h>

int Motor1_RPWM = 5;
int Motor1_FRPM = 6;

int Motor2_RPWM = 7;
int Motor2_FRPM = 8;

// defult LED On the ESP8266 D1
uint8_t ledPin = LED_BUILTIN;

bool bufferStringComplete = false;
String inputString = "";

void setup()
{
  // Motor 1 setup ide
  pinMode(Motor1_RPWM, OUTPUT);
  pinMode(Motor1_FRPM, OUTPUT);

  // Motor 2 setup ide
  pinMode(Motor2_RPWM, OUTPUT);
  pinMode(Motor2_FRPM, OUTPUT);

  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
  while (Serial.available() <= 0)
  {
    sendingTheOB();
    delay(200);
  }
}

void loop()
{

  int messageFromRBPI = 000000;
  // TODO: Adding the code for the system to sending from the RBPI to the Arduino
  if (bufferStringComplete)
  {
    if (inputString.startsWith("status"))
    {
      sendingTheOB();
    } else if (inputString.startsWith("speed"))
    {

      // END TODO:
      // int data[4] = {};

      // data[0] = messageFromRBPI / 10000;
      // messageFromRBPI = messageFromRBPI - messageFromRBPI / 10000;
      // data[1] = messageFromRBPI / 100;
      // messageFromRBPI = messageFromRBPI - messageFromRBPI / 100;
      // data[2] = messageFromRBPI / 10;
      // data[3] = messageFromRBPI - messageFromRBPI / 10;

      // // Motor 1 contorl speed from the RBPI to the setting the PWM signal
      // // forward rotation
      // analogWrite(Motor1_RPWM, 0);
      // analogWrite(Motor1_FRPM, data[0]);
      // if (data[2] == 0)
      // {
      //   analogWrite(Motor1_FRPM, 0);
      //   analogWrite(Motor1_RPWM, data[0]);
      // }

      // // Motor 2 contorl speed from the RBPI to the setting the PWM signal
      // // forward rotation
      // analogWrite(Motor1_RPWM, 0);
      // analogWrite(Motor1_FRPM, data[1]);
      // if (data[3] == 0)
      // {
      //   analogWrite(Motor1_FRPM, 0);
      //   analogWrite(Motor1_RPWM, data[1]);
      // }
    }
    // reset the command
    bufferStringComplete = false;
    inputString = "";
  }
  // command delay
  delay(10);
  if (Serial.available() > 0)
  {
    serialEventListerner();
  }
}

void sendingTheOB()
{
  char buffer[50];
  digitalWrite(ledPin, HIGH);
  sprintf(buffer, "LED ON");
  Serial.println(buffer);
  delay(500);
  sprintf(buffer, "LED ON");
  Serial.println(buffer);
  digitalWrite(LED_BUILTIN, LOW);
}

void serialEventListerner()
{
  while (Serial.available())
  {
    char inchar = (char)Serial.read();
    inputString += inchar;
    if (inchar == '\n')
    {
      bufferStringComplete = true;
    }
  }
}