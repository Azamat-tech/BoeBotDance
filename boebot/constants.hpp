#ifndef ROBOT_CONSTANTS
#define ROBOT_CONSTANTS

#include "Arduino.h"
#include <Servo.h>

// Pins of the Wheel
constexpr int left_wheel = 12;
constexpr int right_wheel = 13;

// Pin of the led
constexpr int led = 11;

// Pin of the button
constexpr int button = 2;

// Pins of the sensors
constexpr int left_most_sensor = 3;
constexpr int left_sensor = 4;
constexpr int middle_sensor = 5;
constexpr int right_sensor = 6;
constexpr int right_most_sensor = 7;
constexpr int sensors[5] = { right_most_sensor, right_sensor,middle_sensor, left_sensor, left_most_sensor };


#endif