#include <Wire.h>
#include <SoftwareSerial.h>
int sensorValue;
SoftwareSerial s(4,5);    // 4:RX 5:TX

void setup() {
  Serial.begin(9600);
  Wire.begin();
  s.begin(9600);
}

void loop() {
   Wire.requestFrom(0xA0 >> 1, 1);
       while(Wire.available()) {          // slave may send less than requested
        unsigned char c = Wire.read();   // receive heart rate value (a byte)
        sensorValue = c;       
        Serial.println(sensorValue);   // print heart rate value
        s.write(sensorValue);  
    }
   delay(2000);
}
