#include <Arduino.h>
#include "mqtt_functions.h"

void setup()
{
  Serial.begin(115200);

  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, stepper.getPeriod(), true);
  timerAlarmEnable(My_timer); // Just Enable

  setup_wifi();

  // Настройка клиента MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // stepper.invertEn(true);
  stepper.autoPower(true);

  stepper.setMaxSpeed(Speed);            // скорость движения к цели
  stepper.setAcceleration(Acceleration); // ускорение
  // stepper.enable();
}

void loop()
{
  // Проверка подключения к MQTT серверу
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

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
