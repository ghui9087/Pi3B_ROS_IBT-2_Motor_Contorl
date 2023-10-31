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

bool bufferStringComplete = false;
String inputString = "";

int motor1S = 0;
int motor2S = 0;

void setup() {
  // Motor 1 setup ide
  pinMode(Motor1_RPWM, OUTPUT);
  pinMode(Motor1_FRPM, OUTPUT);

  // Motor 2 setup ide
  pinMode(Motor2_RPWM, OUTPUT);
  pinMode(Motor2_FRPM, OUTPUT);

  pinMode(ledPin, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  while (Serial.available() <= 0) {
    sendingTheOB();
    delay(200);
  }
}

void loop() {

  // TODO: Adding the code for the system to sending from the RBPI to the Arduino
  if (bufferStringComplete) {
    // Base status information checking
    if (inputString.startsWith("status")) {
      sendingTheOB();
    } else if (inputString.startsWith("speed")) {
      inputString.remove(0, 5);
      inputString.remove(inputString.length() - 1);
      if (isStringDigit(inputString)) {
        if (inputString.startsWith("0")) {
          inputString.remove(0, 1);
          unsigned long messageFromRBPI = strtoul(inputString.c_str(), NULL, 10);
          motor1S = messageFromRBPI;
          // Motor 1 contorl speed from the RBPI to the setting the PWM signal
          // forward rotation
          analogWrite(Motor1_RPWM, 0);
          analogWrite(Motor1_FRPM, abs(motor1S));
          if (motor1S < 0) {
            analogWrite(Motor1_FRPM, 0);
            analogWrite(Motor1_RPWM, abs(motor1S));
          }
          sendingTheOB();
        } else {
          inputString.remove(0, 1);
          unsigned long messageFromRBPI = strtoul(inputString.c_str(), NULL, 10);
          motor2S = messageFromRBPI;
          // Motor 2 contorl speed from the RBPI to the setting the PWM signal
          // forward rotation
          analogWrite(Motor2_FRPM, 0);
          analogWrite(Motor2_RPWM, abs(motor2S));
          if (abs(motor2S) < 0) {
            analogWrite(Motor2_RPWM, 0);
            analogWrite(Motor2_FRPM, abs(motor2S));
          }
          sendingTheOB();
        }
      } else {
        Serial.println("Speed Data Is increcing");
      }
    }
    // reset the command
    bufferStringComplete = false;
    inputString = "";
  }
  // command delay
  delay(10);
  if (Serial.available() > 0) {
    serialEventListerner();
  }
}

/**
 * Status Chekcing with the Buildign board LED flash once
 * Once the LEC flash once It mean the report have send back on the UART
 */
void sendingTheOB() {
  char buffer[50];
  sprintf(buffer, "Current Speed 1: %d%%, 2: %d%%%", motor1S, motor2S);
  Serial.println(buffer);
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
}

/**
 * Listerner that Listern the UART from the Serial
 * Just Listerner not judgement
 */
void serialEventListerner() {
  while (Serial.available()) {
    char inchar = (char)Serial.read();
    inputString += inchar;
    if (inchar == '\n') {
      bufferStringComplete = true;
    }
  }
}

bool isStringDigit(String data) {
  for (int i = 0; i < inputString.length(); i++) {
    if (i == 1 && inputString[i] == '-') {
      continue;
    }

    if (!isdigit(inputString[i])) {
      return false;
    }
  }
  return true;
}