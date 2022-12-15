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

// Motor Control
void motor(int freq1, int amp1) {
    ledcWrite(AMPchannel, settingsAMP[amp1]*250);
    ledcWrite(FREQchannel, settingsFREQ[freq1]*250);
}

// Heartbeat Measuring Decision
bool heartbeat () {
    // measure the heartbeat
    // BPM improved = BPM + tresholdBPM < lastBPM
    // if BPM improved, return true, otherwise return false
    // also if BPM reaches 60, change "restingBPMreached" to true
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

