#include <M5Stack.h>

// Delays
int generalDelay = 2000;
int stressDelay = 3000;
int heartbeatDelay = 12000;

// Motor Vals
int freq1 = 5;  
int amp1 = 5;
int freq = 1000;
int resolutionBits = 8;
float settingsFREQ[5] = {0.05, 0.2, 0.4, 0.6, 0.8};
float settingsAMP[5] = {0.05, 0.2, 0.4, 0.6, 0.8};
int AMPchannel = 0;
int FREQchannel = 1;
int pinAmp = 1;
int pinFreq = 3;

// Heartbeat Vals
#define pinLDR 5
int lastBPM = 220;
int tresholdBPM = 10;
bool restingBPMreached = false;
int LDRValue = 0;   // result of reading the analog pin
int BPM = 0;        //This variable will keep track of the current BPM of the baby, which must be between 60 and 240
int previous_value = 0; //This we keep track of the value from the previous loop, to see the comparison

// Motor Control
void motor(int freq1, int amp1) {
    ledcWrite(AMPchannel, settingsAMP[amp1]*250);
    ledcWrite(FREQchannel, settingsFREQ[freq1]*250);
}

// Heartbeat Measuring Decision
    // measure the heartbeat
    // BPM improved = BPM + tresholdBPM < lastBPM
    // if BPM improved, return true, otherwise return false
    // also if BPM reaches 60, change "restingBPMreached" to true
bool heartbeat(){
  delay(heartbeatDelay);
    
  int first_change = 0;   //Boolean integers to stop loops
  int second_change = 0;
  int bpm_value = 0;
  bool better;

  int timestart = 0; //Variable to see start time
  int timestop = 0; //Variable to see stopping time
  int timepassed = 0; //To store the time the has passed between the two variable above
  int beats = 1; //Beats, this is automatically 0, because 
  int valid_value = 0;

  int measuring_value = 0; //Variable that isn't used anymore, it think.
  previous_value = digitalRead(LDRpin);     // read the value from the LDR, 

//  Serial.print("Initialized");
  while(valid_value){
    while(first_change == 0){    //While loop which stop if there is a bpm
      LDRValue = digitalRead(LDRpin);     // read the value from the LDR
      Serial.println(LDRValue);
  //    Serial.print("first time looping");
      if(LDRValue != previous_value){  //Checks if there has been a change from last loop
        timestart = millis();  //Takes the time the first change is noticed
        Serial.print("First change!");
        Serial.print("Previous: ");
        Serial.println(previous_value);
        first_change = 1; //Boolean variable, not necessarry, when coding smart 
      }
      previous_value = LDRValue; // Set the previous vale to the current loop for next loop
    }
    
    while(second_change == 0){
      LDRValue = digitalRead(LDRpin); // read the value from the LDR
      Serial.println(LDRValue);
      if(LDRValue != previous_value){     // Checks of the hearbeat has changed from last time
        timestop = millis();   //Takes the time the second cange is noticed
        Serial.print("Second change");
        timepassed = timestop - timestart; //Calculate the time for one up or down time. 
  
        Serial.print("Timepassed: ");
        Serial.println(timepassed);
  
        if(timepassed > 500){ //If the time passed is bigger than half a second it will calculate the bpm and stop the programm, maybe this value can be a bit lower to make it faster.
          Serial.print("beats");
          Serial.println(beats);
          BPM = ((0.5 *60*beats)/(0.001*timepassed)); //Calculate the current bpm which is time two, because you got half a cycle, times 0.001 because the time is in millis and times 60 because it is beats per MINUTE
    //      Serial.print("BPM is: ");
    //      Serial.println(BPM);
          second_change = 1;  //Boolean variable, not necessarry, when coding smart
        }
        else{
          beats = beats+1;
          previous_value = LDRValue; // Set the previous vale to the current loop for next loop
          Serial.print("adding a beat");
        }
      }
      previous_value = LDRValue; // Set the previous vale to the current loop for next loop
    }
    if(lastBPM == 80 && BPM > 50 && BPM < 70){ //Rest mode
      better = true;
      valid_value = 1;
      restingBPMreached = true;
    }
    else if(BPM > (lastBPM-30) && BPM < (lastBPM-10)){  //See if it has gone one stress level lower
      better = true;
      valid_value = 1;
    }
    else if(BPM > (lastBPM-10) && BPM < (lastBPM+10)){ //Kept the same
      better = false;
      valid_value = 1;
    }
    else if(BPM > 230 && BPM < 250){ //Panic mode
      better = false;
      valid_value = 1;
    }
  }
  return better; //return the current bpm to the function, currently not really necessary because it is a global variable
}

void setup() {
    // Init M5Core.  
    M5.begin();
    // Init Power Module.
    M5.Power.begin();
    // Serial Communication Begin
    Serial.begin(9600);

    // Motor
    ledcAttachPin(pinAmp, AMPchannel);
    ledcAttachPin(pinFreq, FREQchannel);
    ledcSetup(AMPchannel, freq, resolutionBits);
    ledcSetup(FREQchannel, freq, resolutionBits);

    delay(generalDelay);

    // Start the Motor at 80% Duty Cycle
    motor(freq1, amp1);
    delay(stressDelay);
}

void loop() {

    freq1 -= 1;
    motor(freq1, amp1);
    delay(stressDelay);

    // Measure the Heartbeat
    bool heartbeatResponse = heartbeat();

    // if restingBPMreached == true, turn off the M5Stack
    if (restingBPMreached == true) {
        M5.Power.powerOFF();
    }

    // If Heartbeat is higher or the same: FALSE
    if (heartbeatResponse == false) {

        // Move Back
        freq1 += 1;
        motor(freq1, amp1);
        delay(1);

        // Go Other Direction
        amp1 -= 1;
        motor(freq1, amp1);
        delay(stressDelay);

        // Update lastBPM
        lastBPM -= 20;
    }
    else {
        // Correct Direction
        // Update lastBPM
        lastBPM -= 20;
    }

    // Hitting Corner Solution
    if (amp1 == 1) {
        while (freq1 > 1) {
            freq1 -= 1;
            motor(freq1, amp1);
            delay(stressDelay);
        }
    }
    if (freq1 == 1) {
        while (amp1 > 1) {
            amp1 -= 1;
            motor(freq1, amp1);
            delay(stressDelay);
        }
    }
}

