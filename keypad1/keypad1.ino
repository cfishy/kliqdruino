/* Copyright Hsiao-yu Chen  2012
   Arduino based keyboard controller.
   
   Arduino one key keyboard.
*/
#include <Usb.h>
#include <Keypad.h>
const bool PROGRAMMING = 1;  /* Programming mode */
const bool DEBUGGING = 1;    /* Debug via Serial */
const int MODE_TOGGLE = 2;   /* pin for mode toggle switch, short to ground */

const int KEY1 = 9;   /* pin for the single key */

/* USB HID scan code to send when the key is pressed. */
const int scancode = 4; /* scan code to send */

/* Mode toggle switch tester to ensure mode toggle is working. */
  /* when toggle switch is closed, LED should be off, indicating programming mode. */
  /* when toggle switch is open, LED should be on, indicating running mode. */

int statusLED = 13;  /* built in LED */
uint8_t KeyBuffer[8] = {0,0,0,0,0,0,0,0};

/* Setup a toggle to switch between 
   programming and run mode. Avoids interference. */
void setupModeToggle() {
  /* Assign toggle pin and pull up */
  pinMode(MODE_TOGGLE, INPUT);
  digitalWrite(MODE_TOGGLE, HIGH);
  digitalWrite(KEY1, HIGH);
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
  return digitalRead(MODE_TOGGLE);
}

int scan() {
  return !digitalRead(KEY1);
}


void setup() {
  setupModeToggle();
}

void clearKeyBuffer() {
  for (int i = 0; i < 8; i++){
    if (KeyBuffer[i] != 0) {
      KeyBuffer[i] = 0;
    }
  }
}

void loop() {
  /* LED to indicate whether programming mode is on. */
  digitalWrite(statusLED, isRunState());
  /* TODO: optimize to use isRunState once */
  if (DEBUGGING) {
    debugWriteState();
  }
  
  /* scan */
  if (isRunState() != PROGRAMMING) {
    if (scan()) {
      if (DEBUGGING) {
        Serial.write(" keypress ");
      }
      //testing HID_SendReport
      KeyBuffer[3] = scancode;
      HID_SendReport(2, KeyBuffer, 8);
      //end testing
    } else {
      clearKeyBuffer();
      HID_SendReport(2, KeyBuffer, 8);
    }
  }
  delay(50);
}
