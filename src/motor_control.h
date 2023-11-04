#pragma once

#include <GyverStepper2.h>
#define STEP_PIN 0
#define DIR_PIN 2
#define ENABLE_PIN 4

GStepper2< STEPPER2WIRE> stepper(6400, STEP_PIN, DIR_PIN, ENABLE_PIN);
hw_timer_t *My_timer = NULL;

float Speed = 30000;
float Acceleration = 30000;

void IRAM_ATTR onTimer() {
 // мотор асинхронно крутится тут
  if (stepper.tickManual()) timerAlarmWrite(My_timer, stepper.getPeriod(), true);
  else timerStop(My_timer);
}




