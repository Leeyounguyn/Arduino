#include <SoftwareSerial.h>
#include <LSM303D.h>
#include <Wire.h>
#include <SPI.h>

int rate;
int blueTx=2;   //Tx (보내는핀 설정)
int blueRx=3;   //Rx (받는핀 설정)
SoftwareSerial mySerial(blueTx, blueRx);  //시리얼 통신을 위한 객체선언
SoftwareSerial s(4,5);    // 4:RX 5:TX
int accel[3];  // we'll store the raw acceleration values here
int mag[3];  // raw magnetometer values stored here
float realAccel[3];  // calculated acceleration values here
float heading, titleHeading;

#define SPI_CS 10

void setup() {
  char rtn = 0;
  Serial.begin(9600);
  mySerial.begin(9600); //블루투스 시리얼 개방
  s.begin(9600);
  rtn = Lsm303d.initI2C();
  
  if(rtn != 0)  // Initialize the LSM303, using a SCALE full-scale range
  {
    Serial.println("\r\nLSM303D is not found");
    while(1);
  }
  else
  {
    Serial.println("\r\nLSM303D is found");
  }
}
void loop() {
  Serial.println("\r\n**************");
  Lsm303d.getAccel(accel);
  while(!Lsm303d.isMagReady());// wait for the magnetometer readings to be ready
  Lsm303d.getMag(mag);  // get the magnetometer values, store them in mag
  for (int i=0; i<3; i++)
  {
    realAccel[i] = accel[i] / pow(2, 15) * ACCELE_SCALE;  // calculate real acceleration values, in units of g
  }
  heading = Lsm303d.getHeading(mag);
  titleHeading = Lsm303d.getTiltHeading(mag, realAccel);
  
  if(s.available())
  {
    rate = s.read();    //여기로 값이 들어온다
    Serial.println(rate);
    mySerial.print("heat rate : ");
    mySerial.println(rate);
    printValues();    
  }
  delay(2000);
}

void printValues()
{  
  mySerial.print("Acceleration of X,Y,Z is");
  for (int i=0; i<3; i++)
  {
    mySerial.print(realAccel[i]); //보낼값들 X,y,z
    mySerial.println("g");
  }
  /* print both the level, and tilt-compensated headings below to compare */
  Serial.println("The clockwise angle between the magnetic north and x-axis: ");
  mySerial.print(heading);
  mySerial.println(" degrees");
  Serial.print("The clockwise angle between the magnetic north and the projection");
  Serial.println(" of the positive x-axis in the horizontal plane: ");
  mySerial.print(titleHeading);  // see how awesome tilt compensation is?! //이것도 보내야됨
  mySerial.println(" degrees");
}
