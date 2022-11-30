#include <M5Stack.h>

// ADC input pins for ANALOG (34/35/36)
const int analogPin = 36;

void setup() {
    M5.begin();
    pinMode(analogPin, INPUT);
    Serial.printf("MIC SENSOR TEST");
}

void loop() {
    int sensorValue = analogRead(analogPin);
    Serial.println("Value Output: %0d\n", sensorValue);
    delay(1000);
    // delay(200);
}