#include <M5Stack.h>

// Delays
int generalDelay = 2000;
int stressDelay = 2000;
int heartbeatDelay = 10000;

// Motor Vals
int freq1 = 4;
int amp1 = 4;
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
bool heartbeatResponse = false;
int lastBPM = 220;
int tresholdBPM = 10;

// Crying Vals
#define pinMIC 35
bool crying_started = false;
volatile int amount_of_peaks = 0;

int previous_amount_of_peaks_1 = 0;
int previous_amount_of_peaks_2 = 0;

int current_amount_of_peaks_1 = 0;
int current_amount_of_peaks_2 = 0;

bool previous_decision = false;
bool current_decision = false;

int final_previous_amount_of_peaks = 0;
int final_current_amount_of_peaks = 0;

// ########## CONTROLS ##########

// Motor Control
void motor(int freq1, int amp1)
{
  ledcWrite(AMPchannel, settingsAMP[amp1] * 250);
  ledcWrite(FREQchannel, settingsFREQ[freq1] * 250);
}

// Crying Measuing Decision
bool crying()
{
  if ((final_current_amount_of_peaks - final_previous_amount_of_peaks) <= -150)
  {
    return true;
  }
  return false;
}

// ISR to count the amount of peaks
void count_peaks()
{
  amount_of_peaks++;
}

// Heartbeat Measuring Decision
bool heartbeat()
{
  // measure the heartbeat
  // BPM improved = BPM + tresholdBPM < lastBPM
  // if BPM improved, return true, otherwise return false

  bool decision;

  bool valid_value = false;
  bool first_change = false;
  bool second_change = false;

  // time tracking
  int timestart = 0;
  int timestop = 0;
  int timepassed = 0;

  int beats = 1;

  // Current read value from the LDR
  int LDRValue = 0;
  // New BPM value
  int BPM = 0;

  // ########## BPM Decision ##########

  // Initial read value from the LDR
  delay(heartbeatDelay);
  int previous_value = digitalRead(pinLDR);

  // Validate the value loop
  while (valid_value == false)
  {

    // Loop until bpm is detected
    while (first_change == false)
    {

      // Read the value from the LDR
      LDRValue = digitalRead(pinLDR);
      Serial.println(LDRValue);

      // Checks if there has been a change from last loop
      if (LDRValue != previous_value)
      {
        // Takes the time the first change is noticed
        timestart = millis();

        Serial.print("First change!");
        Serial.print("Previous: ");
        Serial.println(previous_value);

        first_change = true;
      }

      previous_value = LDRValue;
    }

    // Loop until bpm is detected 2nd
    while (second_change == false)
    {
      LDRValue = digitalRead(pinLDR);
      Serial.println(LDRValue);

      // Checks of the hearbeat has changed from last time
      if (LDRValue != previous_value)
      {
        // Takes the time the second change is noticed
        timestop = millis();
        Serial.print("Second change");
        timepassed = timestop - timestart;

        Serial.print("Timepassed: ");
        Serial.println(timepassed);

        // Checks if the time passed is more than 500ms, if so, it is a valid bpm
        if (timepassed > 500)
        {
          Serial.print("beats: ");
          Serial.println(beats);

          // Calculate the current bpm which is time two, because you got half a cycle, times 0.001 because the time is in millis and times 60 because it is beats per MINUTE
          BPM = ((0.5 * 60 * beats) / (0.001 * timepassed));

          second_change = true;
        }
        else
        {
          // Set the previous value to the current loop for next loop
          // and add a beat
          beats = beats + 1;
          previous_value = LDRValue;
          Serial.print("adding a beat");
        }
      }

      previous_value = LDRValue;
    }

    // BPM improved = BPM + tresholdBPM < lastBPM
    if (BPM > (60 - tresholdBPM) && BPM < (60 + tresholdBPM) && lastBPM == 80)
    {
      // Rest mode
      decision = true;
      valid_value = true;
    }
    // make a check that new bpm is within the treshold of the last bpm
    // example: lastBPM = 200, tresholdBPM = 10, BPM = 190, 200, 210 -> meaning changed and is within the treshold
    // Nothing has changed
    else if (BPM > (lastBPM - tresholdBPM) && BPM < (lastBPM + tresholdBPM))
    {
      decision = false;
      valid_value = true;
    }
    // example: lastBPM = 200, tresholdBPM = 10, BPM = 218 -> meaning didnt change or went higher
    // The stress level, went one lower
    else if (BPM > (lastBPM - tresholdBPM - 20) && BPM < (lastBPM - tresholdBPM))
    {
      decision = true;
      valid_value = true;
      if (BPM > 130 && BPM < 150)
      {
        crying_started = true;
      }
    }
    // Panic code
    else if (BPM > 230 && BPM < 250)
    {
      decision = false;
      valid_value = true;
    }
  }

  return decision;
}

