// Provide interface for speed of sound sensor on Mega
// All variables for Speed of Sound will have the SOS_ prefix
/*Concept of operations:
Include sos.h for definitions
Call SOS_setup(); in setup()
Call SOS_main(); in loop()
*/

#include <Arduino.h>

// Variables: use #define?
#define SOS_buzzerPin A15
#define SOS_selectPin 6
#define SOS_toneFreq 500;

//#define SOS_ledPin 13;

// Keep track of time (millis);
int SOS_currentTime = 0;
int SOS_prevTime = 0;
int SOS_count = 0; // Preserve count

void SOS_setup(void);
int SOS_main(void);