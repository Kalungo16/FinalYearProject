/* Alexander Merry, ID:2173596, University of Birmingham
FINAL YEAR PROJECT ARDUINO CODE */

/* TO DO: 
Finish control Mechanism by adding options for different motors and directional movement
Figure out a viable 'speed' for the user to react to
incorporate sensors into postioning
Create a motor rig
Add motors into the motor rig --  May need to make a second document to test out vibration and frequency
Talk to Ethan about potential control */

/* -----------The enter macro idea-------------
for this to work, there has to be a way to input an enter into the serial port 
every single time there is an input to improve fluidity in the case of the user 
THIS WORKS, using a python script*/

#include <Wire.h>
#include <Braccio.h>
#include <BraccioRobot.h>
#include <Servo.h>

const int MPU_addr = 0x68;  // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

Servo base;  // Names of motors within the arm
Servo shoulder;
Servo elbow;
Servo wrist_ver;
Servo wrist_rot;
Servo gripper;

Position pos;

unsigned long previousMillis = 0;

const long interval = 25;  // check interval for the positional vibrations, granted they work

bool HOLD = false;
int m1 = 90;      // M1=base degrees. Allowed values from 0 to 180 degrees
int m2 = 90;      // M2=shoulder degrees. Allowed values from 15 to 165 degrees
int m3 = 90;      // M3=elbow degrees. Allowed values from 0 to 180 degrees
int m4 = 180;     // M4=wrist degrees. Allowed values from 0 to 180 degrees
int m5 = 90;      // M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
int m6 = 73;      // M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the gripper is open, 73: the gripper is closed.
int moCount = 1;  // initialise the motor at motor number 1

void setup() {
  // ------Gyro and accelerometer setup------
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);

  Serial.begin(9600);

  //------- Motor and robot setup -------
  pinMode(2, OUTPUT);   // FOR LEFT
  pinMode(4, OUTPUT);   // FOR RIGHT
  BraccioRobot.init();  //Variation 1
}

String Mselect = "m1";  // Default motor select variable
float compX = 0;
float compY = 0;
float compZ = 0;
float CGx=0,CGy=0,CGz=0;

