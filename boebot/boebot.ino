#include "constants.hpp"

using namespace std;

#define l digitalWrite(11, 0);
#define k digitalWrite(11,1);

enum DIR { UP, DOWN, LEFT, RIGHT};

bool currSensors[5];
bool prevSensors[5];

bool serialInputgiven;
bool serialInputInitialized;

unsigned long currentTime = millis();
unsigned long waitingTime = millis();

unsigned long finalRotationTime;
unsigned long finalRotationStartedFrom;

int moveIndex;
unsigned long ak = millis();
int val = 0;

String danceCoordinates =
" A1W\
a1 t38\
3b t100\
2c t195\
e1 t0\
5e t367\
4d t0\
5e t0\
4d t0\
5e t0\
c4 t0\
5a t760\
a3 t838\
b4 t916\
a2 t960\
b3 t1074\
a2 t0\
1e t0\
a1 t0";
String nextPositionFromString = "";
int positionTimeDelay;
String startingPosition = "";
String serialDanceCoordinates = "";

class Position {
  public:
    bool horizontalFirst;
    int xPos;
    int yPos;
    DIR direction;

    void SetPositions(char x, char y){
      if(isalpha(x)){
        horizontalFirst = true;
      }
      else {
        horizontalFirst = false;
        SwitchHorizontalAndVertical(x, y);
      }
      switch(x){
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
      switch(y){
        case '1':
          yPos = 0;
          break;
        case '2':
          yPos = 1;
          break;
        case '3':
          yPos = 2;
          break;
        case '4':
          yPos = 3;
          break;
        case '5':
          yPos = 4;
          break;
      }
    }

    void SwitchHorizontalAndVertical(char & x, char & y) {
      char temp = y;
      y = x;
      x = temp;
    }
};

enum DIR GetTheDirections(char direction) {
  return direction == 'n' ? UP :
         direction == 'e' ? RIGHT :
         direction == 'w' ? LEFT :
         DOWN;
}

// Robot class
class Robot {
    private:
        Servo left;
        Servo right;
        unsigned long extraMovingTime;
        bool sawNewLine;
        bool sawLineWhileRotating;
        bool rotationAligned;
        int numberOfEncountersOfLine;
        int timeForAxisRotation;

    public:
      bool buttonBeingPressed;
      int numberOfButtonPresses;
      Position currentPosition;
      Position nextPosition;

      void AttachWheels(){
        left.attach(left_wheel);
        right.attach(right_wheel);
      }

      void CheckIfButtonIsPressed() {
        bool buttonState = (digitalRead(button) == 0);
        if(buttonState && !buttonBeingPressed){
          buttonBeingPressed = true;
        }else if(!buttonState && buttonBeingPressed){
          buttonBeingPressed = false;
          numberOfButtonPresses = (numberOfButtonPresses + 1) ;
        }
      }

      void _InitializeTheRobot() {
        extraMovingTime = millis();
        numberOfButtonPresses = 0;
        numberOfEncountersOfLine = 0;
        buttonBeingPressed = false;
        sawNewLine = false;
        sawLineWhileRotating = false;
        rotationAligned = false;
        timeForAxisRotation = 550;
      }

      void InitializeTheRobot(String startingPosition, int & positionTimeDelay) {
        StopWheels();
        currentPosition.SetPositions(startingPosition[0], startingPosition[1]);
        currentPosition.direction = GetTheDirections(startingPosition[2]);
        nextPosition.SetPositions(startingPosition[0], startingPosition[1]);
        nextPosition.direction = GetTheDirections(startingPosition[2]);
        _InitializeTheRobot();
        positionTimeDelay = 0;
      }

      void SetWheels(int leftWheel, int rightWheel){
        left.writeMicroseconds(leftWheel);
        right.writeMicroseconds(rightWheel);
      }

      void StopWheels(){
        SetWheels(1500, 1500);
      }

      void Forward(){
        SetWheels(1700, 1300);
      }

      void Backward(){
        SetWheels(1300, 1700);
      }

      void TurnLeft(){
        SetWheels(1400, 1400);
      }

      void TurnRight(){
        SetWheels(1600, 1600);
      }

