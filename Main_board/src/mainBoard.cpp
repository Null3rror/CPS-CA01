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

float Temperature;
float Humidity;

char incomingByteChar;
String incomingNumberAsStr;

void setup()
{

  Serial.begin(9600);

  pinMode(ANTI_CLOCKWISE, OUTPUT);
  pinMode(CLOCKWISE, OUTPUT);

  Temperature = 0;
  Humidity = 0;
  incomingNumberAsStr = "";
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
}

void loop()
{

  //analogWrite(CLOCKWISE, 255);
  //digitalWrite(ANTI_CLOCKWISE, 0);
  if (Serial.available() > 0)
  {
    while (Serial.available())
    {
      incomingByteChar = Serial.read();

      if (incomingByteChar == START_CHAR)
        incomingNumberAsStr = "";

      else if (incomingByteChar == END_CHAR)
      {
        Temperature = incomingNumberAsStr.toFloat();
        lcd.setCursor(0, 0);
        lcd.print("Temperature: " + String(Temperature));
        incomingNumberAsStr = "";
        break;
      }
      else if (incomingByteChar == DIAMETER)
      {
        Humidity = incomingNumberAsStr.toFloat();
        lcd.setCursor(0, 1);
        lcd.print("Humidity: " + String(Humidity));
        incomingNumberAsStr = "";
      }
      else
        incomingNumberAsStr += incomingByteChar;

      delay(1);
    }

    if (Humidity > 50)
    {
      digitalWrite(CLOCKWISE, 0);
      digitalWrite(ANTI_CLOCKWISE, 0);

      lcd.setCursor(0, 2);
      lcd.print("Motor Off        ");
    }
    else if (Humidity < 20)
    {
      analogWrite(CLOCKWISE, 63);
      lcd.setCursor(0, 2);
      lcd.print("Motor Speed: 25%");
    }
    else
    {
      if (Temperature < 25)
      {
        digitalWrite(CLOCKWISE, 0);
        lcd.setCursor(0, 2);
        lcd.print("Motor Off        ");
      }
      else
      {
        analogWrite(CLOCKWISE, 26);
        lcd.setCursor(0, 2);
        lcd.print("Motor Speed: 10%");
      }

      Serial.end();
      Serial.begin(9600);
    }
  }

  // set first clumn first row
  // lcd.setCursor(0, 0);
  // // print the number of seconds since reset:
  // lcd.print("test lcd");
}