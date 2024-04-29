#include<Wire.h>
#include <Braccio.h>
#include <BraccioRobot.h>
#include <Servo.h>


unsigned long previousMillis = 0;  // hopefully this works

const long interval = 25;  // interval at the motor takes to fully wind up/down
bool HOLD = false;
void setup() {
  Serial.begin(9600);
  //------- Motor and robot setup -------
  pinMode(2, OUTPUT); // Can consider the second motor later for left/right control
  pinMode(4, OUTPUT);
  //put second motor input here, when completed
}


void loop() {
  while (Serial.availableForWrite()) {

    char InChar = Serial.read();
    int input = InChar;
    
    unsigned long currentMillis = millis();
    
    if (input == 119) { //  The letter W TO SIGNIFY UP, Need to include both motors when done
      if (currentMillis-previousMillis >= interval){
        previousMillis = currentMillis;
        HOLD = !HOLD;
        if (HOLD==true){
          digitalWrite(2,HIGH);
          digitalWrite(4,HIGH);
        }
        else if (HOLD==false){
          digitalWrite(2,LOW);
          digitalWrite(4,LOW);
        }
        Serial.println("Up");
      }
    }
    if (input == 115) { //  The letter s TO SIGNIFY down
      if (currentMillis-previousMillis >= interval){
        previousMillis = currentMillis;
        HOLD = !HOLD;
        if (HOLD==true){
          digitalWrite(2,HIGH);
          digitalWrite(4,HIGH);
        }
        else if (HOLD==false){
          digitalWrite(2,LOW);
          digitalWrite(4,LOW);
        }
        Serial.println("Down");
      }
    }
    else if (input == 97) { // The letter A
        if (currentMillis-previousMillis >= interval){
          previousMillis = currentMillis;
          digitalWrite(2,HIGH);
        }
        Serial.println("Left");
    }
    else if (input == 100) { // The letter d, change when adding second motor
        if (currentMillis-previousMillis >= interval){
          previousMillis = currentMillis;
          digitalWrite(4,HIGH);
        }
        Serial.println("Right");
    }
    if (currentMillis-previousMillis >= interval*2){
      previousMillis = currentMillis;
      digitalWrite(2,LOW);
      digitalWrite(4,LOW);
      HOLD = false;
    }
    // This is currently written for one motor, as the risks of wires breaking was too great and I dont want to break multiple motors

  }
}