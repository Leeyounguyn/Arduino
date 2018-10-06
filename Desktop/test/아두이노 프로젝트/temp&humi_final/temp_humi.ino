#include <SimpleDHT.h>
#include <SoftwareSerial.h>

int pinDHT11 = 4; //DHT1 data pin
SimpleDHT11 dht11;

#define SENDING_INTERVAL 1000
unsigned long prevReadTime = 0;

SoftwareSerial hcSerial(3, 2);



void setup() {
  Serial.begin(9600);
  hcSerial.begin(9600);
}

void loop() {

Serial.println("====================");
Serial.println("Sample DHT11...");

  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;  
  if ((err = dht11.read(pinDHT11, &temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
  {
    Serial.print("Read DHT11 failed, err="); Serial.print(err);
    return;
  }
    Serial.print("Sample OK: ");
    Serial.print((int)temperature); Serial.print(" *C, "); //int temperature -> 값임.  
    Serial.print((int)humidity); Serial.println(" %");    

    if(millis() - prevReadTime > SENDING_INTERVAL)
    {
       hcSerial.write(0x55);
       hcSerial.write(0x01);
       hcSerial.write(0x01); // client 장치를 구분하는 id 
       hcSerial.write((byte)(temperature >> 8));
       hcSerial.write((byte)temperature);
       hcSerial.write((byte)(humidity >> 8));
       hcSerial.write((byte)humidity);
       hcSerial.write(0xFE);

       prevReadTime = millis();
    }
  delay(1000);
}
