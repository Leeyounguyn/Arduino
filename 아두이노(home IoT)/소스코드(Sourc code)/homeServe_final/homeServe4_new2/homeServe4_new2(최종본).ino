#include<SoftwareSerial.h>

SoftwareSerial hcSerial(2,3);

const char CHECK_0 = 0x55; 
const char CHECK_1 = 0x01;
const char CHECK_2_1 = 0x01;  // (Temperature & Humidity first) 온습도 센서 확인 완료
const char CHECK_2_2 = 0x02;  //  (AirQuality first)             공기질 센서 확인 완료
const char CHECK_2_3 = 0x03;  //  (Door Loock first)             문열림 센서 확인  완료
const char CHECK_2_4 = 0x04;  //  인체감지 센서
const char CHECK_2_5 = 0x05;    //미세먼지 센서            
const char CHECK_END_1 = 0xFE;  // 온습도 센서 마지막값.
const char CHECK_END_2 = 0xFF;  // 공기질 센서 마지막값.
const char CHECK_END_3 = 0xF1;  // 문열림 센서 마지막값.
const char CHECK_END_4 = 0xF2;   //인체 감지 센서 마지막값.
const char CHECK_END_5 = 0xF3;   //미세먼지  센서 마지막값.
const int DISP_CHAR_LEN = 11;    // 디지털 핀의 값
char str_line1[] = " Temp :     ";
char str_line2[] = " Humi :     ";
char str_line3[] = " AirQ :     ";
char str_line4[] = " DoorState :";
char str_line5[] = "Motion :  ";
char str_line6[] = "Dust Density :"

#define BUFFER_SIZE 8

static char Buffer[BUFFER_SIZE] = {0x00, 0x00, 0x00, 
                        0x00,0x00, 0x00, 0x00, 0x00};
static char SendingBuffer[BUFFER_SIZE+1] = {0x00, 0x00, 0x00, 
                        0x00,0x00, 0x00, 0x00, 0x00};

int AirQuality  = 0;
int Humidity    = 0;
int Temperature = 0;
int DoorSensor  = 0;
int Motion      = 0;
int DustInt = 0;
float DustFloat = 0;

void setup() 
{
  Serial.begin(9600);
  hcSerial.begin(9600);
}

void loop() {
  boolean is_state_changed = false;  // 값이 들어왔는지 확인하는부분
  
  if(hcSerial.available() > 0)
  {
    char in_byte = 0;  //byte 단위로 들어오는 값.
    in_byte = hcSerial.read();

    for(int i = 0; i<BUFFER_SIZE-1; i++)
    {
      Buffer[i] = Buffer[i+1]; 
    }
    Buffer[BUFFER_SIZE-1] = in_byte;
    
    if(Buffer[0] == CHECK_0 && Buffer[1] == CHECK_1)  //첫문자와 두번쨰문자를 이용하여 구분 0x55 0x01
    {
      if(Buffer[2] == CHECK_2_1)  //
      {
          Serial.println("Parse tem & humi command : ");

          if(Buffer[7] == CHECK_END_1)
          {
             Temperature = (Buffer[3] << 8) | Buffer[4];
             Humidity    = (Buffer[5] << 8) | Buffer[6];
             Serial.print("Temp =");
             Serial.print(Temperature);
             Serial.print(", Humi = ");
             Serial.println(Humidity);
             is_state_changed = true;
          }
          Serial.println("Parsing Completed.");
      }
        if(Buffer[2] == CHECK_2_2)  // 공기상태를에대한정보가(else if 로 바꿔야함.) 들어왔을떄.
        {
          Serial.println("Parse AriQ  : ");

          if(Buffer[7] == CHECK_END_2)
          {
            AirQuality = (Buffer[3] << 8) | Buffer[4];
            Serial.print("AriQ = ");
            Serial.println(AirQuality);
            is_state_changed = true;
          }
          Serial.println("Parsing Completed");
    }
      if(Buffer[2] == CHECK_2_3)
      {

        Serial.println("Parse DoorSate : ");

        if(Buffer[7] == CHECK_END_3)
        {
          DoorSensor = (Buffer[3] << 8) | Buffer[4];
          Serial.print("DoorState = ");
          Serial.print(DoorSensor);
          is_state_changed = true;
        }
      }
      if(Buffer[2] == CHECK_2_4)
      {
        if(Buffer[7] == CHECK_END_4)
        {
          Motion = (Buffer[3]<< 8) | Buffer[4];
          Serial.print("Motion = ");
          Serial.print(Motion);
          is_state_changed = true;
        }
      }
      if(Buffer[2] == CHECK_2_5)
      {
        if(Buffer[7] == CHECK_END_5)
        {
          DustInt = (Buffer[3]<< 8) | Buffer[4];
          DustFloat = (Buffer[5]<< 8) | Buffer[6];
          DustFloat = DustInt + (DustFloat * 0.01);
          Serial.print("Dust = ");
          Serial.println(DustFloat);
          is_state_changed = true;
        }
      }
   }

  if(is_state_changed)
  {
    for(int i= 0; i<DISP_CHAR_LEN; i++)
    {
        str_line1[i] = 0x00;
        str_line2[i] = 0x00;
        str_line3[i] = 0x00;
        str_line4[i] = 0x00;
        str_line5[i] = 0x00;
    }
    String dataString1 = "Temp : ";
    dataString1 += Temperature;
    dataString1 += "C";
    dataString1.toCharArray(str_line1, DISP_CHAR_LEN);
    hcSerial.println(dataString1); 

    String dataString2 ="Humi : ";
    dataString2 += Humidity;
    dataString2 += "%";
    dataString2.toCharArray(str_line2, DISP_CHAR_LEN);
    hcSerial.println(dataString2); 

    String dataString3 ="AirQ : ";
    dataString3 += AirQuality;
    dataString3 +="";
    dataString3.toCharArray(str_line3, DISP_CHAR_LEN);    
    hcSerial.println(dataString3);

     String dataString4 ="DoorState : ";
     
     if(DoorSensor == 67)  // 67 -> C
         dataString4 += " CLOSE";
     else if(DoorSensor == 79) // 79 -> O
         dataString4 += "OPEN"; 
     else
         dataString4 += "ERR";
         
     dataString4 +="";
     dataString4.toCharArray(str_line4, DISP_CHAR_LEN);
     hcSerial.println(dataString4);
  
     String dataString5 ="DoorState : ";
     
     if(Motion == 69)  // 0 -> 감지되는 정보가 없을 경우
         dataString5 += " Not DetectData Sensor  ";
     else if(Motion == 79) // 1 -> 감지되는 정보가 있을경우
         dataString5 += " !!!!!!!!!! DetectData Sensor !!!!!!!! ";  
     dataString5 += Motion;    
     dataString5 +="";
     dataString5.toCharArray(str_line5, DISP_CHAR_LEN);
     hcSerial.println(dataString5);
  }
}
}
