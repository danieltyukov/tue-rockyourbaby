#include <M5Stack.h>

//GPIO input pin for DIGITAL signal
const int digitalPin = 2;

void setup() {
    M5.begin();
    pinMode(digitalPin, INPUT);
    Serial.printf("LIGHT SENSOR TEST");
}

void loop() {
    int sensorValue = digitalRead(digitalPin);
    Serial.printf("Value Output: %0d\n", sensorValue);
    delay(1000);
    // delay(200);
}
