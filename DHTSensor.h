#ifndef DHT_SENSOR_H
#define DHT_SENSOR_H

#include <Arduino.h>
#include <DHT.h>
#include "Task.h"


//DHT
//#define DHTPIN 2     // what pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor


// Timed task to blink a LED.
class DHTSensor : public TimedTask {
public:
	DHTSensor(uint8_t _pin, uint32_t _rate);
	virtual void run(uint32_t now);
	double Humidity;
	double Temperature;
	uint32_t LastReadTime;
private:
	DHT dht;
	uint8_t pin;      // LED pin.
	uint32_t rate;    // Blink rate.
	uint32_t startTiming;

	int State = 0; //0 - need first read, 

	static const int CIRCLES_COUNT = 5;

	double temperatures[CIRCLES_COUNT];
	double humidities[CIRCLES_COUNT];
	int countValues = 0;

	void AddValues(double temperature, double humidity);
	double GetMediumTemperature();
	double GetMediumHumidity();

	double GetMedium(double *arr, int count);
};

DHTSensor::DHTSensor(uint8_t _pin, uint32_t _rate) : TimedTask(millis()), pin(_pin), rate(_rate), dht(_pin, DHTTYPE), countValues(0){
}

void DHTSensor::run(uint32_t now)
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
			double h = dht.readHumidity();
			double t = dht.readTemperature();

			// Check if any reads failed and exit early (to try again).
			if (isnan(Humidity) || isnan(Temperature)) {
				Serial.println("Failed to read from DHT sensor!");
				incRunTime(2100);
				return;
			}

			AddValues(t, h);

			incRunTime(2100);
			break;
		}
		else
		{
			State = 2;
		}
	case 2:
		State = 0;
		Temperature = GetMediumTemperature();
		Humidity = GetMediumHumidity();
		LastReadTime = millis();

		Serial.print("Humidity: ");
		Serial.print(Humidity);
		Serial.print(" %\t");
		Serial.print("Temperature: ");
		Serial.print(Temperature);
		Serial.println(" *C");

		setRunTime(startTiming + rate);
		break;
	}
}

void DHTSensor::AddValues(double temperature, double humidity)
{
	temperatures[countValues] = temperature;
	humidities[countValues] = humidity;
	countValues++;
}

double DHTSensor::GetMedium(double *arr, int count)
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

double DHTSensor::GetMediumTemperature()
{
	return GetMedium(temperatures, countValues);
}

double DHTSensor::GetMediumHumidity()
{
	return GetMedium(humidities, countValues);
}

#endif