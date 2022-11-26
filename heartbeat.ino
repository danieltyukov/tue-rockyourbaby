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
