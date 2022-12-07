#include <M5Stack.h>
int freq1 = 0;  
int ampl = 0; 
int freq = 1000;
int channel = 0;
int resolution_bits = 8;
int buzzer = 1;
/*M5.Speaker.tone(uint32_t freq); to Generate a beep at a specified frequency (Hz)
M5.Speaker.mute();*/
void setup() {
  
    M5.begin();        // Init M5Core.  
    M5.Power.begin();  // Init Power module.  
    M5.Lcd.setTextColor(
        YELLOW);  // Set the font color to yellow.  
    M5.Lcd.setTextSize(2);     // Set the font size. 
    M5.Lcd.setCursor(65, 10);  // Move the cursor position to (65, 10).
                               
    M5.Lcd.println(
        "Select PWM");  //print instructions on the screen 
        
    M5.Lcd.setCursor(3, 35);
    //M5.Lcd.println("Press button A for 20B for 700ms");
    M5.Lcd.println("A=20% duty cycle");
    M5.Lcd.println("B=70% duty cycle");
    M5.Lcd.println("C=100% duty cycle");
    M5.Lcd.println("hold B for variable PWM");
    M5.Lcd.setTextColor(RED);
    ledcSetup(channel, freq, resolution_bits);
    ledcAttachPin(buzzer, channel);
    ledcAttachPin(0, 1);
}


void loop() {
    M5.update();  //which button corresponds to which PWM
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
         ledcWrite(channel, 50);
       
    } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
        ledcWrite(channel, 175);
        
    } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
        ledcWrite(channel, 250);
        
    } else if (M5.BtnB.wasReleasefor(700)) { //loop 
      for (int i = 0; i < 4; i++){
        for (int i = 255; i > 0; i--){
          ledcWrite(channel,i);
          delay(5);
        }
        for (int i = 0; i < 255; i++){
          ledcWrite(channel,i);
          ledcWrite(1,i);
          delay(5);
        }
      }
       //M5.Lcd.setCursor(0, 0);
    }
}