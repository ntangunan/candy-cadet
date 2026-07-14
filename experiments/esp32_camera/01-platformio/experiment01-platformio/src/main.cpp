#include <Arduino.h>

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("================================");
    Serial.println(" Claptrap Experiment 01");
    Serial.println(" ESP32 Boot Successful");
    Serial.println("================================");
}

void loop()
{
    Serial.println("ESP32 Running");

    delay(1000);
}