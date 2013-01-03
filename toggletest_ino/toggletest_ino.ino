#include <Keypad.h>
#define PROGRAMMING 1 /* Programming mode */
#define DEBUGGING 1       /* Debug via Serial */
/* Copyright Hsiao-yu Chen  2012
   Arduino based keyboard controller.
*/

/* Mode toggle switch tester to ensure mode toggle is working. */
  /* when toggle switch is closed, LED should be off, indicating programming mode. */
  /* when toggle switch is open, LED should be on, indicating running mode. */

int modeToggle = 2;  /* pin for mode toggle switch, short to ground */
int statusLED = 13;  /* built in LED */

/* Setup a toggle to switch between 
   programming and run mode. Avoids interference. */
void setup_mode_toggle() {
  /* Assign toggle pin and pull up */
  pinMode(modeToggle, INPUT);
  digitalWrite(modeToggle, HIGH);
  /* LED to indicate mode */
  pinMode(statusLED, OUTPUT);
  
  Serial.begin(9600);
}

void debugWriteState() {
  if (isRunState() == PROGRAMMING) {
    Serial.write(" programming ");
  } else {
    Serial.write(" running ");
    
  }
}

/* Check to see whether the toggle is on run state. */
bool isRunState() {
  return digitalRead(modeToggle);
}

void setup() {
  setup_mode_toggle();
}

void loop() {
  /* LED to indicate whether programming mode is on. */
  digitalWrite(statusLED, isRunState());
  /* TODO: optimize to use isRunState once */
  if (DEBUGGING) {
    debugWriteState();
  }
  delay(1000);
}
