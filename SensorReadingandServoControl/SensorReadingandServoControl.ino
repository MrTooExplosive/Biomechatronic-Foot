#include <Servo.h>

#define PWM_START 2 // Initial pin number for PWM pins
#define SERVO_COUNT 1

Servo servos[SERVO_COUNT];

void setup()
{
  // Attach the signal pin for each servo
  // Servo pins start at pin2
  for (int i = 0; i < SERVO_COUNT; i++)
    servos[i].attach(i + PWM_START);
}

void loop()
{
}
