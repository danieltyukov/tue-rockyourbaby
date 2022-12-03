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
  LDRValue = digitalRead(LDRpin);     // read the value from the LDR
  if(LDRValue != previous_value){     // Checks of the hearbeat has changed from last time
    Beats = Beats+1;                  // If it has it adds a beat, there is a bug in this because we add twice as much beats as actually happened. This makes it more accurate. But we need to compensate for this in the actual calculation. 
  }
  myTime = millis();                  // Asks the time that the program is running and saves it in the variable mytime. It is called mytime, because if you just use time you call a build in function.
  BPM = (Beats/(myTime*0.001)*60)/2;  // The actual calculation, which should be beats/time. However the time is in miliseconds so you need to multiply by a thousand and by 60 to make it a value in minutes. The reason for the times 2 afterwards is explained in the beats++ line>

  // Serial.print("Beats: ");         // These statements are used to debug if neccesarry the output will be on the serial monitor.
  // Serial.println(Beats);
  // Serial.print("Time: ");
  // Serial.println(myTime);
  // Serial.print("Previous: ");
  // Serial.println(previous_value);
  // Serial.println(LDRValue);      // print the value to the serial portx
  
  Serial.print("BPM: ");
  Serial.println(BPM);              // Here the actual value gets printed

  
 
  previous_value =LDRValue;      // Set the previous vale to the current loop for next loop
  delay(100);                    // wait a little
  
}
