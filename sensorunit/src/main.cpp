#include <Arduino.h>
#include <etl/string.h>
#include <etl/vector.h>

int             counter{0};
etl::string<32> hello{};

void setup() {
    hello.append("Hello ETL Arduino No ");

    Serial.begin(115200);
    delay(2000);

    Serial.print(hello.c_str());
    Serial.println(counter);
    counter++;
}

void loop() {
    delay(2000);
    Serial.print(hello.c_str());
    Serial.println(counter);
    counter++;
}