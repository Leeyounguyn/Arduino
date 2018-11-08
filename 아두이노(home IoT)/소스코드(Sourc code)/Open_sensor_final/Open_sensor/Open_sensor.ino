#include<SoftwareSerial.h>

int val;
int swPin = 4;

#define SENDING_IN 1000
unsigned long prevReadTime = 0;

SoftwareSerial hcSerial(3,2);

char door_data = NULL;

void setup()
{
    pinMode(swPin, INPUT_PULLUP);
    Serial.begin(9600);
    hcSerial.begin(9600);
}

void loop()
{
  val = digitalRead(swPin);

  if(val == LOW)
  {
    Serial.println("문이 닫혔습니다."); // 기존에는 자석이 서로 붙어있을떄  출력됨. -> 반대로하여서 해결.
    door_data = 'C';
    Serial.print(door_data);
  }
  else
  {
    Serial.println("문이 열렸습니다.");
    door_data = 'O';
    Serial.print(door_data);
  }

  
  if(millis() - prevReadTime > SENDING_IN)
  {
    hcSerial.write(0x55); // 첫 고정 2bye
    hcSerial.write(0x01);
    hcSerial.write(0x03);
    hcSerial.write((byte)(door_data >> 8));
    hcSerial.write((byte)door_data);
    hcSerial.write((byte)0x00);
    hcSerial.write((byte)0x00);
    hcSerial.write(0xF1);

    prevReadTime = millis();

    //delay(500);
  }
  //delay(1000);
}
