#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include<Wire.h>
 
const int MPU_addr=0x68;
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
 
int minVal=265;
int maxVal=402;
 
double x;
double y;
double z;
char c ;

const char forward[] = "forward";
const char reverse[] = "reverse";
const char stop[] = "stop";
const char left[] = "left";
const char right[] = "right";

const char dir[5]={'0','1','2','3','4'};
//stop,foward,reverse,left,right

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

 char gyro()
{
  Wire.beginTransmission(MPU_addr);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true);
AcX=Wire.read()<<8|Wire.read();
AcY=Wire.read()<<8|Wire.read();
AcZ=Wire.read()<<8|Wire.read();
int xAng = map(AcX,minVal,maxVal,-90,90);
int yAng = map(AcY,minVal,maxVal,-90,90);
int zAng = map(AcZ,minVal,maxVal,-90,90);
 
x= RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);
y= RAD_TO_DEG * (atan2(-xAng, -zAng)+PI);
z= RAD_TO_DEG * (atan2(-yAng, -xAng)+PI);
 
Serial.print("AngleX= ");
Serial.println(x);
 
Serial.print("AngleY= ");
Serial.println(y);
 
Serial.print("AngleZ= ");
Serial.println(z);
Serial.println("-----------------------------------------");

if((x<350 && x>250) && (y <50  && y>0) && (z<320 && z>250))
{
  Serial.println("left");
   return dir[3];
}
else if((x<70 && x>30) && (y<30 && y >0) && (z<100&& z>50))
{
  Serial.println("right");
  return dir[4];
}
else if((x<380 && x>330) && (y<350 && y >300) && (z<190 && z>150))
{
  Serial.println("forward");
  return dir[1];
}
else if((x<370 && x>330) && (y<70 && y >30) && (z<370&& z>320))
{
  Serial.println("reverse");
  return dir[2];
}
else if((x<380 && x>320) && (y<380 && y >320) && (z<2300&& z>180))
{
  Serial.println("stop");
  return dir[0];
}
}


void setup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

   Wire.begin();
  Serial.begin(115200);

  Wire.begin();
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B);
Wire.write(0);
Wire.endTransmission(true);
}

void loop() {
  c = gyro();
  //const char text[] = "Hello World";
  radio.write(&c, sizeof(c));
  delay(100);
}