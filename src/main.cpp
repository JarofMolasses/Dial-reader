#include <Arduino.h>
#include "dial.h"

uint8_t commandBuffer[1];
uint8_t dataPin = 2;
uint8_t clkPin = 3; 
DialReader dial = DialReader(clkPin, dataPin);
int32_t reading = 0;
bool continuousPrint = false;

unsigned long printMillis;
const unsigned long printPeriod = 100;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("Dial indicator gaming");
    printMillis = millis();
}

void loop() {
    dial.ProcessBit();

    if(Serial.available())
    {
        Serial.readBytes(commandBuffer, 1);
        switch((uint8_t)commandBuffer[0])
        {
            case 'c':
                continuousPrint = !continuousPrint;
                break;
            case 's':
                reading = dial.GetReading();
                Serial.println(reading);
                break;

            default:
                break;
        }
    }

    // I'm too lazy to set up timer interrupt for this demo, but we can do that later.
    if(continuousPrint)
    {
        if(millis() - printMillis > printPeriod)      
        {
            digitalWrite(LED_BUILTIN, HIGH);
            reading = dial.GetReading();
            Serial.println(reading);
            printMillis = millis();
            digitalWrite(LED_BUILTIN,LOW);
        }
    }
}

