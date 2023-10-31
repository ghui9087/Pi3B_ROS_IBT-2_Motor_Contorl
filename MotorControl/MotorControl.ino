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

int Motor2_RPWM = 9;
int Motor2_FRPM = 10;

// defult LED On the ESP8266 D1
uint8_t ledPin = LED_BUILTIN;

bool bufferString = false;
String inputString = "";

int motor1S = 0;
int motor2S = 0;

void setup()
{
  // Motor 1 setup ide
  pinMode(Motor1_RPWM, OUTPUT);
  pinMode(Motor1_FRPM, OUTPUT);

  // Motor 2 setup ide
  pinMode(Motor2_RPWM, OUTPUT);
  pinMode(Motor2_FRPM, OUTPUT);

  // Defult LED pin setup ide
  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);

  // if the Serial is not being connect to the other system that support the Serial
  // Puse the command here
  while (!Serial)
  {
    ;
  }

  // Once the Serial is connect to the other system that support the Serial
  // loop the current status until First command have being send from the UART
  while (Serial.available() <= 0)
  {
    sendingTheOB();
    delay(200);
  }
}

// While loop that will not be stop unless the Ardurino Board is crash
void loop()
{

  // If the bufferString is still in read or it was not finish, Skip during this loop
  if (bufferString)
  {
    // Base status information checking
    if (inputString.startsWith("status"))
    {
      sendingTheOB();
    }
    // Gettng speed fro mthe command "speed"
    else if (inputString.startsWith("speed"))
    {
      inputString.remove(0, 5);
      inputString.remove(inputString.length() - 1);

      // Checking if the Motor speed and the select is in the digit format
      // It can chekcing if the motor is in the negivte value
      if (isStringDigit(inputString))
      {

        // Chekcing if the motor select is the Motor ID: 0
        if (inputString.startsWith("0"))
        {
          inputString.remove(0, 1);
          unsigned long messageFromRBPI = strtoul(inputString.c_str(), NULL, 10);
          motor1S = messageFromRBPI;

          // Motor 1 contorl speed from the RBPI to the setting the PWM signal
          // forward rotation
          analogWrite(Motor1_RPWM, 0);
          analogWrite(Motor1_FRPM, abs(motor1S));

          // reverse rotation
          if (motor1S < 0)
          {
            analogWrite(Motor1_FRPM, 0);
            analogWrite(Motor1_RPWM, abs(motor1S));
          }

          // Return the current status of both motor
          sendingTheOB();
        }
        // Chekcing if the motor select is the Motor ID: 1
        else if (inputString.startsWith("1"))
        {
          inputString.remove(0, 1);
          unsigned long messageFromRBPI = strtoul(inputString.c_str(), NULL, 10);
          motor2S = messageFromRBPI;

          // Motor 2 contorl speed from the RBPI to the setting the PWM signal
          // forward rotation
          analogWrite(Motor2_FRPM, 0);
          analogWrite(Motor2_RPWM, abs(motor2S));

          // reverse rotation
          if (motor2S < 0)
          {
            analogWrite(Motor2_RPWM, 0);
            analogWrite(Motor2_FRPM, abs(motor2S));
          }

          // Return the current status of both motor
          sendingTheOB();
        }
        // If they are not both Return the ERROR
        else
        {
          Serial.println("Speed Data Error:/n Can not select the Motor ID:" + inputString);
        }
      }
      // If the motor inforamtion is not the digit after the word "speed" Return ERROR
      else
      {
        Serial.println("Speed Data Error: /n Motor is not in the digit format");
      }
    }
    // If the command is not else status or speed Return the ERROR invald command
    else
    {
      Serial.println("Inviald Command");
    }
    // reset the command
    bufferString = false;
    inputString = "";
  }
  // command delay 10ms
  delay(10);
  
  // Once the Serial is available running the serial listerner 
  if (Serial.available() > 0)
  {
    serialEventListerner();
  }
}

/**
 * Status Chekcing with the Building board LED flash once
 * This will return the both motor current speed at the same time
 * They will return in this formart:
 * Current Speed1: x%, 2: x%
 * Once the LEC flash once It mean the report have send back on the UART
 */
void sendingTheOB()
{
  char buffer[50];
  sprintf(buffer, "Current Speed 1: %d%%, 2: %d%%%", motor1S, motor2S);
  Serial.println(buffer);
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
}

/**
 * Listerner that Listern the UART from the Serial
 * Lister only lister the command that is send fro mthe Serial that In beingfarmat in the uart.
 * And they need to be in binery
 * Once the serial recived the command, It will cover they back to string form
 * They will be add into the inputString
 * And rest the bufferString to true
 * Just Listerner not judgement
 */
void serialEventListerner()
{
  while (Serial.available())
  {
    char inchar = (char)Serial.read();
    inputString += inchar;
    if (inchar == '\n')
    {
      bufferString = true;
    }
  }
}

/** 
 * Checking if the String is pure digit or else
 * If they are not pure digit it will return false
 * else return true
*/
bool isStringDigit(String data)
{
  for (int i = 0; i < inputString.length(); i++)
  {
    if (i == 1 && inputString[i] == '-')
    {
      continue;
    }

    if (!isdigit(inputString[i]))
    {
      return false;
    }
  }
  return true;
}