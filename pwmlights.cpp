/**
 * Basic Lights Library for Techman83's home automation
 */

#include <pwmlights.h>
#include "Arduino.h"

pwmlights::pwmlights() {
  this->callback    = nullptr;
  this->topic       = "test";
  this->brightness  = 886;
  this->fadeAmount  = 20;
  this->dimDelay    = 100;
  this->dimLater    = 0;
  this->sw_state    = false;
  this->pwm_state   = false;
  this->pwm_dim     = false;
}

pwmlights& pwmlights::setPwmPin(uint8_t pwm_pin) {
  this->pwm_pin = pwm_pin;
  return *this;
}

pwmlights& pwmlights::setSwPin(uint8_t sw_pin) {
  this->sw_pin = sw_pin;
  return *this;
}

pwmlights& pwmlights::setRelayPin(uint8_t relay_pin) {
  this->relay_pin = relay_pin;
  return *this;
}

pwmlights& pwmlights::setCallback(CALLBACK_SIGNATURE, const char* topic) {
    if (callback != nullptr) {
      this->callback = callback;
    }
    this->topic = topic;
    return *this;
}


void pwmlights::Setup() {
  Serial.println("Intialising switch on");
  
  pinMode(pwm_pin, OUTPUT);
  pinMode(relay_pin, OUTPUT);
  pinMode(sw_pin, INPUT_PULLUP);
  analogWrite(pwm_pin, brightness);
}

int getPercentage(int brightness) {
  float calc_brightness = brightness - 300;
  calc_brightness = (calc_brightness / 585) * 100;
  return int (100 - calc_brightness);
}

void pwmlights::Loop(){
  int state = digitalRead(sw_pin);
  unsigned long timeNow = millis();

  if (state == 0 && sw_state && timeNow >= dimLater) {
    Serial.println("Adjusting brightness");
    dimLater = timeNow + dimDelay;

    // set the brightness
    analogWrite(pwm_pin, brightness);

    // increment/decrement the brightness
    brightness = brightness + fadeAmount;
  
    // limit to 10-bit (0-1023)
    if (brightness < 300) brightness = 300;
    if (brightness > 880) brightness = 880;
  
    // reverse the direction of the fading at each end
    if (brightness == 300 || brightness == 880) {
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
      brightness = 300;
      pwm_state = true;
    } else {
      Serial.println("Power off");
      analogWrite(pwm_pin, 885);
      brightness = 885;
      digitalWrite(relay_pin, LOW);
      pwm_state = false;
    }
  }

  if (state == 0 && ! sw_state && timeNow >= dimLater) {
    Serial.println("Setting SW_STATE true");
    sw_state = true;
    digitalWrite(relay_pin, HIGH);
    // Debounce
    dimLater = timeNow + 250;
  }
  
  if (state == 1 && sw_state && timeNow >= dimLater) {
    Serial.println("Setting SW_STATE false");
    sw_state = false;    
    pwm_dim = false;
    callback(topic, getPercentage(brightness));
  }
}
