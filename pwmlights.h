#ifndef __PWMLIGHTS_H
#define __PWMLIGHTS_H

#include <cstdlib>
#include <stdint.h>

class pwmlights {
private:
  uint8_t pwm_pin;
  uint8_t sw_pin;
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
  pwmlights& setSwPin(const uint8_t pwm_pin);
  void Setup();
  void Loop();
};

#endif
