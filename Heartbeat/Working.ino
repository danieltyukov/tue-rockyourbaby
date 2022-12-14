#define LDRpin 5    // pin where we connect LDR and resistor, this is a digital pin
#define ButtonPin 35 // Pin to which the button will be connected

int LDRValue = 0;   // result of reading the analog pin
int BPM = 0;        //This variable will keep track of the current BPM of the baby, which must be between 60 and 240
int previous_value = 0; //This we keep track of the value from the previous loop, to see the comparison
int buttonState = 0; //variable for reading the pushbutton status only for testing


void setup() {
  Serial.begin(9600); // sets serial port for communication, to read on your pc
  pinMode(ButtonPin, INPUT); //Set port the pin from the button to only input  values. 
}

void loop() {

  buttonState = digitalRead(ButtonPin);

  //Check if the button is pressed, if it is the buttonstate is HIGH:
  if (buttonState == HIGH){
    Serial.print("Button is high");
    previous_value = digitalRead(LDRpin); //For the first time we run the loop we don't have a accurate value, so we need to set the previous value one time.
//    Serial.print("Previous value is: ");
//    Serial.println(previous_value);
    BPM = currentBPM(); //Call the function that will measure and calculate the current bpm
    Serial.print("BPM: ");
    Serial.println(BPM);              // Here the actual value gets printed
  }  
  delay(100);
}

int currentBPM(){
  int first_change = 0; 
  int second_change = 0;

  int timestart = 0; //Variable to see time
  int timestop = 0; //Variable to see time
  int timepassed = 0; //To store the time the has passed between the two variable above
  int beats = 1;

  int measuring_value = 0;
  previous_value = digitalRead(LDRpin);     // read the value from the LDR

//  Serial.print("Initialized");
  while(first_change == 0){    //While loop which stop if there is a bpm
    LDRValue = digitalRead(LDRpin);     // read the value from the LDR
    Serial.println(LDRValue);
//    Serial.print("first time looping");
    if(LDRValue != previous_value){
      timestart = millis();  //Takes the time the first change is noticed
      Serial.print("First change!");
      Serial.print("Previous: ");
      Serial.println(previous_value);
      first_change = 1;
    }
    previous_value = LDRValue; // Set the previous vale to the current loop for next loop
  }
  
  while(second_change == 0){
    LDRValue = digitalRead(LDRpin);
    Serial.println(LDRValue);
    if(LDRValue != previous_value){     // Checks of the hearbeat has changed from last time
      timestop = millis();   //Takes the time the second cange is noticed
      Serial.print("Second change");
      timepassed = timestop - timestart; //Calculate the time for one up or down time. 

      Serial.print("Timepassed: ");
      Serial.println(timepassed);

      if(timepassed > 500){
        Serial.print("beats");
        Serial.println(beats);
        BPM = ((0.5 *60*beats)/(0.001*timepassed)); //Calculate the current bpm which is time two, because you got half a cycle, times 0.001 because the time is in millis and times 60 because it is beats per MINUTE
  //      Serial.print("BPM is: ");
  //      Serial.println(BPM);
        second_change = 1;
      }
      else{
        beats = beats+1;
        previous_value = LDRValue; // Set the previous vale to the current loop for next loop
        Serial.print("adding a beat");
      }
    }
    previous_value = LDRValue; // Set the previous vale to the current loop for next loop
  }
  return BPM; //return the current bpm to the function, currently not really necessary because it is a global variable
}
