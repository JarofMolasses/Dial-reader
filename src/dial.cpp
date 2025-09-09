#include "dial.h"

DialReader::DialReader(int clkPin_, int dataPin_)
{
    clkPin = clkPin_;
    dataPin = dataPin_;

    pinMode(clkPin, INPUT);
    pinMode(dataPin, INPUT);

    lastNibbleTime = millis();
}

// Run as frequently as possible.
void DialReader::processNextBit()
{
    clk_temp = digitalRead(clkPin);

    if(clk_state == HIGH && clk_temp == LOW)     
    {
        if(millis() - lastNibbleTime > TIMEOUT)
        {
            bit = 0;
            raw = 0;
        }
        lastNibbleTime = millis();

        data = !digitalRead(dataPin); 
        raw |= data<<(bit);
        bit++;

        // Serial.print("Bit ");
        // Serial.print(bit);
        // Serial.print(", Data:");
        // Serial.println(data);
        // Serial.print("Raw:");
        // Serial.println(raw, BIN);
        
        if(bit == PACKETLENGTH)
        {
            int8_t negativeMultiplier = raw & SIGNMASK ? -1 : 1;            
            uint32_t value =  raw & VALUEMASK;

            processed = negativeMultiplier*value;
            bit = 0;
            raw = 0;
        }
    }
    clk_state = clk_temp;
}

int32_t DialReader::getReading()
{
    return processed;
}