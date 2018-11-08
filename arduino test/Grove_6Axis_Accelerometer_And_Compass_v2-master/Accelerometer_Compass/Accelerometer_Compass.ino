//출력본
#include <SoftwareSerial.h>
#include <LSM303D.h>
#include <Wire.h>
#include <SPI.h>

SoftwareSerial serial(4,5);    // 4:RX 5:TX
/* Global variables */
int accel[3];  // we'll store the raw acceleration values here
int mag[3];  // raw magnetometer values stored here
float realAccel[3];  // calculated acceleration values here
float heading, titleHeading;

#define SPI_CS 10

void setup()
{
	  char rtn = 0;
    Serial.begin(9600);  // Serial is used for debugging
    serial.begin(9600);
    Serial.println("\r\npower on");
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

void loop()
{
	Serial.println("\r\n**************");
	//getLSM303_accel(accel);  // get the acceleration values and store them in the accel array
	Lsm303d.getAccel(accel);
	while(!Lsm303d.isMagReady());// wait for the magnetometer readings to be ready
	Lsm303d.getMag(mag);  // get the magnetometer values, store them in mag
	
	for (int i=0; i<3; i++)
	{
		realAccel[i] = accel[i] / pow(2, 15) * ACCELE_SCALE;  // calculate real acceleration values, in units of g
	}
	heading = Lsm303d.getHeading(mag);
	titleHeading = Lsm303d.getTiltHeading(mag, realAccel);
	
	printValues();
	
	delay(5000);  // delay for serial readability
}

void printValues()
{  
	Serial.println("Acceleration of X,Y,Z is");
	for (int i=0; i<3; i++)
	{
		serial.write(realAccel[i]); //보낼값들 X,y,z
		Serial.println("g");
	}
	/* print both the level, and tilt-compensated headings below to compare */
	Serial.println("The clockwise angle between the magnetic north and x-axis: ");
	serial.write(heading); // this only works if the sensor is level //이것도 보내야됨
	Serial.println(" degrees");
	Serial.print("The clockwise angle between the magnetic north and the projection");
	Serial.println(" of the positive x-axis in the horizontal plane: ");
	serial.write(titleHeading);  // see how awesome tilt compensation is?! //이것도 보내야됨
	Serial.println(" degrees");
}
