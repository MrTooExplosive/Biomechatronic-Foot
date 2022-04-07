#include <Servo.h>

#define PWM_START 2 // Initial pin number for PWM pins
#define SERVO_COUNT 2 // Currently zero for testing
#define ANALOG_START 0 // Initial pin number for analog pins
#define POT_COUNT 0 // Currently zero for testing
#define PRESSURE_COUNT 0 // Currently zero for testing

Servo servos[SERVO_COUNT];
int initalPotValues[POT_COUNT];
int initialPressureVals[PRESSURE_COUNT];

//define joystick pins (Analog)
int joyX = 14;   // A14
int joyY = 15;   // A15
int joyVal;     // variable to read the values from the analog pins

void setup()
{
  Serial.begin(9600); // Set BAUD rate

  // Get the initial pressures of each pressure transducer
  // Will be useful later
  for (int i = 0; i < PRESSURE_COUNT; i++)
    initialPressureVals[i] = analogRead(i + ANALOG_START + POT_COUNT); // Pressure tranducers will be attached after potentiometers
    
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
  joyVal = analogRead(joyX);
  joyVal = map(joyVal, 0, 1023, 0, 180); //servo value between 0-180
  servos[1].write(joyVal); //set the servo position according to the joystick value

  joyVal = analogRead(joyY);
  joyVal = map(joyVal, 0, 1023, 0, 180);
  servos[0].write(joyVal);

  delay(15);
}
