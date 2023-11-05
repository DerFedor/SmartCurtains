#pragma once
#ifndef MQTT_FUNCTIONS_H
#define MQTT_FUNCTIONS_H

#include <PubSubClient.h>
#include <WiFi.h>
#include <Arduino.h>
#include "motor_control.h"

// Топики MQTT
const char *curtains_topic = "MyRoom/curtains";
// const char *curtains_topic_position = "MyRoom/curtains/position";
const char *curtains_topic_direction = "MyRoom/curtains/direction";
const char *curtains_topic_mode = "MyRoom/curtains/mode";
const char *curtains_topic_opening = "MyRoom/curtains/opening";
// const char *curtains_topic_open = "MyRoom/curtains/open";
// const char *curtains_topic_close = "MyRoom/curtains/close";

// Настройки подключения к Wi-Fi и MQTT серверу
const char *ssid = "DF_";
const char *password = "040301()";

// Ваши учетные данные MQTT
const char *mqtt_server = "m5.wqtt.ru";
const int mqtt_port = 5554;
const char *mqtt_user = "u_NRGVQJ";
const char *mqtt_pass = "SdORcIUH";

WiFiClient espClient;
PubSubClient client(espClient);

bool sent;
bool firstStart = true;

void setup_wifi()
{
  delay(10);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect("Esp32Curtains", mqtt_user, mqtt_pass))
    {
      client.subscribe(curtains_topic);
      client.subscribe(curtains_topic_direction);
      client.subscribe(curtains_topic_mode);
      client.subscribe(curtains_topic_opening);
      // client.subscribe(curtains_topic_position);
      // client.subscribe(curtains_topic_close);
    }
    else
    {
      delay(5000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  String msg = String((char *)payload);
  Serial.print(topic); // выводим в сериал порт название топика
  Serial.print(" => ");
  Serial.println(msg); // чтение данных

  if (strcmp(topic, curtains_topic) == 0) // Обнуление
  {
    if (msg.toInt() == 1)
    {
      if (!firstStart)
      {
        stepper.brake();
        stepper.reset();
        client.publish(curtains_topic, "0");
        client.publish(curtains_topic_opening, "0");
      }
    }
  }
  else if (strcmp(topic, curtains_topic_direction) == 0) // Смена направления
  {
    stepper.reverse(msg.toInt());
  }
  else if (strcmp(topic, curtains_topic_opening) == 0) // Открытие-вращение
  {
    int targetPosition = msg.toInt();
    if (firstStart)
    {
      stepper.setCurrent(targetPosition);
      // client.publish(curtains_topic_opening, (char *)targetPosition);
      firstStart = false;
    }
    else
    {
      // should_be_measured = true;
      // period_measure = 200;
      stepper.setTarget(targetPosition);
      timerAlarmWrite(step_timer, stepper.getPeriod(), true);
      timerStart(step_timer);
    }
  }
  else if (strcmp(topic, curtains_topic_mode) == 0) // Режимы 1 - нормальный, 2 - медленный, 3 - быстрый
  {
    if (msg.toInt() == 1)
    {
      stepper.setMaxSpeed(10000);     // скорость движения к цели
      stepper.setAcceleration(10000); // ускорение
    }
    else if (msg.toInt() == 2)
    {
      stepper.setMaxSpeed(20000);     // скорость движения к цели
      stepper.setAcceleration(20000); // ускорение
    }
    else if (msg.toInt() == 3)
    {
      stepper.setMaxSpeed(30000);     // скорость движения к цели
      stepper.setAcceleration(30000); // ускорение
    }
    else if (msg.toInt() == 4)
    {
      stepper.enable();
      stepper.setSpeed(10000);
      should_be_measured = true;
      period_measure = 200;
      timerAlarmWrite(step_timer, stepper.getPeriod(), true);
      timerStart(step_timer);
    }
  }
}
#endif