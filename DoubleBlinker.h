#ifndef DoubleBlinker_h
#define DoubleBlinker_h 

#include <Arduino.h>
#include "Task.h"

// Timed task to blink a LED.
class DoubleBlinker : public TimedTask {
public:
    DoubleBlinker(uint8_t _pin, uint32_t _rate);
	virtual void run(uint32_t now);
	uint32_t rate;    // Blink rate.
private:
    uint8_t pin;      // LED pin.
    int state;          // 0 - wait rate/2, 1 on rate/6, 2 off rate/6, 3 on rate/6
};

DoubleBlinker::DoubleBlinker(uint8_t _pin, uint32_t _rate) : TimedTask(millis()), pin(_pin), rate(_rate), state(0) {
    pinMode(pin, OUTPUT);     // Set pin for output.
}

void DoubleBlinker::run(uint32_t now)
{
  switch(state) {
    case 0:
      digitalWrite(pin, LOW);
      state = 1;
      incRunTime(rate/2);
      break;
    case 1:
      digitalWrite(pin, HIGH);
      state = 2;
      incRunTime(rate/6);
      break;
    case 2:
      digitalWrite(pin, LOW);
      state = 3;
      incRunTime(rate/6);
      break;
    case 3:
      digitalWrite(pin, HIGH);
      state = 0;
      incRunTime(rate/6);
      break;
  }
}
#endif

