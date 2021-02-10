//
// Rundumlicht v0.5
// Autor: Kai Laborenz
//
// blue chase light simulation with leds  
// positioned in a circular pattern
//
// speed can be changed by button press and 
// is stored on eeprom
//

#include <SoftPWM.h>      // needed for light fading on all ports
#include <EEPROM.h>       // needed to save the speed settings on eeprom

// pins for leds
const byte MAX_LED = 8;
uint8_t leds[8] = {2, 3, 4, 5, 6, 7, 8, 9};

// pin for speed setting button
#define SPEEEDPIN 12

int umlaufzeit = 800;     // time for one "turn" oft the light
const int MAXZEIT = 300;  // maximum delay = slowest speed
const int MINZEIT = 20;   // minimum delay = maximum speed
int zeit = umlaufzeit / MAX_LED;
int dir = -1;

const int FULL = 255;     // pwm value for maximum brightness of led
const int DIM = 3;        // pwm value for dimmed led


//#define DEB_ON            // remove comment to debug

#ifdef DEB_ON
  #define DEBUG_PRINT(str)  Serial.print(str)
  #define DEBUG_PRINTLN(str)  Serial.println(str)
#else
  #define DEBUG_PRINT(str)
  #define DEBUG_PRINTLN(str)
#endif

void setup() {
  
  Serial.begin(9600);
  SoftPWMBegin();

  pinMode(SPEEEDPIN, INPUT_PULLUP);
  
  for (int i = 0; i < MAX_LED; i++) {
    pinMode(i+2, OUTPUT);
  }
  
  for (int i = 0; i < MAX_LED; i++)
    SoftPWMSet(leds[i], 0);

  SoftPWMSetFadeTime(ALL, 300, 300);

  zeit = EEPROM.read(0);

  DEBUG_PRINTLN(zeit);
  
}

void loop() {

  for (int i = 0; i < MAX_LED; i++) {
      //Serial.println(i);
      if (i == 0) {                         // first led 
          SoftPWMSet(leds[i], FULL);
          SoftPWMSet(leds[MAX_LED-2], 0);
      }
      else if (i == 1) {                    // second led
          SoftPWMSet(leds[i], FULL);
          SoftPWMSet(leds[MAX_LED-1], 0);
      }
      else if (i == MAX_LED-1) {            // last -1 led before turn is complete
          SoftPWMSet(leds[i], FULL);
          SoftPWMSet(leds[i-2], 0);
       }
      else {
        SoftPWMSet(leds[i], FULL);
        SoftPWMSet(leds[i-2], 0);
      }
      delay(zeit);
      
      // press button to change speed
      // and update result on eeprom
      if(!digitalRead(SPEEEDPIN)) {
        DEBUG_PRINTLN(zeit);
        zeit = zeit + (5*dir);
        if (zeit < MINZEIT) dir = 1;
        if (zeit > MAXZEIT) dir = -1;
        SoftPWMSetFadeTime(ALL, (zeit*3), (zeit*3));
        EEPROM.update(0, zeit);
      }
  }
  
}
