// M5Stack ESP32
// Duty-cycle 0% 20% 40% 60% 80%
// Range 0-10% 10-30% 30-50% 50-70% 70-90%
// Frequency 0.2 Hz 0.35 Hz 0.5 Hz 0.65 Hz 0.8 Hz
// Amplitude 10% 20% 30% 40% 50% of 3.3V

// PWM signals with frequency of 1000 Hz

#include <M5Stack.h>

float duty_cycle[5] = {0.05, 0.2, 0.4, 0.6, 0.8};
float frequency[5] = {0.2, 0.35, 0.5, 0.65, 0.8};
float amplitude[5] = {0.1, 0.2, 0.3, 0.4, 0.5};

int freq = 1000;
int channel = 0;
int resolution_bits = 8;

// create 2 duty cycle graphs
// for frequency[0] and amplitude[0]

void setup() {
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.printf("Duty-cycle 0%% 20%% 40%% 60%% 80%%");
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("Range 0-10%% 10-30%% 30-50%% 50-70%% 70-90%%");
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.printf("Frequency 0.2 Hz 0.35 Hz 0.5 Hz 0.65 Hz 0.8 Hz");
    M5.Lcd.setCursor(0, 60);
    M5.Lcd.printf("Amplitude 10%% 20%% 30%% 40%% 50%% of 3.3V");
}

void loop() {
  for (int i = 0; i < 5; i++) {
    ledcSetup(channel, freq, resolution_bits);
    ledcAttachPin(25, channel);
    ledcWrite(channel, 255 * duty_cycle[i]);
    delay(1000);
  }
}