void setup()
{
  // Initialize Coomunication with M5Stack
  M5.begin(true, true, false, true);
  Serial.begin(9600);
  M5.Power.begin();

  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 20);
  M5.Lcd.println("BABY ROCKING - V2.0");

  // Motor
  ledcAttachPin(pinAmp, AMPchannel);
  ledcAttachPin(pinFreq, FREQchannel);
  ledcSetup(AMPchannel, freq, resolutionBits);
  ledcSetup(FREQchannel, freq, resolutionBits);

  // Start the Motor at 80% Duty Cycle
  motor(freq1, amp1);

  delay(5000);
  // Microphone
  attachInterrupt(pinMIC, count_peaks, RISING);
}

void loop()
{
  delay(generalDelay);

  // rest mode reached
  while ((freq1 == 0) && (amp1 == 0))
  {
    delay(5000);
  }

  freq1 -= 1;
  motor(freq1, amp1);
  delay(stressDelay);

  // Measure the Heartbeat
  if (crying_started == false)
  {
    bool heartbeatResponse = heartbeat();
  }

  // Measure Microphone - Previous
  if (crying_started == true)
  {
    while (previous_decision == false)
    {
      amount_of_peaks = 0;
      delay(1000);
      previous_amount_of_peaks_1 = amount_of_peaks;

      amount_of_peaks = 0;
      delay(1000);
      previous_amount_of_peaks_2 = amount_of_peaks;

      if ((previous_amount_of_peaks_1 - previous_amount_of_peaks_2) <= 150 && (previous_amount_of_peaks_1 - previous_amount_of_peaks_2) >= -150)
      {
        previous_decision = true;
      }
    }

    final_previous_amount_of_peaks = previous_amount_of_peaks_1;
  }

  if (crying_started == false)
  {
    bool heartbeatResponse = heartbeat();
    delay(generalDelay);
    // If Heartbeat is higher or the same: FALSE
    if (heartbeatResponse == false)
    {
      // Move Back
      freq1 += 1;
      motor(freq1, amp1);
      delay(stressDelay);

      // Go Other Direction
      amp1 -= 1;
      motor(freq1, amp1);
      delay(stressDelay);

      // Update lastBPM
      lastBPM -= 20;
    }
    else
    {
      // Correct Direction
      // Update lastBPM
      lastBPM -= 20;
    }

    // Hitting Corner Solution
    if (amp1 == 0)
    {
      while (freq1 > 0)
      {
        freq1 -= 1;
        motor(freq1, amp1);
        delay(stressDelay);
      }
    }
    if (freq1 == 0)
    {
      while (amp1 > 0)
      {
        amp1 -= 1;
        motor(freq1, amp1);
        delay(stressDelay);
      }
    }
  }
  else
  {
    // Measure Microphone - Current
    while (current_decision == false)
    {
      amount_of_peaks = 0;
      delay(1000);
      current_amount_of_peaks_1 = amount_of_peaks;

      amount_of_peaks = 0;
      delay(1000);
      current_amount_of_peaks_2 = amount_of_peaks;

      if ((current_amount_of_peaks_1 - current_amount_of_peaks_2) <= 150 && (current_amount_of_peaks_1 - current_amount_of_peaks_2) >= -150)
      {
        current_decision = true;
      }
    }

    final_current_amount_of_peaks = current_amount_of_peaks_1;

    bool microphoneResponse = crying();

    // If Crying is higher or the same: FALSE
    if (microphoneResponse == false)
    {
      // Move Back
      freq1 += 1;
      motor(freq1, amp1);
      delay(stressDelay);

      // Go Other Direction
      amp1 -= 1;
      motor(freq1, amp1);
      delay(stressDelay);
    }

    // Hitting Corner Solution
    if (amp1 == 0)
    {
      while (freq1 > 0)
      {
        freq1 -= 1;
        motor(freq1, amp1);
        delay(stressDelay);
      }
    }
    if (freq1 == 0)
    {
      while (amp1 > 0)
      {
        amp1 -= 1;
        motor(freq1, amp1);
        delay(stressDelay);
      }
    }
  }
}