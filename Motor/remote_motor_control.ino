#include <M5Stack.h>

int freq1 = 0;  
int ampl = 0;

int freq = 1000;
int resolution_bits = 8;
float settingsFREQ[5] = {0.05, 0.2, 0.4, 0.6, 0.8};
float settingsAMP[5] = {0.05, 0.2, 0.4, 0.6, 0.8};

int AMPchannel = 0;
int FREQchannel = 1;

int pinAmp = 1;
int pinFreq = 3;

String mode = "A";

void setup() {
  
    // Init M5Core.  
    M5.begin();
    // Init Power module.
    M5.Power.begin();

    // Set the font color to yellow. Set the font size.
    M5.Lcd.setTextColor(YELLOW);  
    M5.Lcd.setTextSize(2);
                               
    //print instructions on the screen
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.println("3rd Button to Switch PWM");
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.println("A Button to Lower PWM");
    M5.Lcd.setCursor(0, 60);
    M5.Lcd.println("B Button to Raise PWM");
        
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.println("0% duty cycle");
    M5.Lcd.println("20% duty cycle");
    M5.Lcd.println("40% duty cycle");
    M5.Lcd.println("60% duty cycle");
    M5.Lcd.println("80% duty cycle");

    M5.Lcd.setTextColor(RED);
    M5.Lcd.setCursor(0, 180);
    M5.Lcd.println("MODE");

    // Pin Init
    ledcAttachPin(pinAmp, AMPchannel);
    ledcAttachPin(pinFreq, FREQchannel);

    // PWM Signal setup
    ledcSetup(AMPchannel, freq, resolution_bits);
    ledcSetup(FREQchannel, freq, resolution_bits);
    
    // PWM Signal
    ledcWrite(AMPchannel, 0.8*250);
    ledcWrite(FREQchannel, 0.8*250);
}


void loop() {
    // Cycle PWM Signal Settings
    M5.update();
    // EXAMPLE: settings[0] * 255 ==> 0.05 * 255 = 12.75 Duty Cycle

    // C BUTTON => Change Mode
    // RED => FREQ
    // GREEN => AMP
    if (M5.BtnC.wasReleased()) {
        if (mode == "A") {
            mode = "F";
            M5.Lcd.setTextColor(RED);
            M5.Lcd.setCursor(0, 180);
            M5.Lcd.println("MODE");
        }
        else if (mode == "F") {
            mode = "A";
            M5.Lcd.setTextColor(GREEN);
            M5.Lcd.setCursor(0, 180);
            M5.Lcd.println("MODE");
        }
    }
    else if (M5.BtnA.wasReleased()) {
        // LOWER DUTY CYCLE
        if (mode == "A") {
            // lower amp1 by 1, if amp1 is 0, set it to 0
            if (ampl > 0) {
                ampl--;
            }
            else {
                ampl = 0;
            }
            ledcWrite(AMPchannel, settingsAMP[ampl] * 255);
        }
        else if (mode == "F") {
            // lower freq1 by 1, if freq1 is 0, set it to 0
            if (freq1 > 0) {
                freq1--;
            }
            else {
                freq1 = 0;
            }
            ledcWrite(FREQchannel, settingsFREQ[freq1] * 255);
        }
    }
    else if (M5.BtnB.wasReleased()) {
        // HIGHER DUTY CYCLE
        if (mode == "A") {
            // raise amp1 by 1, if amp1 is 4, set it to 4
            if (ampl < 4) {
                ampl++;
            }
            else {
                ampl = 4;
            }
            ledcWrite(AMPchannel, settingsAMP[ampl] * 255);
        }
        else if (mode == "F") {
            // raise freq1 by 1, if freq1 is 4, set it to 4
            if (freq1 < 4) {
                freq1++;
            }
            else {
                freq1 = 4;
            }
            ledcWrite(FREQchannel, settingsFREQ[freq1] * 255);
        }
    }
    // display the current frequency and amplitude duty cycle
    M5.Lcd.setCursor(0, 200);
    M5.Lcd.setTextColor(YELLOW, BLACK);
    // if at 0% duty cycle, display 05.00% instead of 5.00%
    if (settingsFREQ[freq1] * 100 < 10) {
        M5.Lcd.println("FREQ: 0" + String(settingsFREQ[freq1] * 100) + "%");
    }
    else {
        M5.Lcd.println("FREQ: " + String(settingsFREQ[freq1] * 100) + "%");
    }
    if (settingsAMP[ampl] * 100 < 10) {
        M5.Lcd.println("AMP: 0" + String(settingsAMP[ampl] * 100) + "%");
    }
    else {
        M5.Lcd.println("AMP: " + String(settingsAMP[ampl] * 100) + "%");
    }
}