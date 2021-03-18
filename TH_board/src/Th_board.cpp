#include <Arduino.h>
#include <Wire.h>

#define SHT25_I2C_ADDR 0x40
#define HUMIDITY_MEASURE 0XF5
#define TEMPERATURE_MEASURE 0XF3

#define END_CHAR '>'
#define START_CHAR '<'
#define DIAMETER '%'

unsigned long maxUnsignedLong = 1;

void I2CTransmision(int mode);
float ReadHumidityData();
float ReadTemperatureData();
bool ShouldBroadcast();
void Broadcast(float humidity , float temperature );

unsigned long lastBroadcastTime;
float Humidity, Temperature;
void setup()
{
  maxUnsignedLong =(maxUnsignedLong << 31) - 1;
  lastBroadcastTime = 0;
  Wire.begin(9600);
  Serial.begin(9600);

 
}

void loop()
{


  I2CTransmision(HUMIDITY_MEASURE);
  delay(100);

  Wire.requestFrom(SHT25_I2C_ADDR, 2);

  if (Wire.available() == 2)
  {
    Humidity = ReadHumidityData();
  }

  I2CTransmision(TEMPERATURE_MEASURE);
  delay(100);

  Wire.requestFrom(SHT25_I2C_ADDR, 2);

  if (Wire.available() == 2)
  {
    Temperature = ReadTemperatureData();
  }


  bool shouldBroadcast = ShouldBroadcast();

  if (shouldBroadcast)
  {
    Broadcast(Humidity , Temperature);
    lastBroadcastTime = millis() - millis() %1000;
  }


}

void I2CTransmision(int mode)
{
  Wire.beginTransmission(SHT25_I2C_ADDR);
  Wire.write(mode);
  Wire.endTransmission();
}

float ReadHumidityData()
{
  unsigned int data[2];

  data[0] = Wire.read();
  data[1] = Wire.read();
  // Convert the data
  float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6;
  // Output data to Serial Monitor
  return humidity;
}

float ReadTemperatureData()
{
  unsigned int data[2];

  data[0] = Wire.read();
  data[1] = Wire.read();
  // Convert the data
  float cTemp = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
  // float fTemp = (cTemp * 1.8) + 32;
  // Output data to Serial Monitor
  return cTemp;
}

bool ShouldBroadcast(){
  return ((millis() % maxUnsignedLong) - (lastBroadcastTime % maxUnsignedLong)) > 5000;
  

}
void Broadcast(float humidity , float temperature ){

  Serial.print(START_CHAR);
  Serial.print(humidity);
  Serial.print(DIAMETER);
  Serial.print(temperature);
  Serial.println(END_CHAR);
}
