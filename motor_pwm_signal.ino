#include <M5Stack.h>  
int freq1 = 0;  
int ampl = 0; 
int freq = 1000;
int channel = 0;
int resolution_bits = 8;
int buzzer = 0;

void setup() {
  M5.begin();
  ledcSetup(channel, freq, resolution_bits);
  ledcAttachPin(buzzer, channel);
}
void loop() {
    ledcWrite(channel, 128);
    delay(1000);
    ledcWrite(channel, 250);
    delay(1000);
    ledcWrite(channel, 0);
    delay(1000);
}