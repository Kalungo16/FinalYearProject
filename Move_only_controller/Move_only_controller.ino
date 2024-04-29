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
THIS WORKS*/

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
int m3= 90;  // M3=elbow degrees. Allowed values from 0 to 180 degrees -- Set to 0 as its currently broken
int m4 = 180;  // M4=wrist degrees. Allowed values from 0 to 180 degrees
int m5 = 90;  // M5=wrist rotation degrees. Allowed values from 0 to 180 degrees
int m6 = 10;  // M6=gripper degrees. Allowed values from 10 to 73 degrees. 10: the gripper is open, 73: the gripper is closed.
int moCount = 1; // initialise the motor at motor number 1
void setup() {
  Serial.begin(9600);

  //------- Motor and robot setup -------
  //pinMode(2, OUTPUT);// FOR NANO
  //pinMode(4, OUTPUT);// FOR NANO
  pinMode(13,OUTPUT); // for UNO
  BraccioRobot.init(); //Variation 1
}

String Mselect = "m1";// Default motor select variable


void loop() {
  
  // add a delay in here to slow the speed of result acquisition down


  pos.set(m1,  m2, m3, m4,  m5,  m6); //Required to move the robot
  BraccioRobot.moveToPosition(pos, 100); 

  // This section of code to attempt to control the arm
  while (Serial.availableForWrite()) {

    char InChar = Serial.read();
    int input = InChar;
    //digitalWrite(2,LOW); //Nano
    //digitalWrite(4,LOW); //Nano
    digitalWrite(13,LOW); //Uno

    if (input== 113){
      // if q is selected, cycle through the motor selection to the left (counting down)
      moCount = moCount-1;
      if (moCount==0){
        moCount=6;// if the motor selection gets too low, reset the count
      }
      Mselect= MotorSelect(moCount);
      //I can likely rewrite this as a function, I may as well
    }
    if (input==114){
      moCount = moCount+1;
      if (moCount==7){
        moCount=1;// if the motor selection gets too low, reset the count
      }
      Mselect= MotorSelect(moCount);
    }
    
    if (input=='w' || input=='a' || input=='s' ||input=='d'){ //At current, this section will only code for the motors which work
      if (Mselect =="m1"){
        if (input=='a' && m1<180){
          m1+=1;
         
        }
        else if (input=='d' && m1>0){
          m1-=1;
          
        }
      }
      else if (Mselect =="m2"){
        if (input=='a' && m2<165){
          m2+=1;
          
        }
        else if (input=='d' && m2>15){
          m2-=1;
        
        }
      }
      else if (Mselect =="m3"){
        if (input=='a' && m3<165){
          m3+=1;
          
        }
        else if (input=='d' && m3>15){
          m3-=1;
         
        }
      }
      else if (Mselect =="m4"){
        if (input=='w' && m4<180){
          m4+=1;
         }
        else if (input=='s' && m4>0){
          m4-=1;
        }
      }
      else if (Mselect== "m5"){
        if (input=='a' && m5<180){
          m5+=1;

        }
        else if (input=='d' && m5>0){
          m5-=1;

        }
      }
      else if (Mselect == "m6"){
        if (input=='w' && m6<73){
          m6+=1;

        }
        else if (input=='s' && m6>10){
          m6-=1;

        }
      }
      break;
      
    }
    break;
  }
  
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
String MotorSelect( int Msel){
    String Mnum="m1";
      if (Msel==1){// m1 selected
        Mnum= "m1";
        Serial.println(Mnum + " selected \n");
      }
      else if (Msel==2){// m2 selected
        Mnum = "m2";
        Serial.print(Mnum + " selected \n");
      }
      else if (Msel==3){// m3 selected
        Mnum = "m3";
        Serial.println(Mnum + " selected \n");
      }
      else if (Msel==4){// m4 selected
        Mnum = "m4";
        Serial.println(Mnum + " selected \n");
      }
      else if (Msel==5){// m5 selected
        Mnum = "m5";
        Serial.println(Mnum + " selected \n");
      }
      else if (Msel==6){// m6 selected
        Mnum = "m6";
        Serial.println(Mnum + " selected \n");
      }
      return (Mnum);

}