      void GoForwardAndAlign(bool sensors[5], bool prevSensors[5], bool isHorizontal = true) {

        if(sawNewLine && millis() >= extraMovingTime + 250) {
          sawNewLine = false;
          extraMovingTime = millis();
          if(currentPosition.direction == UP) {
            currentPosition.yPos++;
          }
          else if(currentPosition.direction == DOWN) {
            currentPosition.yPos--;
          }
          else if(currentPosition.direction == LEFT) {
            currentPosition.xPos--;
          }
          else if(currentPosition.direction == RIGHT) {
            currentPosition.xPos++;
          }
        }
        else if(sawNewLine && millis() < extraMovingTime + 250){
          Forward();
          return;
        }

        if (!sawNewLine && ((sensors[4] && !prevSensors[4]) || (sensors[0] && !prevSensors[0]))) {
          extraMovingTime = millis();
          sawNewLine = true;
        }
        if((sensors[2] && !sensors[1] && !sensors[3]) ||(sensors[2] && sensors[0] && sensors[1]) || ((sensors[2] && sensors[3] && sensors[4])) ||
            (sensors[2] && sensors[0] && sensors[1] && sensors[3] && sensors[4]) || (!sensors[2] && !sensors[0] && !sensors[1] && !sensors[3] && !sensors[4])) {
          Forward();
        }
        else if((sensors[2] && sensors[3]) || (sensors[3] && !sensors[2])) {
          TurnLeft();
        }
        else if((sensors[2] && sensors[1]) || (sensors[1] && !sensors[2])) {
          TurnRight();
        }
      }

      void RotateRight(bool sensors[5], bool prevSensors[5]) {
        if(sawLineWhileRotating && millis() >= extraMovingTime + 250) {
          sawLineWhileRotating = false;
          extraMovingTime = millis();
          rotationAligned = true;
          StopWheels();
        }
        if(sensors[0] && !prevSensors[0]) {
          extraMovingTime = millis();
          sawLineWhileRotating = true;
        }
        if(!rotationAligned) {
          TurnRight();
        }
      }

      void RotateLeft(bool sensors[5], bool prevSensors[5]) {
        if(sawLineWhileRotating && millis() >= extraMovingTime + 250) {
          sawLineWhileRotating = false;
          extraMovingTime = millis();
          rotationAligned = true;
          StopWheels();
        }
        if(sensors[4] && !prevSensors[4]) {
          extraMovingTime = millis();
          sawLineWhileRotating = true;
        }
        if(!rotationAligned) {
          TurnLeft();
        }
      }

      void RotateBackWards(bool sensors[5], bool prevSensors[5], bool fromRight = true) {
        if(fromRight) {
          if(numberOfEncountersOfLine == 2 && millis() >= extraMovingTime + 250) {
            extraMovingTime = millis();
            rotationAligned = true;
            StopWheels();
            numberOfEncountersOfLine = 0;
          }
          if(sensors[0] && !prevSensors[0]) {
            numberOfEncountersOfLine++;
            if(numberOfEncountersOfLine == 2) {
              extraMovingTime = millis();
            }
          }
          if(!rotationAligned) {
            TurnRight();
          }
        }else {
          if(numberOfEncountersOfLine == 2 && millis() >= extraMovingTime + 250) {
            extraMovingTime = millis();
            rotationAligned = true;
            StopWheels();
            numberOfEncountersOfLine = 0;
          }
          if(sensors[4] && !prevSensors[4]) {
            numberOfEncountersOfLine++;
            if(numberOfEncountersOfLine == 2) {
              extraMovingTime = millis();
            }
          }
          if(!rotationAligned) {
            TurnLeft();
          }
        }
      }

      void FinalOrientationAlignment(bool sensors[5], bool prevSensors[5]) {
        if(currentPosition.direction == nextPosition.direction){
          _InitializeTheRobot();
          InitializeStringHelpers();
          return;
        }

        DIR currentDirection = currentPosition.direction;
        DIR desiredDirection = nextPosition.direction;

        int finalRotationTime = (currentDirection == RIGHT && desiredDirection == LEFT) ||
                                (currentDirection == LEFT && desiredDirection == RIGHT) ||
                                (currentDirection == UP && desiredDirection == DOWN) ||
                                (currentDirection == DOWN && desiredDirection == UP) ?
                                2 * timeForAxisRotation : timeForAxisRotation;

        switch(currentDirection) {
          case UP :
            desiredDirection == RIGHT ? TurnRight() : TurnLeft();
            break;
          case DOWN :
            desiredDirection == LEFT ? TurnRight() : TurnLeft();
            break;
          case RIGHT:
            desiredDirection == DOWN ? TurnRight() : TurnLeft();
            break;
          case LEFT:
            desiredDirection == UP ? TurnRight() : TurnLeft();
            break;
        }

        if(millis() >= finalRotationTime + finalRotationStartedFrom) {
          currentPosition.direction = nextPosition.direction;
        }
      }

      void GetToTheNextPosition(bool sensors[5], bool prevSensors[5]) {
        rotationAligned = false;
        if(nextPosition.horizontalFirst) {
          if(currentPosition.xPos != nextPosition.xPos) {

            GetToTheHorizontalPosition(sensors, prevSensors);
          }
          else if(currentPosition.yPos != nextPosition.yPos) {

            GetToTheVerticalPosition(sensors, prevSensors);
          }
        }
        else if(!nextPosition.horizontalFirst){

          if(currentPosition.yPos != nextPosition.yPos) {
            GetToTheVerticalPosition(sensors, prevSensors);
          }
          else if(currentPosition.xPos != nextPosition.xPos) {
            GetToTheHorizontalPosition(sensors, prevSensors);
          }
        }
      }

