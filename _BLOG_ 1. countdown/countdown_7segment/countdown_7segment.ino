/**
 * @file        countdown_7segment.c
 *
 * @brief       exercise to create a countdown timer from a 2-digit 7-Segment display
 *
 * @history     exercise proposed Oct 28th, 2015
 *
 * @author      Allan Schwartz, allans@CodeValue.net
 *
 * @notes       The following Arduino Pins are attached:
 *
 *              name    Arduino   7 Segment display
 *              -----   ----      -----
 *              D1-a    D2        11
 *              D1-b    D3        10
 *              D1-c    D4        8
 *              D1-d    D5        6
 *              D1-e    D6        5
 *              D1-f    D7        12
 *              D1-g    D8        7
 *              anode   +5        13
 *
 *              D10-a   D10       16
 *              D10-b   D11       15
 *              D10-c   D12       3
 *              D10-d   D13       2
 *              D10-e   D14/A0    1
 *              D10-f   D15/A1    18
 *              D10-g   D16/A2    17
 *              anode   +5        14
 *
 *              SW1     D17/A3
 */

// the  1's digit pins a-g are digital pins 2-8
const byte DIGIT1_PIN_A = 2;
// the 10's digit pins a-g are digital pins 10-17        
const byte DIGIT10_PIN_A = 10;      

const byte numbers[10][7] = {       
                            // 0: segment lit, 1: segment unlit
  // a  b  c  d  e  f  g
    {0, 0, 0, 0, 0, 0, 1},  // 0
    {1, 0, 0, 1, 1, 1, 1},  // 1
    {0, 0, 1, 0, 0, 1, 0},  // 2
    {0, 0, 0, 0, 1, 1, 0},  // 3
    {1, 0, 0, 1, 1, 0, 0},  // 4
    {0, 1, 0, 0, 1, 0, 0},  // 5
    {0, 1, 0, 0, 0, 0, 0},  // 6
    {0, 0, 0, 1, 1, 1, 1},  // 7
    {0, 0, 0, 0, 0, 0, 0},  // 8
    {0, 0, 0, 1, 1, 0, 0},  // 9
};


void setup(void) {
    set_pinMode(DIGIT1_PIN_A);
    set_pinMode(DIGIT10_PIN_A);
}

void set_pinMode(byte pin) {
    for ( byte i = 0; i < 7; i++, pin++ )
        pinMode(pin, OUTPUT);
}

void sevenSegmentClear(byte pin) {
    for ( byte i = 0; i < 7; i++, pin++ )
        digitalWrite(pin, 1);
}

// ... display the digit {0..9} in either the ones DIGIT or the tens DIGIT
void sevenSegmentDigit(byte digit, byte pin) {
    for ( byte i = 0; i < 7; i++, pin++ )
        digitalWrite(pin, numbers[digit][i]);
}

// ... display the two-digit number {00..99} in the two 7-segment displays
void sevenSegment_nn(int nn) {
    sevenSegmentDigit((nn / 10) % 10, DIGIT10_PIN_A);
    sevenSegmentDigit(nn % 10, DIGIT1_PIN_A);
}

void loop(void) {

    // ... countdown for 60 seconds
    for ( int seconds = 60; seconds >= 0; seconds-- ) {
        sevenSegment_nn(seconds);
        delay(1000);
    }

    // ... flash the display for a few more seconds
    for ( int seconds = 4; seconds >= 0; seconds-- ) {
        sevenSegmentClear(DIGIT10_PIN_A);
        sevenSegmentClear(DIGIT1_PIN_A);
        delay(500);
        sevenSegment_nn(00);
        delay(500);
    }
    delay(1000);
}

