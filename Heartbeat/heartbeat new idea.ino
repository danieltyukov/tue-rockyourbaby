#include <M5Stack.h>
#define LDRpin 5  // pin where we connect LDR and resistor

int BPM = -1;
int secPerBeat = -1; // integer measuring the number of seconds for one beat (in milliseconds). This equals time2 - time1. 
int beatPerSec = -1; // 1/secPerBeat = beatPerSec --> (check units)
int start = -1; // starting value of LDRpin (either 0 or 1)
// int LDRValue1 = 0;
// int LDRValue2 = 0;
int time1 = -1; // starting measurement time
int time2 = -1; // ending measurement time. time2 = time1 gives the time for one beat.

void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(9600); // sets serial port for communication
}

void loop() {
  start = digitalRead(LDRpin); // reading starting state from LDRpin

  if (start == 0) { // if start is 0, then wait for LDRpin to become 1, then start the measurement.
    for (int x = 1; x > 0; x++) {
      if (digitalRead(LDRpin) == 1) {
        time1 = millis(); // starting measurement time1
        x = -1; // break out of the for loop
      }
      delay(1);
    }
    for (int x = 1; x > 0; x++) {
      if (digitalRead(LDRpin) == 0) { // when LDRpin changes back to 0, stop the measurement
        time2 = millis(); // ending measurement time2
        x = -1; // break out of the for loop
      }
      delay(1);
    }
  }

  // same code below, but start value is swapped

  else if (start == 1) {
    for (int x = 1; x > 0; x++) {
      if (digitalRead(LDRpin) == 0) {
        time1 = millis();
        x = -1;
      }
      delay(1);
    }
    for (int x = 1; x > 0; x++) {
      if (digitalRead(LDRpin) == 1) {
        time2 = millis();
        x = -1;
      }
      delay(1);
    }
  }

  secPerBeat = (time2-time1)/1000;            // time2-time1 gives the time taken for one heart beat, in milliseconds. Divide by 1000 for seconds.
  beatPerSec = 1/secPerBeat;              // 1/secPerBeat gives the beats per second
  BPM = beatPerSec*60;                  // multiplying beats per second by 60 gives the beats per minute --> final value.

  Serial.print("BPM: ");
  Serial.println(BPM);              // Here the actual value gets printed
  delay(2000);                      // wait a bit to start the loop again, to react to a change in heartbeat
}