void loop() {
  unsigned long currentMillis = millis();  // sets up the positional change interval
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true);  // request a total of 14 registers
  /*---- Gyro is all that is needed to produce the results necessary, the labelling sucks
  ------ to be able to get the difference and maybe do maths on said sensor */
  AcX = (Wire.read() << 8 | Wire.read()) / 1000;  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = (Wire.read() << 8 | Wire.read()) / 1000;  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = (Wire.read() << 8 | Wire.read()) / 1000;  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read()<<8|Wire.read();
  GyX = Wire.read()<<8|Wire.read();
  GyY = Wire.read()<<8|Wire.read();
  GyZ = Wire.read()<<8|Wire.read();
  //Serial.print("GyX = "); Serial.print(GyX);
  //Serial.print(" | GyY = "); Serial.print(GyY);
  //Serial.print(" | GyZ = "); Serial.println(GyZ); 
                                                  //Serial.print("AcX = "); Serial.print(AcX);// this is tilt rotationally side to side. X is nearest 0 when it is Vertical
                                                  //Serial.print(" | AcY = "); Serial.print(AcY); // this is tilt for forwards/backwards or upwards/downwards. Y is nearest 0 when it is flat.
                                                  //Serial.print(" | AcZ = "); Serial.println(AcZ); // this is for literal rotation. Z is nearest 0 when it is sideways
  float delX = compX - AcX;
  float delY = compY - AcY;
  float delZ = compZ - AcZ;
  float dgX = CGx - GyX;
  float dgY = CGy - GyY;
  float dgZ = CGz - GyZ;
  // add a delay in here to slow the speed of result acquisition down


  pos.set(m1, m2, m3, m4, m5, m6);  //Required to move the robot
  BraccioRobot.moveToPosition(pos, 100);

  // This section of code to attempt to control the arm
  while (Serial.availableForWrite()) {

    char InChar = Serial.read();
    int input = InChar;
    if (currentMillis - previousMillis >= interval * 2) {
      previousMillis = currentMillis;
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
      HOLD = false;
    }
    //digitalWrite(2,LOW); //Uno
    //digitalWrite(4,LOW); //Nano

    if (input == 113) {
      // if q is selected, cycle through the motor selection to the left (counting down)
      moCount = moCount - 1;
      if (moCount == 0) {
        moCount = 6;  // if the motor selection gets too low, reset the count
      }
      Mselect = MotorSelect(moCount);
    }
    if (input == 114) {
      moCount = moCount + 1;
      if (moCount == 7) {
        moCount = 1;  // if the motor selection gets too low, reset the count
      }
      Mselect = MotorSelect(moCount);
    }


    if (input == 'w' || input == 'a' || input == 's' || input == 'd') {  //At current, this section will only code for the motors which work
      compX = AcX;
      compY = AcY;
      compZ = AcZ;  // May need to move these around the code for the comparison to work
      CGx = GyX;
      CGy = GyY;
      CGz = GyZ;
      if (Mselect == "m1") {
        if (input == 'a' && m1 > 0) {
          m1 -= 1;
          if (currentMillis - previousMillis >= interval) {
            skillcheck(dgX,dgY,dgZ,input);
          }
          //poscheck(delX,delY,delZ); //need to change position, but not measure every cycle
        } else if (input == 'd' && m1 < 180) {
          m1 += 1;
          if (currentMillis - previousMillis >= interval) {
            skillcheck(dgX,dgY,dgZ,input);
          }
          //poscheck(delX,delY,delZ);
        }
      } else if (Mselect == "m2") {
        if (input == 'w' && m2 < 165) {
          m2 += 1;
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            poscheck(delX, delY, delZ);
          }
          // digitalWrite(4,HIGH); // ? for UNO, 4 for NANO
          /*if (m2==165){
            delay(500);
            digitalWrite(4,LOW);
            delay(200);
            digitalWrite(4,HIGH);
            delay(500);
          }*/
        } else if (input == 's' && m2 > 15) {
          m2 -= 1;
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            poscheck(delX, delY, delZ);
          }
          // poscheck(delX,delY,delZ);
          //digitalWrite(13,HIGH); // ? for UNO, 4 for NANO
          /*if (m2==15){
            delay(500);
            digitalWrite(4,LOW);
            delay(200);
            digitalWrite(4,HIGH);
            delay(500);
          }*/
        }
      } else if (Mselect == "m3") {
        if (input == 'w' && m3 < 165) {
          m3 += 1;
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            poscheck(delX, delY, delZ);
          }
          //poscheck(delX,delY,delZ);
        } else if (input == 's' && m3 > 15) {
          m3 -= 1;
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            poscheck(delX, delY, delZ);
          }
          //poscheck(delX,delY,delZ);
        }
      } else if (Mselect == "m4") {
        if (input == 'w' && m4 < 180) {
          m4 += 1;
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            poscheck(delX, delY, delZ);
          }
          //poscheck(delX,delY,delZ);
          //digitalWrite(2,HIGH); // 13 for UNO, 2 for NANO
          /*if (m4==180){
            delay(500);
            digitalWrite(2,LOW);
            delay(200);
            digitalWrite(2,HIGH);
            delay(500);
          }*/
          //digitalWrite(13,HIGH ); // this currently produces a vibration when the key is held!! very good.
        } else if (input == 's' && m4 > 0) {
          m4 -= 1;
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            poscheck(delX, delY, delZ);
          }
          //poscheck(delX,delY,delZ);
          //digitalWrite(13,HIGH);// 13 for UNO, 2 for NANO
          /*if (m4==0){
            delay(500);
            digitalWrite(2,LOW);
            delay(200);
            digitalWrite(2,HIGH);
            delay(500);
          }*/
          //digitalWrite(13,HIGH );  //13 for UNO, 2 for NANO
        }
      } else if (Mselect == "m5") {
        if (input == 'a' && m5 < 180) {
          m5 += 1;
          if (currentMillis - previousMillis >= interval) {
            skillcheck(dgX,dgY,dgZ,input);
          }
          //poscheck(delX,delY,delZ);
        } else if (input == 'd' && m5 > 0) {
          m5 -= 1;
          if (currentMillis - previousMillis >= interval) {
            skillcheck(dgX,dgY,dgZ,input);
          }
          //poscheck(delX,delY,delZ);
        }
      } else if (Mselect == "m6") {
        if (input == 'w' && m6 < 73) {
          m6 += 1;
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            HOLD = !HOLD;
            if (HOLD == true) {
              digitalWrite(2, HIGH);
              digitalWrite(4, HIGH);
            } else if (HOLD == false) {
              digitalWrite(2, LOW);
              digitalWrite(4, LOW);
            }
          }
          //poscheck(delX,delY,delZ);
        } else if (input == 's' && m6 > 10) {
          m6 -= 1;
          if (currentMillis - previousMillis >= interval) {
            previousMillis = currentMillis;
            HOLD = !HOLD;
            if (HOLD == true) {
              digitalWrite(2, HIGH);
              digitalWrite(4, HIGH);
            } else if (HOLD == false) {
              digitalWrite(2, LOW);
              digitalWrite(4, LOW);
            }
            //poscheck(delX,delY,delZ);
          }

        }

        break;
      }

      break;
    }
    break;
    

    // will consider developing this code later to take position and apply it to the motor
    /* Serial.print("Base ");
  Serial.println(pos.getBase()); //M1, Base
  Serial.print("Shoulder ");
  Serial.println(pos.getShoulder()); //M2, Shoulder
  Serial.print("Elbow ");
  Serial.println(pos.getElbow()); //M3, Broken elbow
  Serial.print("Wrist UP/DOWN ");
  Serial.println(pos.getWrist()); //M4, Wrist UPDOWN
  Serial.print("Wrist Rotation ");
  Serial.println(pos.getWristRotation()); //M5, Wrist Rotation
  Serial.print("Gripper ");
  Serial.println(pos.getGripper()); //M6, THE CLAW*/
  }
  
}

