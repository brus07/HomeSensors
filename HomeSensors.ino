/*
 Name:		HomeSensors01.ino
 Created:	7/30/2015 10:07:48 PM
 Author:	Ruslan
*/
#include "TaskScheduler.h"

#include "DoubleBlinker.h"
#include "DHTSensor.h"
#include "LightSensor.h"

#include <DHT.h>


//Scheduler
TaskScheduler *scheduler;
const int NUM_TASKS = 3;
Task *tasks[NUM_TASKS];


// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);

	tasks[0] = new DHTSensor(2, 20000);
	tasks[1] = new DoubleBlinker(13, 1000);
	scheduler = new TaskScheduler(tasks, NUM_TASKS);

  tasks[2] = new LightSensor(0, 10000);
}

// the loop function runs over and over again until power down or reset
void loop() {
	scheduler->run();
}
