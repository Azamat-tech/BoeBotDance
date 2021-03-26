#include "constants.hpp"
#include <string>

// #include <Servo.h>

using namespace std;

enum DIR { UP, DOWN, LEFT, RIGHT};

// Position Class that holds the current position of the robot 
class Position {
  public:
    bool horizontalFirst; // if the position of the dance starts with letter
    int xPos;
    int yPos;
    DIR direction;

    void SetPositions(string position); 
};

enum DIR GetTheDirections(char direction) {
        return direction == 'n' ? UP :
        direction == 'e' ? LEFT :
        direction == 'w' ? RIGHT :
        DOWN;
}



// setPosition sets (x,y) position from string pos. e.g e2 => (4,1)
void Position::SetPositions(string pos){
  if(isalpha(pos[0])){
    horizontalFirst = true;
  }
  switch(pos[0]){
    case 'a':
      xPos = 0;
      break;
    case 'b':
      xPos = 1;
      break;
    case 'c':
      xPos = 2;
      break;
    case 'd':
      xPos = 3;
      break;
    case 'e':
      xPos = 4;
      break;
  }
  switch(pos[1]){
    case 1:
      yPos = 0;
      break;
    case 2:
      yPos = 1;
      break;
    case 3:
      yPos = 2;
      break;
    case 4:
      yPos = 3;
      break;
    case 5:
      yPos = 4;
      break;
  }
}

// class TimeDelay that holds the number of seconds to delay at each destination
class TimeDelay {
  public:
    int seconds;
    
    void SetSeconds(int time);
};

void TimeDelay::SetSeconds(int time) {
  seconds = time / 10;
}

// Robot class
class Robot {
    private:
        // Servo left;
        // Servo right;
    public:

    bool buttonBeingPressed;

    Position currentPosition;
    Position nextPosition;
    
    void AttachWheels();
    void CheckIfButtonIsPressed();
    void InitializeTheRobot();
    void GetToTheNextPosition();
    void SetWheels(int leftWheel, int rightWheel);
    void StopWheels();
    void Forward();
    void Backward();
    void TurnLeft();
    void TurnRight();
    
    void GoForwardAndAlign();
};

void Robot::AttachWheels(){
  // left.attach(left_wheel);
  // right.attach(right_wheel);
}

void Robot::CheckIfButtonIsPressed() {
  // bool buttonState = (digitalRead(button) == 0);
  // if(buttonState && !buttonBeingPressed){
  //   buttonBeingPressed = true;
  // }else if(buttonState && buttonBeingPressed){
  //   buttonBeingPressed = false;
  // }
}

void Robot::InitializeTheRobot() {
  StopWheels();
  boebot.currentPosition.SetPositions(startingPosition);
  buttonBeingPressed = false;
}

void Robot::GoForwardAndAlign() {
  
}



void Robot::GetToTheNextPosition() {
  // From currentPosition -> nextPosition
  if(nextPosition.horizontalFirst) {
    if(currentPosition.xPos == nextPosition.xPos) {
      if(currentPosition.direction == UP) {
        if(currentPosition.yPos < nextPosition.yPos) {
          // go forward and align
        }
        else if (currentPosition.yPos > nextPosition.yPos) {
          if(currentPosition.xPos == 0) {
            // rotate backwards from rightSide
          }
          else if(currentPosition.xPos == 4) {
            // rotate backwards from leftSide
          }
          else {
            // rotate backwards from (left or right)
          }
          // go foward
        }
      }
      else if(currentPosition.direction == DOWN) {
        if(currentPosition.yPos < nextPosition.yPos) {
          if(currentPosition.xPos == 0) {
            // rotate backwards from leftSide
          }
          else if(currentPosition.xPos == 4) {
            // rotate backwards from rightSide
          }
          else {
            // rotate backwards from (left or right)
          }
          // go foward
        }
        else if (currentPosition.yPos > nextPosition.yPos) {
          // go forward
        }
      }
      // else if(currentPosition.direction == LEFT) {

      // }
      // else if(currentPosition.direction == RIGHT) {

      // }
    }
  }
  else{

  }
}

// void Robot::SetWheels(int leftWheel, int rightWheel){
//   left.writeMicroseconds(leftWheel);
//   right.writeMicroseconds(rightWheel);
// }

// void Robot::StopWheels(){
//   setWheels(1500, 1500);
// }

// void Robot::Forward(){
//   setWheels(1700, 1300);
// }

// void Robot::Backward(){
//   setWheels(1300, 1700);
// }

// void Robot::TurnLeft(){
//   setWheels(1300, 1300);
// }

// void Robot::TurnRight(){
//   setWheels(1700, 1700);
// }

Robot boebot; 

bool currSensors[5];
bool prevSensors[5];

int moveIndex;

string danceCoordinates = "a1n a2t0 a3t0 a40 a5t0";
string trimmedDanceCoordinates = "";
string nextPosition = "";
string positionTimeDelay = "";
string startingPosition = trimmedDanceCoordinates.substr(0,3);

void TrimCoordinates(){
    for(size_t i = 0; i < danceCoordinates.length(); i++){
        if( danceCoordinates[i] != '\n' && danceCoordinates[i] != '\t' &&
            danceCoordinates[i] != ' ' && danceCoordinates[i] != ',' &&
            danceCoordinates[i] != ';'){
                trimmedDanceCoordinates += tolower(danceCoordinates[i]);
            }
    }
}

// void UpdateSensors(bool (&sensorArr)[5]) {
//   for(size_t i = 0; i < 5; i ++ ) sensorArr[i] = digitalRead(sensors[i]) == 0;
// }

bool IsTheEndOfDance() {
  return trimmedDanceCoordinates.length() == moveIndex;
}


string GetTheNextDestination(){
  string toReturn = "";
  while(true){
    if(danceCoordinates[moveIndex] == 't') {
      break; 
    }
    toReturn += danceCoordinates[moveIndex];
    moveIndex++;
  }
  return toReturn;
}

string GetTheNextDestinationDelay() {
  string toReturn = "";
  while(true) {
    if(danceCoordinates[moveIndex] == ' ') {
      moveIndex++;
      break;
    }
    toReturn += danceCoordinates[moveIndex];
    moveIndex++;
  }
  return toReturn;
}

void setup() {
  // pinMode(button, INPUT_PULLUP);
  // pinMode(led, OUTPUT);
  boebot.AttachWheels();
  boebot.InitializeTheRobot();  // initializes the robot at the start position
  TrimCoordinates();
  moveIndex = 4;  // start reading the next move
}

void loop() {
  // memcpy(prevSensors, currSensors, sizeof(prevSensors));
  // UpdateSensors(currSensors);
  
  boebot.CheckIfButtonIsPressed();
  if(boebot.buttonBeingPressed){
    if(!IsTheEndOfDance()){
      nextPosition = GetTheNextDestination();
      positionTimeDelay = GetTheNextDestinationDelay();
      boebot.nextPosition.SetPositions(nextPosition);
      
    }
  }else if(!boebot.buttonBeingPressed) {
    boebot.StopWheels();
  }
}
