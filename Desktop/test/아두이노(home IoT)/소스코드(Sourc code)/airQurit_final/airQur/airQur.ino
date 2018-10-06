#include <SoftwareSerial.h>

int pin = 0;
SoftwareSerial hcSerial(3,2); //Tx and Rx pins of HC-11 module

int AirQuality = 0;

#define SENDING_IN 4000  //Interval 500 ms data Send
unsigned long prevReadTime = 0;

void setup() 
{  
  Serial.begin(9600);
  hcSerial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A0);
  AirQuality = (int)((float)sensorValue /1024 * 500);

  if(millis() - prevReadTime > SENDING_IN) // 일정간격 마다 측정된 온습도 를 전송
  {
    hcSerial.write(0x55); // 첫 2byte 고정
    hcSerial.write(0x01);
    hcSerial.write(0x02); // Client id : 2(ariQuality)
    hcSerial.write((byte)(AirQuality >> 8)); // byte AirQuality 
    hcSerial.write((byte)AirQuality);
    hcSerial.write((byte)0x00);
    hcSerial.write((byte)0x00);
    hcSerial.write(0xFF); //패킷의 끝을 알리는 byte

    prevReadTime = millis();
  }
  delay(1000);
}
