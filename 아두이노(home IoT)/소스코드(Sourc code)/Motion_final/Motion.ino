#include<SoftwareSerial.h>

int val = 0;
SoftwareSerial hcSerial(2,3);

#define SENDING_IN 5000
unsigned long prevReadTime = 0;

void setup() {
  pinMode(7, INPUT);     // declare sensor as input
  Serial.begin(9600);
  hcSerial.begin(9600);
}

void loop() {
  val = digitalRead(7);  // read input value
  Serial.println(val);

  if(millis() -prevReadTime > SENDING_IN)
  {
     hcSerial.write(0x55);
     hcSerial.write(0x01);
     hcSerial.write(0x04);
     hcSerial.write((byte)(val>>8));
     hcSerial.write((byte)val);
     hcSerial.write((byte)0x00);
     hcSerial.write((byte)0x00);
     hcSerial.write(0xF2);

     prevReadTime =millis();
  }
}
