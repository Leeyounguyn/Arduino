#include <SoftwareSerial.h>

int measurePin = 0;
int ledPower = 4;       //

#define SENDING_IN 5000
unsigned long prevReadTime = 0;

SoftwareSerial hcSerial(2,3);

int samplingTime = 280;
int deltaTime = 40;
int sleepTime = 9680;

float voMeasured = 0;     //
float calcVoltage = 0;    //
float dustDensity = 0;    //

double integer;    // 정수부를 받기위해서 필요한 변수.
double fraction;   // 실수부를 받기위해서 필요한 변수
int SendDust = 0;  //
int SendDust2 = 0;// 


void setup() {
  Serial.begin(9600);
  hcSerial.begin(9600);
  pinMode(ledPower, OUTPUT);
}

void loop() {
  digitalWrite(ledPower, LOW);
  delayMicroseconds(samplingTime);
  voMeasured = analogRead(measurePin);
  delayMicroseconds(deltaTime);
  digitalWrite(ledPower, HIGH);
  delayMicroseconds(sleepTime);

  calcVoltage = voMeasured *(5.0 / 1024.0);
  dustDensity = 0.17 * calcVoltage - 0.1;

  Serial.print("  Dust Density : ");
  Serial.println(dustDensity);

  fraction = modf(dustDensity, &integer);

  Serial.println((int)integer);
  SendDust = (int)integer;
  SendDust2 = fraction * 100;

//sending
  if(millis() - prevReadTime > SENDING_IN){
  {
    hcSerial.write(0x55); // 첫 고정 2bye
    hcSerial.write(0x01);
    hcSerial.write(0x05);
    hcSerial.write((byte)(SendDust >> 8));
    hcSerial.write((byte)SendDust);
    hcSerial.write((byte)(SendDust2 >> 8));
    hcSerial.write((byte)SendDust2);
    hcSerial.write(0xF3);

    prevReadTime = millis();
    }
    delay(1000);
  }
}
