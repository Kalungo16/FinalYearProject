#include<Wire.h>
#include <Braccio.h>
#include <BraccioRobot.h>
#include <Servo.h>

const int MPU_addr=0x68; // I2C address of the MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

Servo base; // Names of motors within the arm
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

Position pos;
int m1= 90;  // M1=base degrees. Allowed values from 0 to 180 degrees
int m2= 90;  // M2=shoulder degrees. Allowed values from 15 to 165 degrees
int m3= 90;// M3=elbow degrees. Allowed values from 0 to 180 degrees -- Set to 0 as its currently broken
int m4 = 180;  // M4=wrist degrees. Allowed values from 0 to 180 degrees
int m5 = 90;  // M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
int m6 = 45;  // M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the gripper is open, 73: the gripper is closed.

void setup() {
  // ------Gyro and accelerometer setup------
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); // PWR_MGMT_1 register
  Wire.write(0); // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(9600);

  //------- Motor and robot setup -------
  pinMode(2, OUTPUT);// FOR UNO
 // pinMode(4, OUTPUT);// FOR NANO
  //pinMode(13,OUTPUT); // for UNO
  BraccioRobot.init(); //Variation 1
}

String Mselect = "m1";// Default motor select variable
float compX=0;
float compY=0;
float compZ=0;

void loop() {
  while (true){
    // This is currently written for one motor, as the risks of wires breaking was too great and I dont want to break multiple motors
    digitalWrite(2,LOW);
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr,14,true); // request a total of 14 registers
    /*---- Accelerometer is all that is needed to produce the results necessary
    ------ to be able to get the difference and maybe do maths on said sensor */
    AcX=(Wire.read()<<8|Wire.read())/1000; // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
    AcY=(Wire.read()<<8|Wire.read())/1000; // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
    AcZ=(Wire.read()<<8|Wire.read())/1000; // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    //comparison statements to move the motors based on movement
    float delX=compX-AcX;
    float delY=compY-AcY;
    float delZ=compZ-AcZ;
    if (abs(delX)>=4){ // tolerance shift delay weightings, the higher the number in the if statement, the higher the tolerance
      digitalWrite(2,HIGH);
      Serial.print("This is the X axis:");
      Serial.println(abs(delX));
      //Continuous vibrations to show X axis rotation
    }
    if (abs(delY)>=4){ // tolerance shift delay weightings, the higher the number in the if statement, the higher the tolerance
      digitalWrite(2,HIGH);
      Serial.print("This is the Y axis:");
      Serial.println(abs(delY));
      // Short vibrations to signify the up/down movment
    }
    if (abs(delZ)>=6){ // tolerance shift delay weightings, the higher the number in the if statement, the higher the tolerance
      digitalWrite(2,HIGH);
      Serial.print("This is the Z axis:" );
      Serial.println(abs(delZ));
    }
    
    compX=AcX;
    compY=AcY;
    compZ=AcZ;
    delay (1000);
    //Serial.println(compX);
  }
}