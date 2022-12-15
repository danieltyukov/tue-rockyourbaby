#define LDRpin 5  // pin where we connect LDR and resistor

int LDRValue = 0; // result of reading the analog pin
int BPM = 0;
int Beats = 0;
int myTime = 0;
int previous_value = 0;

void setup() {
  Serial.begin(9600); // sets serial port for communication
  previous_value = digitalRead(LDRpin);
}

void loop() {
  LDRValue = digitalRead(LDRpin);// read the value from the LDR
  if(LDRValue != previous_value){
    Beats = Beats+1;
  }
  myTime = millis();
  BPM = Beats/(myTime*0.001)*60;

  Serial.print("Beats: ");
  Serial.println(Beats);
  Serial.print("Time: ");
  Serial.println(myTime);
  Serial.print("Previous: ");
  Serial.println(previous_value);
  Serial.println(LDRValue);      // print the value to the serial port
  Serial.print("BPM: ");
  Serial.println(BPM);

  
 
  previous_value =LDRValue;
  delay(3000);                    // wait a little
  
}
