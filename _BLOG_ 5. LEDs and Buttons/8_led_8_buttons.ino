/**
 * \file _8_LEDs_74hc595
 *
 *      test program for 8 LEDs connected to a 74HC595
 *
 * \author Allan M Schwartz, CodeValue
 *      allans@codevalue.net
 *      April 2017
 *
 * \note
 *      References
 *          74HC595 datasheet
 *
 * ASSEMBLY 
 *      connections 
 *      
 *      Arduino Uno 74hc595
 *      ----------  ----------
 * 
 *      D8  select  13 OE
 *      D9  select  n/c
 *      D10 latch   12 ST_CP
 *      D11 MOSI    14 DS
 *      D12 MISO    n/c
 *      R13 SCLK    11 SH_CP
 *                           9  Q7S can float
 *                           10 MR -> Vcc
 */

// D8 Pin connected to enable pin (OE) of 74HC595
const int enablePin = 8;
// D10 Pin connected to latch pin (ST_CP) of 74HC595
const int latchPin = 10;
// D11 Pin connected to DataIn (DS) of 74HC595
const int dataPin = 11;
// D13 Pin connected to clock pin (SH_CP) of 74HC595
const int clockPin = 13;


void setup()
{
    //set pins to output because they are addressed in the main loop
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    Serial.begin(115200);
    Serial.println("reset");
}

void loop()
{
    Serial.println("loop");
    for (byte k = 0; k < 256; k++) {
        write_shift_regs(k);
        Serial.print("\r\nPin States:");
        Serial.print("0x");
        Serial.print(k, 16);
        Serial.print(", or 0b");
        Serial.print(k, 2);
        delay(500);
    }
}

// This method sends bits to the shift register:

void write_shift_regs(byte ledsState)
{
    digitalWrite(clockPin, LOW);
    digitalWrite(enablePin, HIGH);
    digitalWrite(latchPin, LOW);

    // shift the bits out:
    shiftOut(dataPin, clockPin, MSBFIRST, ledsState);
    delay(20);

    // latch and turn on the output so the LEDs can light up:
    digitalWrite(latchPin, HIGH);
    digitalWrite(enablePin, LOW);
}
