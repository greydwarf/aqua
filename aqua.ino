#include <Metro.h>

#define VALVE_RELAY_PIN1 13
#define VALVE_RELAY_PIN2 14
#define VALVE_RELAY_PIN3 15
#define VALVE_RELAY_PIN4 16

// Change these times to reflect reality.  All times are in seconds.
int GROWBED_DURATION[] = {
  10,         // Time that the growbed 1 is on
  0,             // Drain time for growbed 1 (nothing is on for this time)
  10,         // Time that the growbed 2 is on
  0,             // Drain time for growbed 2 (nothing is on for this time)
  10,         // Time that the growbed 3 is on
  20          // Drain time for growbed 3 (nothing is on for this time)
};

typedef enum 
{
  VALVE_RELAY1,
  VALVE_RELAY2,
  VALVE_RELAY3,
  VALVE_RELAY4,
} VALVE_RELAYS;

typedef enum
{
  GROWBED_UNINITIALIZED = -1,  // This must always be the first thing in the list
  GROWBED1_ON,
  GROWBED1_DRAIN,
  GROWBED2_ON,
  GROWBED2_DRAIN,
  GROWBED3_ON,
  GROWBED3_DRAIN,
  GROWBED_NUM_STATES           // This must always be the last thing in the list
} GROWBED_STATE;

GROWBED_STATE growbedState = GROWBED_UNINITIALIZED;
Metro growbedMetro(0);

void activateValves()
{
  int newState = (growbedState == GROWBED1_ON) ? HIGH : LOW;
  digitalWrite(VALVE_RELAY_PIN1, newState);
  
  newState = (growbedState == GROWBED2_ON) ? HIGH : LOW;
  digitalWrite(VALVE_RELAY_PIN2, newState);
  
  newState = (growbedState == GROWBED3_ON) ? HIGH : LOW;
  digitalWrite(VALVE_RELAY_PIN3, newState);
  
  newState = (growbedState == GROWBED1_DRAIN || growbedState == GROWBED2_DRAIN || growbedState == GROWBED3_DRAIN) 
    ? HIGH : LOW;
  digitalWrite(VALVE_RELAY_PIN4, newState);
}

void transitionGrowbedState()
{
  growbedState = static_cast<GROWBED_STATE>((growbedState + 1) % GROWBED_NUM_STATES);
  growbedMetro.interval(GROWBED_DURATION[growbedState]*1000);
}
  
void setup()
{
  pinMode(VALVE_RELAY_PIN1, OUTPUT);
  pinMode(VALVE_RELAY_PIN2, OUTPUT);
  pinMode(VALVE_RELAY_PIN3, OUTPUT);
  pinMode(VALVE_RELAY_PIN4, OUTPUT);
}

void loop()
{
  if (growbedMetro.check() == 1) 
  {
    transitionGrowbedState();
    activateValves();
  }
}
