#define LDRpin 5    // pin where we connect LDR and resistor, this is a digital pin

int LDRValue = 0;   // result of reading the analog pin
int BPM = 0;        //This variable will keep track of the current BPM of the baby, which must be between 60 and 240
int Beats = 0;      //This is not actually the beats, but the times that the light switches to another states. You could argue this is a bug, but we will correct this in the calculation
int myTime = 0;     //THis variable will be set for each calculation as the time passed since the program started
int previous_value = 0; //This we keep track of the value from the previous loop, to see the comparison

void setup() {
  Serial.begin(9600); // sets serial port for communication, to read on your pc
  previous_value = digitalRead(LDRpin); //For the first time we run the loop we don't have a accurate value, so we need to set the previous value one time.
}

void loop() {
  LDRValue = digitalRead(LDRpin);// read the value from the LDR
  if(LDRValue != previous_value){
    Beats = Beats+1;
  }
  myTime = millis();
  BPM = (Beats/(myTime*0.001)*60)/2;

  // Serial.print("Beats: ");
  // Serial.println(Beats);
  // Serial.print("Time: ");
  // Serial.println(myTime);
  // Serial.print("Previous: ");
  // Serial.println(previous_value);
  // Serial.println(LDRValue);      // print the value to the serial portx
  
  Serial.print("BPM: ");
  Serial.println(BPM);

  
 
  previous_value =LDRValue;
  delay(100);                    // wait a little
  
}
