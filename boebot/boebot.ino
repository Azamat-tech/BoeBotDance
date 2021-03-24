#include "constants.hpp"

#include <Servo.h>

using namespace std;

String dance_coordinates = "a1n c2 t120 d4 t0 b5 t0 a2 t368 e2 t452 \
                            1c t0 e1 t600";

String trimmed_dance_coordinates = "";

class Robot {
    private:
        Servo left;
        Servo right;

    public:

    void attach_wheels();

    void stopWheels();
    void foward();
    void backward();
    void turnLeft();
    void turnRight();
    
};

void trim_coordinates();

void trim_coordinates(){
    for(size_t i = 0; i < dance_coordinates.length(); i++){
        if( dance_coordinates[i] != '\n' && dance_coordinates[i] != '\t' &&
            dance_coordinates[i] != ' ' && dance_coordinates[i] != ',' &&
            dance_coordinates[i] != ';'){
                trimmed_dance_coordinates += dance_coordinates[i];
            }
    }
}

void setup() {
  trim_coordinates();

}

void loop() {
  Robot boebot;
  

}
