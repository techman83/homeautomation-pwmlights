#ifndef __PWMLIGHTS_H
#define __PWMLIGHTS_H

#include <cstdlib>
#include <stdint.h>

#ifdef ESP8266
  #include <functional>
  #define CALLBACK_SIGNATURE std::function<void(const char*, int)> callback
#else
  #define CALLBACK_SIGNATURE void (*callback)(const char*, int)
#endif

class pwmlights {
private:
  CALLBACK_SIGNATURE;
  const char* topic;
  uint8_t pwm_pin;
  uint8_t sw_pin;
  uint8_t relay_pin;
  int brightness = 900;
  int fadeAmount = 20;
  int dimDelay = 100;
  unsigned long dimLater = 0;
  volatile bool sw_state = false;
  volatile bool pwm_state = false;
  volatile bool pwm_dim = false;
public:
  pwmlights();
  pwmlights& setPwmPin(const uint8_t pwm_pin);
  pwmlights& setSwPin(const uint8_t sw_pin);
  pwmlights& setRelayPin(const uint8_t relay_pin);
  pwmlights& setCallback(CALLBACK_SIGNATURE, const char* topic);
  void Setup();
  void Loop();
};

#endif
