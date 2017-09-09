/**
 * Basic Lights Library for Techman83's home automation
 */

#include <pwmlights.h>
#include "Arduino.h"

pwmlights::pwmlights() {
  this->brightness  = 900;
  this->fadeAmount  = 20;
  this->dimDelay    = 100;
  this->dimLater    = 0;
  this->sw_state    = false;
  this->pwm_state    = false;
  this->pwm_dim      = false;
}

pwmlights& pwmlights::setPwmPin(uint8_t pwm_pin) {
  this->pwm_pin = pwm_pin;
  return *this;
}

pwmlights& pwmlights::setSwPin(uint8_t sw_pin) {
  this->sw_pin = sw_pin;
  return *this;
}

void pwmlights::Setup() {
  Serial.print("Intialising switch on");
  
  pinMode(pwm_pin, OUTPUT);
  pinMode(sw_pin, INPUT_PULLUP);
  analogWrite(pwm_pin, brightness);
}

void pwmlights::Loop(){
  int state = digitalRead(sw_pin);
  unsigned long timeNow = millis();

  if (state == 0 && sw_state && timeNow >= dimLater) {
    Serial.println("Adjusting brightness");
    dimLater = timeNow + dimDelay;
    // set the LED brightness
    analogWrite(pwm_pin, brightness);
  
    // increment/decrement the brightness for the next loop
    brightness = brightness + fadeAmount;
  
    // limit to 10-bit (0-1023)
    if (brightness < 300) brightness = 300;
    if (brightness > 885) brightness = 885;
  
    // reverse the direction of the fading at each end
    if (brightness == 300 || brightness == 885) {
      Serial.println("Reversing");
      dimLater = timeNow + 1500;
      fadeAmount = -fadeAmount;
    }
    pwm_state = true;
    pwm_dim = true;
  }
  
  if (state == 1 && sw_state && ! pwm_dim && timeNow >= dimLater) {
    if ( ! pwm_state ) {
      Serial.println("Power on");
      analogWrite(pwm_pin, 300);
      brightness = 299;
      pwm_state = true;
    } else {
      Serial.println("Power off");
      analogWrite(pwm_pin, 900);
      brightness = 900;
      pwm_state = false;
    }
  }

  if (state == 0 && ! sw_state && timeNow >= dimLater) {
    Serial.println("Setting SW_STATE true");
    sw_state = true;
    dimLater = timeNow + 250;
  }
  
  if (state == 1 && sw_state && timeNow >= dimLater) {
    Serial.println("Setting SW_STATE false");
    sw_state = false;    
    pwm_dim = false;
  }
}
