
// [===========================  PIN NUMBERS  ===========================]
int heart = 4; // The heartbeat sensor
int mic = A0;  // The microphone filter/amp output
int f_out = 7; // Frequency output
int a_out = 5; // Amplitude output

// [===========================  PARAMETERS  ===========================]
int initial_delay = 2000;                       // Delay to be used at the start of the program
int heart_delay = 9500;                         // The delay of the heartbeat signal with respect to the stress level
int f_value = 5;                                // The value of the frequency [1-5]
int a_value = 5;                                // The value of the amplitude [1-5]
float settings[5] = {0.05, 0.2, 0.4, 0.6, 0.8}; // The PWM settings of f and a

// [===========================  *** SET F AND A ***  ===========================]
void setF_A(int current_freq, int current_ampl)
{
  // analogWrite(pin, value)
  analogWrite(a_out, settings[current_ampl - 1] * 255);
  analogWrite(f_out, settings[current_freq - 1] * 255);
}

// [===========================  *** SETUP ***  ===========================]
void setup()
{
  Serial.begin(9600);                                                                             // Open the serial-port at 9600 baud [= bits/sec]
  Serial.println("[===========================  *** BOOTING ***  ===========================]");

  pinMode(heart, INPUT);                                                                          // Set the pinmodes to the respective functions of the pins
  pinMode(mic, INPUT);
  pinMode(f_out, OUTPUT);
  pinMode(a_out, OUTPUT);

  setF_A(f_value, a_value);                                                                       // Setting the frequency and the amplitude to their max values initially
  Serial.println("Set to initial values!");
}

// [===========================  *** MAIN LOOP ***  ===========================]
void loop()
{
    // I have the logic constructed on paper
    // will discuss next meeting
}