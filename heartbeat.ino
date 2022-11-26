// pins
int heart = 4;
int amp_out = 5;
int freq_out = 13;
// channels
int ledChannel_a = 0;
int ledChannel_f = 1;

// parameters

// delays
int init_delay = 2000;
int heart_delay = 9500;
int update_delay = 2300;

// signals
// pwm setting
float settings[5] = {0.05, 0.2, 0.4, 0.6, 0.8};
int frequency_cradle = 1000;
int resolution_cradle = 8;
// setting init
int freq_value = 5;
int amp_value = 5;

// BPM
int last_of_state = 0;
int last_of_BPM = 220;

// (new heartrate + heart_threshold < old heartrate) - means better heartrate BABY HAPPY :D
int heart_threshold = 10;
int bpm_threshold = 2;

//BPM array storage
const int bpm_storage_size = 3;
int bpm_storage[bpm_storage_size];

//debugging
int sensor_logs = 1;

void setF_A(int current_freq, int current_amp)
{
  // ledcWrite(pin, value)
  // vary the duty cycle between 0 and 255
  ledcWrite(amp_out, settings[current_amp - 1] * 255);
  ledcWrite(freq_out, settings[current_freq - 1] * 255);
}

void setup()
{
    Serial.begin(9600);
    Serial.println("STARTING");
    
    // pin modes
    pinMode(heart, INPUT);
    pinMode(freq_out, OUTPUT);
    pinMode(amp_out, OUTPUT);
    
    delay(init_delay);
    ledcSetup(freq_out, frequency_cradle, resolution_cradle);
    ledcSetup(amp_out, frequency_cradle, resolution_cradle);
    ledcAttachPin(freq_out, ledChannel_f);
    ledcAttachPin(amp_out, ledChannel_a);
    
    // freq and amp - max vals
    setF_A(freq_value, amp_value);
    delay(update_delay);
}

void loop()
{
    freq_value -= 1;
    setF_A(freq_value, amp_value);

    // hit one of the edges 
    // decrease freq and amp until we get to (freq,amp) = (1,1)
    if (amp_value == 1)
    {
        while (f_value > 1)
        {
            freq_value--;
            setF_A(freq_value, amp_value);
            delay(update_delay);
        }
    }

    if (freq_value == 1)
    {
        while (amp_value > 1)
        {
            amp_value--;
            setF_A(freq_value, amp_value);
            delay(update_delay);
        }
    }
}