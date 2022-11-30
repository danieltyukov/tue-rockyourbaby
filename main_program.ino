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

// (new heartrate + heartbeat_threshold < old heartrate) - means better heartrate BABY HAPPY :D
int heartbeat_threshold = 10;
int bpm_threshold = 2;

//BPM array storage
const int bpm_storage_size = 3;
int bpm_storage[bpm_storage_size];

//debucheck_stateing
int sensor_logs = 1;

void setFreqAmp(int current_freq, int current_amp)
{
    // ledcWrite(pin, value)
    // vary the duty cycle between 0 and 255
    ledcWrite(amp_out, settings[current_amp - 1] * 255);
    ledcWrite(freq_out, settings[current_freq - 1] * 255);
}

int heartbeatFunction()
{
    // enter loop until equilibrium is reached
    bool equilibrium_reached = false;
    // empty array
    int constArray[bpm_storage_size] = {NULL};
    int i = 0;
    // current time at which we start measuring
    long prior_time = millis();
    while (!equilibrium_reached)
    {
        // bool used to check if we reached the equilibrium                                                                  
        bool check_state = true;
        // read heartbeat circuit input
        int new_of_state = digitalRead(heart);
        if ((new_of_state != last_of_state) && (new_of_state == HIGH))
        {
            // check at which time the heartbeat goes to the edge
            long new_time = millis();
            // change in starting time and time of heartbeating going to edge
            long interval = new_time - prior_time;
            prior_time = new_time;
            // calculate the Beats Per Minute
            int BPM = 60000 / ((int)interval);

            // checking for recorded Beats Per Minute are within the established range
            if (BPM >= 60 - heartbeat_threshold && BPM <= 240 + heartbeat_threshold)
            {
                // if the BPM is within the range, we add it to the array
                constArray[i % bpm_storage_size] = BPM;
                i++;
                // if the array is full, we check if the values are within the established range
                if (i >= bpm_storage_size)
                {
                    int j = 0;
                    // For all adjacent couples of values in the array AND the boolean check_state
                    while ((j < 2) && check_state)
                    {
                        // checking if the two adjacent vals inside the array are too far apart
                        if (abs(constArray[j] - constArray[j + 1]) > bpm_threshold)
                        {
                        // equilibrium not yet reached
                        check_state = false; 
                        }
                        j++;
                    }
                    // if the check_state is still true, we have reached equilibrium
                    // the adjacent values we were comparing inside the array are similar
                    if (check_state)
                    {
                        // equilibrium was reached and we end the loop
                        equilibrium_reached = true;
                    }
                }
            }
        }
        // We update the last state to the new state of the rising to edge stress on computer
        last_of_state = new_of_state;
    }

    // we return the average Beats per Minute of the array
    int sum = 0;
    for (int i = 0; i < bpm_storage_size; i++)
    {
        sum += constArray[i];
    }
    int averageBPM = sum / bpm_storage_size;
    Serial.println("Average BPM: ");
    Serial.println(averageBPM);
    return averageBPM;
}

bool readSensorVals()
{
    Serial.print("READ THE SENSOR: ");
    Serial.print(sensor_logs);

    sensor_logs++;

    delay(heart_delay - update_delay);
    int new_of_BPM = heartbeat();
    Serial.print("The updated heartbeat: ");
    Serial.println(new_of_BPM);
    Serial.print("The previous heartbeat: ");
    Serial.println(last_of_BPM);
    bool return_val;

    // check the different against the threshhold
    if (new_of_BPM - last_of_BPM < -1 * heartbeat_threshold)
    {
        // new_of_BPM + heartbeat_threshold < last_of_BPM
        // which means that the baby stress level went down
        return_val = true;
    }
    else
    {
        return_val = false;
    }
    last_of_BPM = new_of_BPM;
    return return_val;
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
    setFreqAmp(freq_value, amp_value);
    delay(update_delay);
}

void loop()
{
    freq_value -= 1;
    setFreqAmp(freq_value, amp_value);
    bool response_of_sensor = readSensorVals();

    // if sensor recorded a degrading stress level:
    if (response_of_sensor == false)
    {
        // inc freq_value and amp_value +1
        freq_value += 1;
        setF_A(freq_value, amp_value);
        delay(1);
        amp_value -= 1;
        setF_A(freq_value, amp_value);
        delay(update_delay);
        // Beats per minute decrease by 20 beats per minute
        last_of_BPM -= 20;
    }

    // hit one of the edges 
    // decrease freq and amp until we get to (freq,amp) = (1,1)
    if (amp_value == 1)
    {
        while (freq_value > 1)
        {
            freq_value--;
            setFreqAmp(freq_value, amp_value);
            delay(update_delay);
        }
    }

    if (freq_value == 1)
    {
        while (amp_value > 1)
        {
            amp_value--;
            setFreqAmp(freq_value, amp_value);
            delay(update_delay);
        }
    }
}