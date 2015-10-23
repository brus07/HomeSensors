#ifndef MONITOR_H
#define MONITOR_H

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "Task.h"

class Monitor : public TimedTask {
public:
	Monitor(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
	virtual void run(uint32_t now);
private:
  LiquidCrystal_I2C lcd;

  //0 - starting
  int State = 0; 
};

Monitor::Monitor(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows) : TimedTask(millis()), lcd(lcd_addr, lcd_cols, lcd_rows){
}

void Monitor::run(uint32_t now)
{
	switch (State)
	{
	case 0:
    lcd.begin();
    // Turn on the blacklight and print a message.
    lcd.backlight();
    lcd.print("Hello, World!");
		State = 1;
    setRunTime(5000);
    break;
	case 1:
    lcd.setCursor(0, 1);
    lcd.print("Working...");
    State = 2;
    break;
  case 2:
    break;
	}
}

#endif
