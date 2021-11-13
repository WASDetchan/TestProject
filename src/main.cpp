#include <Arduino.h>
#include <BetterSerial/BetterSerial.h>

BetterSerial MySerial(&Serial, 9600);

void setup() {
    Serial.begin(9600);
}

void loop() {

}
