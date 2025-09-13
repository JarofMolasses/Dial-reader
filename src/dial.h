#ifndef DIALH
#define DIALH

#include "Arduino.h"

#define TIMEOUT 20
#define PACKETLENGTH 24
//MASK 0b 0000 1111 1111 1111 1111 1111
#define VALUEMASK 0x0FFFFF
//MASK 0b 1000 0000 0000 0000 0000 0000
#define UNITMASK 0b100000000000000000000000
//MASK 0b 0001 0000 0000 0000 0000 0000
#define SIGNMASK 0b000100000000000000000000

class DialReader
{
    private:
        uint8_t clkPin;
        uint8_t dataPin;
        volatile uint32_t raw = 0;
        volatile int32_t processed = 0;

        // crude state tracking
        volatile bool clk_state = 0;
        volatile bool clk_temp = 0;
        volatile uint32_t data = 0;
        volatile unsigned long lastNibbleTime = 0;
        volatile uint8_t bit = 0;

    public:
        DialReader(int clkPin_, int dataPin_);
        int32_t GetReading();
        void ProcessBit();

};

#endif