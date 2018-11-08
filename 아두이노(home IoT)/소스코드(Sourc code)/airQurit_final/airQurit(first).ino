#include <SoftwareSerial.h>

int pin = 0;
SoftwareSerial hcSerial(3,2);

int AirQuality = 0;

#define SENDING_IN 3000
unsigned long prevReadTime = 0;

void setup() {
  Serial.begin(9600);
  hcSerial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  AirQuality = (int)((float)sensorValue /1024 * 500);

  if(millis() - prevReadTime > SENDING_IN)
  {
    hcSerial.write(0x55); // 첫 고정 2bye
    hcSerial.write(0x01);
    hcSerial.write(0x02);
    hcSerial.write((byte)(AirQuality >> 8));
    hcSerial.write((byte)AirQuality);
    hcSerial.write((byte)0x00);
    hcSerial.write((byte)0x00);
    hcSerial.write(0xFF);

    prevReadTime = millis();

    delay(500);
  }
}