      void GetToTheHorizontalPosition(bool sensors[5], bool prevSensors[5]) {
        char goalDirection;
        if(currentPosition.xPos < nextPosition.xPos) {
          goalDirection = 'r';
          TurnBasedOnDirection(goalDirection, sensors, prevSensors);
        }
        else if(currentPosition.xPos > nextPosition.xPos) {
          goalDirection = 'l';
          TurnBasedOnDirection(goalDirection, sensors, prevSensors);
        }
        if(rotationAligned) {
          GoForwardAndAlign(sensors, prevSensors, true);
        }
      }

      void GetToTheVerticalPosition(bool sensors[5], bool prevSensors[5]) {
        char goalDirection;
        if(currentPosition.yPos < nextPosition.yPos) {
          goalDirection = 'u';
          TurnBasedOnDirection(goalDirection, sensors, prevSensors);
        }
        else if(currentPosition.yPos > nextPosition.yPos) {
          goalDirection = 'd';
          TurnBasedOnDirection(goalDirection, sensors, prevSensors);
        }
        if(rotationAligned) {
            GoForwardAndAlign(sensors, prevSensors, false);
        }
      }

      void TurnBasedOnDirection(char goalDirection, bool sensors[5], bool prevSensors[5]) {
        DIR robotDirection = currentPosition.direction;

        switch (robotDirection) {
          case UP:
            if (goalDirection == 'r') {
              RotateRight(sensors, prevSensors);
              if (rotationAligned) { currentPosition.direction = RIGHT; }
            }
            else if (goalDirection == 'l') {
              RotateLeft(sensors, prevSensors);
              if (rotationAligned) { currentPosition.direction = LEFT; }
            }
            else if (goalDirection == 'd') {
              if (currentPosition.xPos == 0) { RotateBackWards(sensors, prevSensors, true); }
              else if (currentPosition.xPos == 4) { RotateBackWards(sensors, prevSensors, false); }
              else { RotateBackWards(sensors, prevSensors, true); }
              if (rotationAligned) { currentPosition.direction = DOWN; }
            }
            else if (goalDirection == 'u') { rotationAligned = true; }
            break;
          case LEFT:
            if (goalDirection == 'r') {
              if(currentPosition.yPos == 0) { RotateBackWards(sensors, prevSensors, true); }
              else if(currentPosition.yPos == 4) { RotateBackWards(sensors, prevSensors, false); }
              else { RotateBackWards(sensors, prevSensors, true); }
              if (rotationAligned) { currentPosition.direction = RIGHT; }
            }
            else if (goalDirection == 'u') {
              RotateRight(sensors, prevSensors);
              if(rotationAligned) { currentPosition.direction = UP; }
            }
            else if (goalDirection == 'd') {
              RotateLeft(sensors, prevSensors);
              if (rotationAligned) { currentPosition.direction = DOWN; }
            }
            else if (goalDirection == 'l') { rotationAligned = true; }
            break;
          case RIGHT:
            if (goalDirection == 'l') {
              if (currentPosition.yPos == 0) { RotateBackWards(sensors, prevSensors, false); }
              else if (currentPosition.yPos == 4) { RotateBackWards(sensors, prevSensors, true); }
              else { RotateBackWards(sensors, prevSensors, true); }
              if (rotationAligned) { currentPosition.direction = LEFT; }
            }
            else if (goalDirection == 'u') {
              RotateLeft(sensors, prevSensors);
              if (rotationAligned) { currentPosition.direction = UP; }
            }
            else if (goalDirection == 'd') {
              RotateRight(sensors, prevSensors);
              if (rotationAligned) { currentPosition.direction = DOWN; }
            }
            else if (goalDirection == 'r') {
              rotationAligned = true;
            }
            break;
          case DOWN:
            if (goalDirection == 'r') {
              RotateLeft(sensors, prevSensors);
              if (rotationAligned) { currentPosition.direction = RIGHT; }
            }
            else if (goalDirection == 'l') {
              RotateRight(sensors, prevSensors);
              if (rotationAligned) { currentPosition.direction = LEFT; }
            }
            else if (goalDirection == 'u') {
              if(currentPosition.xPos == 0) { RotateBackWards(sensors, prevSensors, false); }
              else if (currentPosition.xPos == 4) { RotateBackWards(sensors, prevSensors, true); }
              else { RotateBackWards(sensors, prevSensors, true); }
              if (rotationAligned) { currentPosition.direction = UP; }
            }
            else if (goalDirection == 'd') { rotationAligned = true; }
            break;
        }
      }
};

Robot boebot;

String TrimCoordinates(const String & coordinates){
    String trimmedDanceCoordinates = "";
    for(size_t i = 0; i < coordinates.length(); i++){
        if( coordinates[i] != '\n' && coordinates[i] != '\t' &&
            coordinates[i] != ' ' && coordinates[i] != ',' &&
            coordinates[i] != ';' && coordinates[i] != '\\'){
                trimmedDanceCoordinates += (char)coordinates[i];
            }
    }
    return trimmedDanceCoordinates;
}

void UpdateSensors(bool (&sensorArr)[5]) {
  for(size_t i = 0; i < 5; i ++ ) sensorArr[i] = digitalRead(sensors[i]) == 0;
}

bool IsTheEndOfDance() {
  return danceCoordinates.length() == moveIndex;
}

bool GotToTheNextLocation() {
  return boebot.currentPosition.xPos == boebot.nextPosition.xPos && boebot.currentPosition.yPos == boebot.nextPosition.yPos;
}

String GetTheNextDestination() {
  String toReturn = "";
  while(true){
    if(danceCoordinates[moveIndex] == 't') {
      moveIndex++;
      break;
    }
    toReturn += danceCoordinates[moveIndex];
    moveIndex++;
  }
  return toReturn;
}

String GetTheNextDestinationDelay() {
  String toReturn = "";
  while(true) {
    if(isalpha(danceCoordinates[moveIndex])) {
      if(danceCoordinates[moveIndex + 1] == 't') {
        toReturn.remove(toReturn.length()-1);
        moveIndex--;
      }
      break;
    }
    toReturn += danceCoordinates[moveIndex];
    moveIndex++;
    if(IsTheEndOfDance()) { break; }
  }
  return toReturn;
}

void GetInputFromSerialChannel() {
  if (Serial.available()) {
    String input = Serial.readString();
    serialDanceCoordinates += input;
    Serial.println(input);
    serialInputgiven = true;
  }
}

void UpdateTheDanceCoordinate() {
  danceCoordinates = TrimCoordinates(serialDanceCoordinates);
  danceCoordinates.toLowerCase();
  startingPosition = danceCoordinates.substring(0, 3);
}

void InitializeStringHelpers() {
  moveIndex = 3;  // start reading the next move
  positionTimeDelay = 0;
}

void updateval(){
  if (millis() >= ak + 200){
    Serial.println(String(moveIndex) + " " + String(danceCoordinates.length()));
    val = (val + 1) % 2;
    ak = millis();
  }
}

void setup() {
  Serial.begin(115000);
  pinMode(button, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  boebot.AttachWheels();
  danceCoordinates = TrimCoordinates(danceCoordinates);
  danceCoordinates.toLowerCase();
  startingPosition = danceCoordinates.substring(0,3);
  boebot.InitializeTheRobot(startingPosition, positionTimeDelay);  // initializes the robot at the start position
  serialInputInitialized = false;
}

void loop() {
  updateval();
  memcpy(prevSensors, currSensors, sizeof(prevSensors));
  UpdateSensors(currSensors);

  currentTime = (millis() - waitingTime) / 100;

  boebot.CheckIfButtonIsPressed();

  if(boebot.numberOfButtonPresses != 0){
    if(serialInputgiven && !serialInputInitialized) {
      UpdateTheDanceCoordinate();
      serialInputInitialized = true;
    }
    if(!IsTheEndOfDance()){
      if(GotToTheNextLocation() && currentTime >= positionTimeDelay) {
        if (boebot.numberOfButtonPresses == 1) {
          nextPositionFromString = GetTheNextDestination();
          positionTimeDelay = GetTheNextDestinationDelay().toInt();
          boebot.nextPosition.SetPositions(nextPositionFromString[0], nextPositionFromString[1]);
        }else {
            boebot.nextPosition.SetPositions(startingPosition[0], startingPosition[1]);
            boebot.nextPosition.direction = GetTheDirections(startingPosition[2]);
            if (GotToTheNextLocation()) {
              boebot.FinalOrientationAlignment(currSensors, prevSensors);
            }
        }
      }else if(GotToTheNextLocation() && currentTime < positionTimeDelay) {
        boebot.StopWheels();
      }
      else {
        finalRotationStartedFrom = millis();
        boebot.GetToTheNextPosition(currSensors, prevSensors);
      }

    }
    else if(!GotToTheNextLocation()) {
      boebot.GetToTheNextPosition(currSensors, prevSensors);
    }
    else {
      boebot.StopWheels();
    }
  }else if(boebot.numberOfButtonPresses == 0){
    waitingTime = millis();
    boebot.StopWheels();
    GetInputFromSerialChannel();
  }
}
