/**
 * @file        max7219_7segment_date_time_temp.c
 *
 * @brief       view DATE/TIME/TEMPERATURE using an 8-digit 7-Segment display
 *
 * @history     Feb 6th, 2017
 *
 * @author      Allan Schwartz <allans@CodeValue.net>
 *              Michael Diamond <m.diamond.il@gmail.com>
 *
 * @notes       The following Arduino Pins are attached:
 *
 *              name    Arduino   7 Segment display
 *              -----   ----      -----
 *                      +5V       VCC
 *                      GND       GND
 *              DIN     D4        DIN
 *              CS      D5        CS
 *              CLK     D6        CLK
 *
 *              name    Arduino   RTC module
 *              -----   ----      -----
 *                      nc        32K
 *                      nc        SQW
 *              SCL     SCL/A5    SCL
 *              SDA     SDA/A4    SDA
 *                      3v3       VCC
 *                      GND       GND
 */

#include <RTClib.h>

// enumerate the pins attached to MAX7219 (and also see notes above)
enum {  MAX7219_DIN = 4,
        MAX7219_CS = 5,
        MAX7219_CLK = 6,
     };

// enumerate the MAX7219 registers
// See MAX7219 Datasheet, Table 2, page 7
enum {  MAX7219_REG_DECODE    = 0x09,
        MAX7219_REG_INTENSITY = 0x0A,
        MAX7219_REG_SCANLIMIT = 0x0B,
        MAX7219_REG_SHUTDOWN  = 0x0C,
        MAX7219_REG_DISPTEST  = 0x0F
     };

const byte DP = 0b10000000;
const byte C  = 0b01001110;
const byte F  = 0b01000111;


// create an instance of the DS3231 called 'rtc',
// and specify the I2C hardware interface PINS
RTC_DS3231 rtc;


// ... setup code here, to run once
void setup()
{
    // initialize the serial port:
    Serial.begin(115200);           // initialize serial communication
    Serial.println("\nmax7219_7segment_date_time_temp\n");
    bool rc = rtc.begin();
    Serial.println("rtc.begin() returns " + String(rc));

    // define type of pin
    pinMode(MAX7219_DIN, OUTPUT);   // serial data-in
    pinMode(MAX7219_CS, OUTPUT);    // chip-select, active low
    pinMode(MAX7219_CLK, OUTPUT);   // serial clock
    digitalWrite(MAX7219_CS, HIGH);

    resetDisplay();                 // reset the MAX2719 display

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, lets set the time!");
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}


// ... loop forever...
void loop()
{
    // ... display Date, dd.mm.yyyy
    DateTime now = rtc.now();
    displayDate(now.year(), now.month(), now.day());
    delay(6000);


    // ... display Time, hh-mm-ss
    for ( int i = 0; i < 10; i++ ) {
        DateTime now = rtc.now();
        displayTime(now.hour(), now.minute(), now.second());
        delay(1000);
    }

    // ... display Temperature in Celsius, xx.xx C
    // read the temperature, as a float, from the RTC
    float t = rtc.temperature();
    displayTemp(t, 2, C);            // display on the 7-segment
    delay(3000);

    // ... display Temperature in Fahrenheit, xx.xx F
    t = t * 1.8 + 32.0;             // convert the value to Fahrenheit
    displayTemp(t, 1, F);           // display on the 7-segment
    delay(3000);
}


// ... write a value into a max7219 register
// See MAX7219 Datasheet, Table 1, page 6
void set_register(byte reg, byte value)
{
    digitalWrite(MAX7219_CS, LOW);
    shiftOut(MAX7219_DIN, MAX7219_CLK, MSBFIRST, reg);
    shiftOut(MAX7219_DIN, MAX7219_CLK, MSBFIRST, value);
    digitalWrite(MAX7219_CS, HIGH);
}


// ... reset the max7219 chip
void resetDisplay()
{
    set_register(MAX7219_REG_SHUTDOWN, OFF);      // turn off display
    set_register(MAX7219_REG_DISPTEST, OFF);      // turn off test mode
    set_register(MAX7219_REG_INTENSITY, 0x0D);    // Set display intensity
}


// ... display the DATE on the 7-segment display
void displayDate(int yyyy, byte mon, byte dd)
{
    set_register(MAX7219_REG_SHUTDOWN, OFF);      // turn off display
    set_register(MAX7219_REG_SCANLIMIT, 7);       // scan limit to 8 digits
    set_register(MAX7219_REG_DECODE, 0b11111111); // decode all 8 digits

    set_register(8, '2');
    set_register(7, '0');
    set_register(6, (yyyy / 10) % 10);
    set_register(5, (yyyy % 10) | DP);      // plus decimal point
    set_register(4, mon / 10 );
    set_register(3, mon % 10 | DP);
    set_register(2, dd / 10 );
    set_register(1, dd % 10 );

    set_register(MAX7219_REG_SHUTDOWN, 1);       // Turn on the display
}


// ... display the TIME on the 7-segment display
void displayTime(byte hh, byte mm, byte ss)
{
    set_register(MAX7219_REG_SHUTDOWN, OFF);      // turn off display
    set_register(MAX7219_REG_SCANLIMIT, 7);       // scan limit to 8 digits
    set_register(MAX7219_REG_DECODE, 0b11111111); // decode all 8 digits

    set_register(8, hh / 10);
    set_register(7, hh % 10);
    set_register(6, '/');
    set_register(5, mm / 10);
    set_register(4, mm % 10);
    set_register(3, '/');
    set_register(2, ss / 10);
    set_register(1, ss % 10);

    set_register(MAX7219_REG_SHUTDOWN, 1);       // Turn on the display
}


// ... display the TEMP on the 7-segment display
void displayTemp(float tempValue, int precision, char C_or_F )
{
    set_register(MAX7219_REG_SHUTDOWN, OFF);      // turn off display
    set_register(MAX7219_REG_SCANLIMIT, 5);       // scan limit to 6 digits
    set_register(MAX7219_REG_DECODE, 0b00111100); // decode 4 digits

    String tempString = String(tempValue, precision); // format that value
    set_register(1, C_or_F);
    set_register(2, 0);                           // blank
    set_register(3, tempString.charAt(4));
    set_register(4, tempString.charAt(3));
    set_register(5, tempString.charAt(1) | DP);   // plus decimal point
    set_register(6, tempString.charAt(0));
    set_register(MAX7219_REG_SHUTDOWN, 1);        // Turn On display
}
