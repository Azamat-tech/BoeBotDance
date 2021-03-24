#ifndef ROBOT
#define ROBOT

#include "constants.hpp"

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


#endif