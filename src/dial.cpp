#include "dial.h"

DialReader::DialReader(int clkPin_, int dataPin_)
{
    clkPin = clkPin_;
    dataPin = dataPin_;

    pinMode(clkPin, INPUT);
    pinMode(dataPin, INPUT);

    lastNibbleTime = millis();
}

/*
 * Scans for a single bit from the datastream. 
 */
void DialReader::ProcessBit()
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
            processed = negativeMultiplier * (raw & VALUEMASK);
            bit = 0;
            raw = 0;
        }
    }
    clk_state = clk_temp;
}

/*
 * Returns reading in microns as signed integer
 */
int32_t DialReader::GetReading()
{
    return processed;
}

