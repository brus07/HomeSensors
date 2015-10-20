#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include <Arduino.h>
#include "Task.h"


class LightSensor : public TimedTask {
public:
	LightSensor(uint8_t _pin, uint32_t _rate);
	virtual void run(uint32_t now);
	double Light;
	uint32_t LastReadTime;
private:
	uint8_t pin;      // LED pin.
	uint32_t rate;    // Blink rate.
	uint32_t startTiming;
 
  //0 - start (prepare) state
  //1 - get values
  //2 - output state
	int State = 0; 

	static const int CIRCLES_COUNT = 5;

	double lightes[CIRCLES_COUNT];
	int countValues = 0;

	void AddValue(double light);
	double GetMediumLight();

	double GetMedium(double *arr, int count);
};

LightSensor::LightSensor(uint8_t _pin, uint32_t _rate) : TimedTask(millis()), pin(_pin), rate(_rate), countValues(0){
}

void LightSensor::run(uint32_t now)
{
	switch (State)
	{
	case 0:
		countValues = 0;
		startTiming = now;
		State = 1;
	case 1:
		if (countValues < CIRCLES_COUNT)
		{
			double light = analogRead(pin);
      light /= (1024/10); //res in [0;10)

			AddValue(light);

			incRunTime(50);
			break;
		}
		else
		{
			State = 2;
		}
	case 2:
		State = 0;
		Light = GetMediumLight();
		LastReadTime = millis();

		Serial.print("Light: ");
		Serial.println(Light);

		setRunTime(startTiming + rate);
		break;
	}
}

void LightSensor::AddValue(double light)
{
	lightes[countValues] = light;
	countValues++;
}

double LightSensor::GetMedium(double *arr, int count)
{
	//sort :)
	bool swaped = true;
	while (swaped)
	{
		swaped = false;
		for (int i = 1; i < count; i++)
		{
			if (arr[i] < arr[i - 1])
			{
				double temp = arr[i];
				arr[i] = arr[i - 1];
				arr[i - 1] = temp;
				swaped = true;
			}
		}
	}

	//get average, but cut the lowest and the highest
	double value = 0;
	int countOfValues = 0;
	for (int i = 1; i < count - 1; i++)
	{
		value += arr[i];
		countOfValues++;
	}
	return value / countOfValues;
}

double LightSensor::GetMediumLight()
{
	return GetMedium(lightes, countValues);
}

#endif
