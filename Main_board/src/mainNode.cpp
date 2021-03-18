#include <Arduino.h>
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define CLOCKWISE 9       //CLOCKWISE DIRECTION
#define ANTI_CLOCKWISE 10 //ANTI CLOCKWISE DIRECTION

#define END_CHAR '>'
#define START_CHAR '<'
#define DIAMETER '%'

#define MAX_PWM 255
#define MIN_PWM 0

float Temperature;
float Humidity;

char incomingByteChar;
String incomingNumberAsStr;

void show_message(int x, int y, String message);
void make_decision_4_irrigation();
void reestablish_serial_connection();

void setup()
{

  Serial.begin(9600);

  pinMode(ANTI_CLOCKWISE, OUTPUT);
  pinMode(CLOCKWISE, OUTPUT);
  digitalWrite(ANTI_CLOCKWISE, 0);


  Temperature = 0;
  Humidity = 0;
  incomingNumberAsStr = "";
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
}

void loop()
{
  if (Serial.available() > 0)
  {
    while (Serial.available())
    {
      incomingByteChar = Serial.read();

      if (incomingByteChar == START_CHAR)
        incomingNumberAsStr = "";

      else if (incomingByteChar == DIAMETER)
      {
        Humidity = incomingNumberAsStr.toFloat();
        show_message(0, 0, "Humidity: " + incomingNumberAsStr);
        incomingNumberAsStr = "";
      }
      else if (incomingByteChar == END_CHAR)
      {
        Temperature = incomingNumberAsStr.toFloat();
        show_message(0, 1, "Temperature: " + incomingNumberAsStr);
        incomingNumberAsStr = "";
        break;
      }
      else
        incomingNumberAsStr += incomingByteChar;

      delay(3);
    }

    make_decision_4_irrigation();
    reestablish_serial_connection();
  }
}

void show_message(int x, int y, String message)
{
  lcd.setCursor(x, y);
  lcd.print(message);
}

void make_decision_4_irrigation()
{
  if (Humidity > 50)
  {
    analogWrite(CLOCKWISE, MIN_PWM);
   

    show_message(0, 2, "Motor Stat: Off");
  }
  else if (Humidity < 20)
  {
    analogWrite(CLOCKWISE, MAX_PWM / 4);
    show_message(0, 2, "Motor Stat: 25%");
  }
  else
  {
    if (Temperature < 25)
    {
      analogWrite(CLOCKWISE, MIN_PWM);
      show_message(0, 2, "Motor Stat: Off");
    }
    else
    {
      analogWrite(CLOCKWISE, MAX_PWM / 10);
      show_message(0, 2, "Motor Stat: 10%");
    }
  }
}

void reestablish_serial_connection()
{
  Serial.end();
  Serial.begin(9600);
}