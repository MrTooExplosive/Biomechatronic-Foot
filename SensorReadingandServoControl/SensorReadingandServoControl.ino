#include <Servo.h>
#include <EEPROM.h>

#define PWM_START 2 // Initial pin number for PWM pins
#define SERVO_COUNT 2 // Currently zero for testing
#define ANALOG_START 0 // Initial pin number for analog pins
#define POT_COUNT 0 // Currently zero for testing
#define PRESSURE_COUNT 0 // Currently zero for testing
#define STATE_NAME_LENGTH 5 // Number of bytes of state names

Servo servos[SERVO_COUNT];
int initalPotValues[POT_COUNT];
int initialPressureVals[PRESSURE_COUNT]; 

//define joystick pins (Analog)
int joyX = 14;   // A14
int joyY = 15;   // A15
int joyVal;     // variable to read the values from the analog pins

class FootPositionState
{
  private: 
    char name[STATE_NAME_LENGTH];
    int relPositions[POT_COUNT];
  public:
    FootPositionState(char stateName[STATE_NAME_LENGTH], int potPos[POT_COUNT]);
    FootPositionState();
    char * getName();
    int write();
    void read(unsigned int byteAddress);
    void goTo();
};
unsigned int address = 0; // Address of byte to write to in EEPROM
const int STATE_SIZE = 1 + POT_COUNT + STATE_NAME_LENGTH; // Number of bytes to store each state
const int MAX_STATES = floor(EEPROM.length() / STATE_SIZE);
FootPositionState * states = new FootPositionState [MAX_STATES];
unsigned int stateCount = 0; // Number of states saved

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

  for (int i = 0; i < EEPROM.length(); i++)
  {
    if (EEPROM.read(i) == (char)30)
    {
      states[stateCount].read(i);
      Serial.print(states[stateCount++].getName());
      Serial.println(" found!");
    }
  }
  Serial.print(stateCount);
  Serial.println(" states found.");
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

FootPositionState::FootPositionState()
{
  for (int i = 0; i < STATE_NAME_LENGTH; i++)
    name[i] = '\0';
  for (int i = 0; i < POT_COUNT; i++)
    relPositions[i] = 0;
}
FootPositionState::FootPositionState(char stateName[STATE_NAME_LENGTH], int potPos[POT_COUNT])
{
  for (int i = 0; i < STATE_NAME_LENGTH; i++)
    name[i] = stateName[i];
  for (int i = 0; i < POT_COUNT; i++)
    relPositions[i] = potPos[i];
}

char * FootPositionState::getName()
{
  return name;
}

int FootPositionState::write()
{
  if (address >= MAX_STATES * STATE_SIZE) // If EEPROM does not have room, do not write
    return -1;
  EEPROM.update(address, (char)30); // Start with a record separator
  address += 1; // Increment address
  for (int i = 0; i < STATE_NAME_LENGTH; i++)
  {
    EEPROM.update(address, name[i]);
    address++;
  }
  for (int i = 0; i < POT_COUNT; i++)
  {
    EEPROM.update(address, (byte)(relPositions[i]/4));
    address++;
  }
  return 0; 
}
    
void FootPositionState::read(unsigned int byteAddress)
{
  for (int i = 0; i < STATE_NAME_LENGTH; i++)
    name[i] = EEPROM.read(byteAddress + 1 + i);
  for (int i = 0; i < POT_COUNT; i++)
    relPositions[i] = 4 * EEPROM.read(byteAddress + 1 + STATE_NAME_LENGTH + i);
}

// SERVOS SHOULD BE CALIBRATED AT ZERO POSITION BEFORE ASSEMBLY
void FootPositionState::goTo()
{
  for (int i = 0; i < POT_COUNT; i++)
    servos[i].write(relPositions[i] * 180 / 1023);
}
