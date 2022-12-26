 #include <M5Stack.h>

int microphone = 35;
volatile int amount_of_peaks = 0;
int previous_amount_of_peaks = 0;
int previous_previous_amount_of_peaks = 0;
int last_time = 0;

void setup() {
  // Initializes the M5Stack
  M5.begin();
  M5.Power.begin();
  // Initializes the microphone
  pinMode(microphone, INPUT);
  // Prints the UI layout
  M5.Lcd.drawString("The value is:", 0, 0, 2);
  M5.Lcd.drawString("The level is:", 0, 20, 2);
  // Creates a rising interupt for the crying signal to count the amount of peaks
  attachInterrupt(microphone, count_peaks, RISING);
  // Initializes the timer
  last_time = millis();
}

void loop() {
  // Runs function every second
  if(millis() - last_time >= 1000){
    // Prints the amount of peaks and the level to the screen
    M5.Lcd.drawString("               ", 84, 0, 2);
    M5.Lcd.drawString((String) amount_of_peaks, 84, 0, 2);
    Serial.println(amount_of_peaks);
    M5.Lcd.drawString("               ", 84, 20, 2);
    M5.Lcd.drawString((String) difference_between_peaks(), 84, 20, 2);
    // Resets the peaks for a new count
    previous_previous_amount_of_peaks = previous_amount_of_peaks;
    previous_amount_of_peaks = amount_of_peaks;
    amount_of_peaks = 0;
    // Resets the timer
    last_time = millis();
  }
  delay(10);
}

// ISR to count the amount of peaks
void count_peaks(){
  amount_of_peaks++;
}

// Calculate difference between peaks -1 crying got worse, 0 remained, 1 got better
bool difference_between_peaks(){
  if((amount_of_peaks - previous_amount_of_peaks) <= -150 && (amount_of_peaks - previous_previous_amount_of_peaks) <= -150){
    return true;
  }
  return false;
}