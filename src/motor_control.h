#pragma once

#include <GyverStepper2.h>
#include "INA219_functions.h"

#define STEP_PIN 15
#define DIR_PIN 2
#define ENABLE_PIN 4

GStepper2<STEPPER2WIRE> stepper(6400, STEP_PIN, DIR_PIN, ENABLE_PIN);
hw_timer_t *step_timer = NULL;

float Speed = 30000;
float Acceleration = 30000;

void IRAM_ATTR onTimer()
{
    // мотор асинхронно крутится тут
    if (stepper.tickManual())
    {
        timerAlarmWrite(step_timer, stepper.getPeriod(), true);
    }
    else
    {
        timerStop(step_timer);
        // should_be_measured = false;
    }
}
