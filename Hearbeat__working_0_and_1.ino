#define LDRpin 5  // pin where we connect LDR and resistor

int LDRValue = 0; // result of reading the analog pin

void setup() {
  Serial.begin(9600); // sets serial port for communication
}

void loop() {
  LDRValue = digitalRead(LDRpin);// read the value from the LDR
  Serial.println(LDRValue);      // print the value to the serial port
  delay(100);                    // wait a little
}
