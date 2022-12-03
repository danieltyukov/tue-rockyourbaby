
//#include <M5Stack.h>

// 3.3V
// GND
// GPIO 36 - FOR ANALOG
// GPIO 5 - FOR DIGITAL
#define ANALOG_SENSOR_PIN 36
#define DIGITAL_SENSOR_PIN  5

void setup() {
  // M5.begin();
  // serial communication at 9600 bits/s
  Serial.begin(9600);
  pinMode(36, INPUT);
  pinMode(5, INPUT);
}

void loop() {
  int analogValue = analogRead(ANALOG_SENSOR_PIN);
  int digitalValue = digitalRead(DIGITAL_SENSOR_PIN);
  Serial.println("Analog Value = ");
  Serial.println(analogValue);

  delay(500);

  Serial.println("Digital Value = ");
  Serial.println(digitalValue);

  delay(900);
}
