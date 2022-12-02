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
  BPM = Beats/(myTime*1000)*60;
  
  Serial.println(LDRValue);      // print the value to the serial port
  Serial.print("BPM: ");
  Serial.println(BPM);
 
  previous_value =LDRValue;
  delay(100);                    // wait a little
  
}
