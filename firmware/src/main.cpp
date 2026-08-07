#include <Arduino.h>

#include "app/application.h"

App::Application application;

void setup()
{
  Serial.begin(115200);
  Serial.println("BOOTING CANDY CADET");

  application.initialize();
}

void loop()
{
  application.update();
}