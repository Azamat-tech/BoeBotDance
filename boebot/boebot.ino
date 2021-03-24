#include "constants.hpp"

#include <Servo.h>

using namespace std;

bool currSensors[5];
bool prevSensors[5];

class Robot {
    private:
        Servo left;
        Servo right;

        


    public:

    bool buttonBeingPressed;
    

    void attach_wheels();

    void checkIfButtonIsPressed();

    void initializeTheRobot();

    void setWheels(int leftWheel, int rightWheel);

    void stopWheels();
    void forward();
    void backward();
    void turnLeft();
    void turnRight();
    
};

void Robot::attach_wheels(){
  left.attach(left_wheel);
  right.attach(right_wheel);
}

void Robot::checkIfButtonIsPressed(){
  bool buttonState = (digitalRead(button) == 0);
  if(buttonState && !buttonBeingPressed){
    buttonBeingPressed = true;
  }else if(buttonState && buttonBeingPressed){
    buttonBeingPressed = false;
  }
}

void Robot::initializeTheRobot(){
  stopWheels();
  buttonBeingPressed = false;
}

void Robot::setWheels(int leftWheel, int rightWheel){
  left.writeMicroseconds(leftWheel);
  right.writeMicroseconds(rightWheel);
}

void Robot::stopWheels(){
  setWheels(1500, 1500);
}

void Robot::forward(){
  setWheels(1700, 1300);
}

void Robot::backward(){
  setWheels(1300, 1700);
}

void Robot::turnLeft(){
  setWheels(1300, 1300);
}

void Robot::turnRight(){
  setWheels(1700, 1700);
}

Robot boebot;

String dance_coordinates = "a1n c2 t120 d4 t0 b5 t0 a2 t368 e2 t452 \
                            1c t0 e1 t600";

String trimmed_dance_coordinates = "";

void trim_coordinates(){
    for(size_t i = 0; i < dance_coordinates.length(); i++){
        if( dance_coordinates[i] != '\n' && dance_coordinates[i] != '\t' &&
            dance_coordinates[i] != ' ' && dance_coordinates[i] != ',' &&
            dance_coordinates[i] != ';'){
                trimmed_dance_coordinates += tolower(dance_coordinates[i]);
            }
    }
}

void updateSensors(bool (&sensorArr)[5]) {
  for(size_t i = 0; i < 5; i ++ ) sensorArr[i] = digitalRead(sensors[i]) == 0;
}

void setup() {
  boebot.attach_wheels();
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  boebot.initializeTheRobot();
  trim_coordinates();
  

}

void loop() {
  memcpy(prevSensors, currSensors, sizeof(prevSensors));
  updateSensors(currSensors);
  
  boebot.checkIfButtonIsPressed();
  if(boebot.buttonBeingPressed){
    boebot.forward();
  }else if(!boebot.buttonBeingPressed) {
    boebot.stopWheels();
  }
}
