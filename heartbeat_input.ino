
// 3.3V
// GND
// GIPIO 36
#define LIGHT_SENSOR_PIN 36

void setup() {
  // serial communication at 9600 bits/s
  Serial.begin(9600);
}

void loop() {
  int analogValue = analogRead(LIGHT_SENSOR_PIN);
  Serial.print("Analog Value = ");
  Serial.print(analogValue);
  delay(500);
}