int poscheck(int delX, int delY, int delZ) {
  // How do I get the X-axis movement to actually make a difference??
  //Re-add the X coordinate system when you can figure out a final value
  //if (abs(delX)>=4){ // tolerance shift delay weightings, the higher the number in the if statement, the higher the tolerance
  // digitalWrite(2,HIGH); // this is for the motor vibration
  // Serial.print("This is the X axis:");
  //Serial.println(abs(delX));
  //Continuous vibrations to show X axis rotation
  // }
  if (abs(delY) >= 2) {  // tolerance shift delay weightings, the higher the number in the if statement, the higher the tolerance
    Serial.print("This is the Y axis:");
    Serial.println(abs(delY));
    HOLD = !HOLD;
    // Short vibrations to signify the up/down movment
    if (HOLD == true) {
      digitalWrite(2, HIGH);  //check pin
      digitalWrite(4, HIGH);
    } else if (HOLD == false) {
      digitalWrite(2, LOW);
      digitalWrite(4, LOW);
    }
  }
  //Only use the Z axis if you can figure out the need for it
  //if (abs(delZ)>=6){ // tolerance shift delay weightings, the higher the number in the if statement, the higher the tolerance
  //digitalWrite(2,HIGH);
  //Serial.print("This is the Z axis:" );
  //Serial.println(abs(delZ));
  //Sometimes the Z axis and X axis get confused, so both are being added.
  // }
}
int skillcheck(int dgX,int dgY,int dgZ, char rec){
  if (rec=='a'){ // tolerance shift delay weightings, the higher the number in the if statement, the higher the tolerance
   digitalWrite(2,HIGH); // this is for the motor vibration
  }else if (rec=='d'){
    digitalWrite(4,HIGH);
  }  //Continuous vibrations to show X axis rotation
 }
String MotorSelect(int Msel) {
  String Mnum = "m1";
  if (Msel == 1) {  // m1 selected
    Mnum = "m1";
    Serial.println(Mnum + " selected \n");
  } else if (Msel == 2) {  // m2 selected
    Mnum = "m2";
    Serial.print(Mnum + " selected \n");
  } else if (Msel == 3) {  // m3 selected
    Mnum = "m3";
    Serial.println(Mnum + " selected \n");
  } else if (Msel == 4) {  // m4 selected
    Mnum = "m4";
    Serial.println(Mnum + " selected \n");
  } else if (Msel == 5) {  // m5 selected
    Mnum = "m5";
    Serial.println(Mnum + " selected \n");
  } else if (Msel == 6) {  // m6 selected
    Mnum = "m6";
    Serial.println(Mnum + " selected \n");
  }
  return (Mnum);
}