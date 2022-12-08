#include <M5Stack.h>
#define LDRpin 5  // pin where we connect LDR and resistor

int BPM = -1; // calculated BPM when heartRateSensor() function is called
int secPerBeat = -1; // integer measuring the number of seconds for one beat (in milliseconds). This equals time2 - time1. 
int beatPerSec = -1; // 1/secPerBeat = beatPerSec --> (check units)
int start = -1; // starting value of LDRpin (either 0 or 1)
// int LDRValue1 = 0; variable most likely not needed
// int LDRValue2 = 0; variable most likely not needed
int time1 = -1; // starting measurement time
int time2 = -1; // ending measurement time. time2 = time1 gives the time for one beat.
int heartRate = -1; // final heart rate value sent to the decision-making module (every few seconds, in real time).

void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(9600); // sets serial port for communication
}

void loop() {

  int heartRateSensor() {
    
    start = digitalRead(LDRpin); // reading starting state from LDRpin

    if (start == 0) { // if start is 0, then wait for LDRpin to become 1, then start the measurement.
      for (int x = 1; x > 0; x++) {
        if (digitalRead(LDRpin) == 1) {
          time1 = millis(); // starting measurement time1
          x = -1; // break out of the for loop after recording the time that LDRpin changed value.
        }
        delay(1);
      }
      for (int x = 1; x > 0; x++) {
        if (digitalRead(LDRpin) == 0) { // when LDRpin changes back to 0, stop the measurement
          time2 = millis(); // ending measurement time2
          x = -1; // break out of the for loop after recording the time that LDRpin changed back.
        }
        delay(1);
      }
    }

    // same code below, but start value is swapped. This is done so that the code will work whether the LDRpin value is initially at 0 or 1.
    // The program ignores the initial beat, because it may be in the middle of a beat. So it waits for a change in the value to ensure that the times measured are the exact start/end of a beat.

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
    BPM = beatPerSec*60;                  // multiplying beats per second by 60 gives the beats per minute --> final return value to decision-maker
    return BPM;
  }

  heartRate = heartRateSensor(); // the loop starts here. It calls the heartRateSensor() function above.

  Serial.print("Current heart rate: ");
  Serial.println(heartRate);              // Here the actual value gets printed
  delay(2000);                      // wait a bit to start the loop again, to account for possible changes in the heartbeat
}
