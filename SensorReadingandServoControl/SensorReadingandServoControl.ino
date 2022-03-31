#include <Servo.h>

#define PWM_START 2 // Initial pin number for PWM pins
#define SERVO_COUNT 0 // Currently zero for testing
#define ANALOG_START 0 // Initial pin number for analog pins
#define POT_COUNT 1

Servo servos[SERVO_COUNT];
int initalPotValues[POT_COUNT];


void setup()
{
  Serial.begin(9600); // Set BAUD rate

  // Get the initial position of each potentiometer
  // Used for the finding the relative positions
  for (int i = 0; i < POT_COUNT; i++)
    initalPotValues[i] = analogRead(i + ANALOG_START);
    
  // Attach the signal pin for each servo
  for (int i = 0; i < SERVO_COUNT; i++)
    servos[i].attach(i + PWM_START);
}

void loop()
{
}
