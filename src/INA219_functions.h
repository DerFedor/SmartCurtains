#pragma once
#include <Wire.h>
#include <Adafruit_INA219.h> // You will need to download this library

#define I2C_SDA 21
#define I2C_SCL 22

unsigned long sens_timing;           // Переменная для хранения точки отсчета
unsigned long period_measure = 1000; // Переменная для хранения периода замера
bool should_be_measured = false;     // Переменная для выполнения или не выполнения замеров

Adafruit_INA219 sensor219; // Declare and instance of INA219
float busVoltage = 0;
float current = 0; // Measure in milli amps
float power = 0;


void onSensTimer();