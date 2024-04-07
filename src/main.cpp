#include <Arduino.h>
#include "mqtt_functions.h"
// AsyncWebServer server(80);

void setup()
{
  Wire.begin(I2C_SDA, I2C_SCL); // define which pins used for i2c
  Serial.begin(115200);
  sensor219.begin();

  // power_timer = timerBegin(3, 80, true);               // таймер для для датчика тока
  // timerAttachInterrupt(power_timer, &onSensTimer, true);
  // timerAlarmWrite(power_timer, 1000000, true);
  // timerAlarmEnable(power_timer); // Just Enable

  step_timer = timerBegin(0, 80, true); // таймер для шаговика
  timerAttachInterrupt(step_timer, &onTimer, true);
  timerAlarmWrite(step_timer, stepper.getPeriod(), true);
  timerAlarmEnable(step_timer); // Just Enable

  setup_wifi();
  setup_OTA();

  // Настройка клиента MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // stepper.invertEn(true);
  stepper.autoPower(true);
  stepper.disable();

  stepper.setMaxSpeed(Speed);            // скорость движения к цели
  stepper.setAcceleration(Acceleration); // ускорение
  // stepper.enable();
}

void loop()
{
  ArduinoOTA.handle();
  // Проверка подключения к MQTT серверу
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if ((millis() - sens_timing > period_measure) && should_be_measured)
  { // Вместо 10000 подставьте нужное вам значение паузы
    sens_timing = millis();
    onSensTimer();
    // Serial.println("measure_curent_ ");
  }

  if (stepper.ready())
  {
    should_be_measured = false;
  }

  // static uint32_t tmr;
  // if (millis() - tmr >= 30)
  // {
  //   tmr = millis();
  //   Serial.println(stepper.getPeriod());
  // }

  // if (stepper.ready() &&
  //     !sent)
  // {
  //   client.publish(curtains_topic_position, (char *)stepper.getCurrent());
  //   sent = true;
  // }
}

void onSensTimer()
{
  // буду замерять ток и напряжение
  busVoltage = sensor219.getBusVoltage_V();
  current = sensor219.getCurrent_mA();
  power = busVoltage * (current / 1000); // Calculate the Power
  // Serial.print("Current:       ");
  // Serial.print(current);
  // Serial.println(" mA");
  if (current > 500)
  {
    Serial.println("Warning!!!");
    stepper.brake();
    stepper.reset();
    should_be_measured = false;
    client.publish(curtains_topic_opening, "0");
  }
}