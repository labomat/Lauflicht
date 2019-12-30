//
// Rundumlicht v0.4
// Autor: Kai Laborenz
//
// blue chase light simulation with leds  
// positioned in a circular pattern
//
// speed can be changed by button press and 
// is stored on eeprom
//

#include <SoftPWM.h>
#include <EEPROM.h>

const byte MAX_LED = 8;
int umlaufzeit = 800;     // time for one "turn" oft the light
const int MAXZEIT = 300;  // maximum delay = slowest speed
const int MINZEIT = 20;   // minimum delay = maximum speed
const int FULL = 255;     // pwm value for maximum brightness of led
const int DIM = 3;        // pwm value for dimmed led
int zeit = umlaufzeit / MAX_LED;
int dir = -1;


uint8_t leds[8] = {2, 3, 4, 5, 6, 7, 8, 9};

void setup() {
  
  Serial.begin(9600);
  SoftPWMBegin();

  pinMode(12, INPUT_PULLUP);
  
  for (int i = 0; i < MAX_LED; i++) {
    pinMode(i+2, OUTPUT);
  }
  
  for (int i = 0; i < MAX_LED; i++)
    SoftPWMSet(leds[i], 0);

  SoftPWMSetFadeTime(ALL, 300, 300);

  zeit = EEPROM.read(0);
  Serial.println(zeit);
  
}


void loop() {

  for (int i = 0; i < MAX_LED; i++) {
      //Serial.println(i);
      if (i == 0) {                         // first led 
          SoftPWMSet(leds[i], FULL);
          SoftPWMSet(leds[i+1], DIM);
          SoftPWMSet(leds[MAX_LED-1], DIM);
          SoftPWMSet(leds[MAX_LED-2], 0);
      }
      else if (i == 1) {                    // second led
          SoftPWMSet(leds[i], FULL);
          SoftPWMSet(leds[i+1], DIM);
          SoftPWMSet(leds[i-1], DIM);
          SoftPWMSet(leds[MAX_LED-1], 0);
      }
      else if (i == MAX_LED-1) {            // last -1 led before turn is complete
          SoftPWMSet(leds[i], FULL);
          SoftPWMSet(leds[0], DIM);
          SoftPWMSet(leds[i-1], DIM);
          SoftPWMSet(leds[i-2], 0);
       }
      else {
        SoftPWMSet(leds[i+1], DIM);
        SoftPWMSet(leds[i], FULL);
        SoftPWMSet(leds[i-1], DIM);
        SoftPWMSet(leds[i-2], 0);
      }
      delay(zeit);
      // press button to change speed
      // and update result on eeprom
      if(!digitalRead(12)) {
        Serial.println(zeit);
        zeit = zeit + (5*dir);
        if (zeit < MINZEIT) dir = 1;
        if (zeit > MAXZEIT) dir = -1;
        SoftPWMSetFadeTime(ALL, (zeit*3), (zeit*3));
        EEPROM.update(0, zeit);
      }
  }
  
}
