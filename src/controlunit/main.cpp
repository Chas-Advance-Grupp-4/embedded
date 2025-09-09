#include <Arduino.h>

int counter {0};

void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.print("Hello ESP32 No ");
    Serial.println(counter);
    counter++;
}

void loop()
{
    delay(2000);
    Serial.print("Hello ESP32 No ");
    Serial.println(counter);
    counter++;
}