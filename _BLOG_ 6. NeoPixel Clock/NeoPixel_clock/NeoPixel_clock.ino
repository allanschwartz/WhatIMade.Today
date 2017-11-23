/**
 * \file NeoPixel_clock
 *
 *      Time display program for an ESP8266 and a pair of NeoPixel rings
 *
 * \author Allan M Schwartz, CodeValue
 *      allans@codevalue.net
 *      November 2017
 *
 * \References
 *      1)  AdaFruit's NeoPixel "Uberguide"
 *          https://learn.adafruit.com/adafruit-neopixel-uberguide/the-magic-of-neopixels
 *
 *      2)  NIST explaination of services:
 *          https://www.nist.gov/pml/time-and-frequency-division/services/internet-time-service-its
 *
 *      3)  earlier work by Joshua Brooks
 *          https://www.instructables.com/id/Desktop-NeoPixel-Clock/
 *
 *      4)  See my blog about this Arduino application at
 *          http://www.whatimade.today/desktop-neopixel-clock/
 */

#include "Adafruit_NeoPixel.h"
#include <ESP8266WiFi.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Time.h>


/*---------------------------------------------------------------------
/   Constants and Global Variables
/----------------------------------------------------------------------*/
// WIFI
const char* WIFI_SSID = "ssid";         // YOUR WiFi SSID here
const char* WIFI_PWD = "passsword";     // YOUR WiFi password here

// NeoPixel Settings
const int NEOPIXEL_DATA_PIN = D6;       // WeMos digital pin D6
const int NEO_NUM_PIXELS = 12 + 24;     // two rings
const int BRIGHTNESS = 96;              // experiment with this
// the minute ring is daisy-chained to the hour ring, with indexes of 12-36
const int MINUTE_PIXEL_OFFSET = 12;

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(
                              NEO_NUM_PIXELS,
                              NEOPIXEL_DATA_PIN,
                              NEO_GRB + NEO_KHZ800);

// TCP connection
const uint16_t TCP = 13;
const char *NistHostName = "time.nist.gov"; // our NTP server url
IPAddress NistServerAddr(0, 0, 0, 0);       // set via DNA lookup, maybe to 129.6.15.28

// global Time synchonization variables
time_t      clocktime_s = 0;        // seconds on the clock (since midnight)
clock_t     clockset_at_ms = 0;     // milliseconds this program has been running,
// ... at the moment of the clock synchronization event
bool        clock_sync_ok = false;

// global timekeeping variables
int clock_hour, clock_minute, clock_second, clock_milli;

// define the local timezone offset from GMT
const time_t  TIMEZONEOFFSET = (2 * 60 * 60);     // Israel is GMT + 2 hours

/*
 ---------------------------------------------------------------------
 */


/**
 *      getTime_from_NIST -- retrieve the current time from nist.time.gov, by
 *          opening a TCP/IP (port 13) connection and reading the packet
 */
time_t  getTime_from_NIST(void)
{
    // Check WiFi connection status
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("lost WiFi connection");
        return -1;
    }

    Serial.printf("Connecting to: %s: %d\r\n", NistHostName, TCP);
    WiFiClient client;
    if (!client.connect(NistServerAddr, TCP)) {
        Serial.println("Connection failed");
        return -1;
    }

    // send empty request packet through TCP connection.
    client.print("\n");

    // Read the reply packet from server and print this to Serial
    String line = client.readStringUntil('\r');

    // debug printf ... print whatever we received
    Serial.print(line);

    client.stop();   // Close connection
    return parse_NTP_response(line);
}


/**
 *      parseNTPresponse -- parse the packet from nist.time.gov,
 *          returning the number of seconds since midnight, GMT
 */
time_t  parse_NTP_response(String line)
{
    if (line && line.length() > 24 ) {

        // now parse this string.  it looks like:
        //  jjjjj mm/dd/yyyy hh:mm:ss ....

        char buf[50];
        line.toCharArray(buf, sizeof buf);

        int blanks[5] = { 0, };  // index of the spaces within buf
        blanks[2] = 0;
        for ( int i = 0, j = 0; (i < strlen(buf)) && (j < 5); i++ )  {
            char c = buf[i];
            // find the first couple of spaces, delimiting the first couple of tokens
            if ( c == ' ' || isspace(c)) {
                blanks[j++] = i;
            }
        }

        // the third token looks like "hh:mm:ss"
        if (blanks[2]) {
            char *tok = &buf[blanks[2] + 1];

            int nist_hour = atoi(&tok[0]);
            int nist_min = atoi(&tok[3]);
            int nist_sec = atoi(&tok[6]);

            // debug printf ... print parsed time on the console
            Serial.println(" hour " + String(nist_hour) +
                           " min " + String(nist_min) +
                           " sec " + String(nist_sec));

            return (nist_hour * 60L * 60L) + (nist_min * 60L) + nist_sec;
        }
    }
    return 0;
}


/**
 *      sychronizeTime -- called to open a connection to the time server
 *          and set global variables for the current time
 */
bool sychronizeTime(void)
{
    time_t  nist_time_s = getTime_from_NIST();
    if (nist_time_s < 0) {
        Serial.println("time syncronization failure");
        clock_sync_ok = false;
    }
    else if (nist_time_s == 0) {
        Serial.println("NTP packet parsing failure");
        clock_sync_ok = false;
    }
    else {
        // otherwise, success!
        // remember now_s, and the current millisecond clock
        clocktime_s = nist_time_s + TIMEZONEOFFSET;
        clockset_at_ms = millis();
        Serial.println("time syncronized!");
        clock_sync_ok = true;
    }
    return clock_sync_ok;
}


/**
 *      setPixelColor -- set a single pixel in the NeoPixel ring to a specific RGB value
 */
void setPixelColor(Adafruit_NeoPixel & strip, int index, byte red, byte green, byte blue, int brightness = 256)
{
    strip.setPixelColor(index,
                        (((int)red) * brightness) >> 8,
                        (((int)green) * brightness) >> 8,
                        (((int)blue) * brightness) >> 8
                       );
}


/**
 *      drawSecondHand -- update the outer NeoPixel ring, with a pixel representing seconds
 */
void drawSecondHand(int sec)
{
    int pix_index = 24 * sec / 60;          // we only have a 24-pixel ring

    static int blinkstate = 0;

    if (++blinkstate % 2 == 1)
        setPixelColor(strip, pix_index + MINUTE_PIXEL_OFFSET,
                      0, 0, BRIGHTNESS / 2);
}


/**
 *      drawMinuteHand -- update the outer NeoPixel ring, with a pair of pixels representing minutes
 */
void drawMinuteHand(int minute)
{
    int pix_index = 24 * minute / 60;       // we only have a 24-pixel ring
    int pix_after = 24 * minute / 60 + 1;

    switch (minute % 5) {
        case 0:
            setPixelColor(strip, pix_index + MINUTE_PIXEL_OFFSET,
                          255, 0, 0, BRIGHTNESS);
            break;
        case 1:
            setPixelColor(strip, pix_index + MINUTE_PIXEL_OFFSET,
                          255, 0, 0, BRIGHTNESS);
            setPixelColor(strip, pix_after + MINUTE_PIXEL_OFFSET,
                          255, 64, 0, BRIGHTNESS / 2);
            break;
        case 2:
            setPixelColor(strip, pix_index + MINUTE_PIXEL_OFFSET,
                          255, 64, 32, BRIGHTNESS / 2);
            setPixelColor(strip, pix_after + MINUTE_PIXEL_OFFSET,
                          255, 64, 0, BRIGHTNESS);
            break;
        case 3:
            setPixelColor(strip, pix_index + MINUTE_PIXEL_OFFSET,
                          255, 64, 0, BRIGHTNESS);
            setPixelColor(strip, pix_after + MINUTE_PIXEL_OFFSET,
                          255, 64, 32, BRIGHTNESS / 2);
            break;
        case 4:
            setPixelColor(strip, pix_index + MINUTE_PIXEL_OFFSET,
                          255, 64, 0, BRIGHTNESS / 2);
            setPixelColor(strip, pix_after + MINUTE_PIXEL_OFFSET,
                          255, 0, 0, BRIGHTNESS);
            break;
    }
}


/**
 *      drawHourHand -- update the inner NeoPixel ring, with a pixel representing the hour
 */
void drawHourHand(int Hour)
{
    Hour %= 12;
    if (Hour == 0 ) {
        // special case for midnight and noon
        setPixelColor(strip, Hour, 0, 255, 0, BRIGHTNESS / 2);
        return;
    }
    // for hours 1 through 11
    for (int i = 1; i <= Hour; i++) {
        setPixelColor(strip, i, 0, 255, 0, BRIGHTNESS / 2);
    }
}


/**
 *      refreshClock -- update both NeoPixel rings, with the current time
 */
void refreshClock(int Hour, int minute, int second)
{
    strip.clear();
    drawHourHand(Hour);
    drawSecondHand(second);     // second hand is "under" minute hand
    drawMinuteHand(minute);
    strip.show();
}


/**
 *      setup -- initialization of time display program,
 *          including initializing the NeoPixels,
 *          initializing the serial port,
 *          connecting to WiFi,
 */
void setup()
{
    strip.begin();
    strip.clear();
    strip.show(); // Initialize all pixels to 'off'

    Serial.begin(115200);
    Serial.println();
    Serial.println();

    WiFi.begin(WIFI_SSID, WIFI_PWD);

    // try for 2 minutes to connect
    for (int trys = 0; trys < 2 * 60; trys++) {
        if  (WiFi.status() == WL_CONNECTED) {
            Serial.print("\r\nWifi connected: ");
            Serial.println(WiFi.localIP());
            break;
        }
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");

    int rc = WiFi.hostByName(NistHostName, NistServerAddr);
    if (rc < 0)
        Serial.println("GetHostByName returns : " + String(rc));
    else {
        Serial.print("GetHostByName resolved " + String(NistHostName) + ": ");
        Serial.println(NistServerAddr);
    }
}


/**
 *      timeKeeping -- called once per loop to manage to time variables
 */
void timeKeeping()
{
    clock_t  now_ms = millis();
    clock_t  delta_ms = now_ms - clockset_at_ms;       // how many milliseconds since we synced

    time_t   secs_since_midnight = clocktime_s + delta_ms / 1000;

    clock_hour = (secs_since_midnight / 3600L) % 24L;
    clock_minute = (secs_since_midnight / 60L) % 60L;
    clock_second = secs_since_midnight % 60L;
    clock_milli = now_ms % 1000L;

    if (delta_ms > 3600 * 1000) {
        // Once per hour sychronize the time to nist.time.gov
        sychronizeTime();
    }
    if (clock_second == 20 && !clock_sync_ok) {
        // if we initially failed to connect
        // keep trying once per minute to sychronize the time to nist.time.gov
        sychronizeTime();
    }
}


/**
 *      loop -- time display program for a ESP8266 and a pair of NeoPixel rings
 */

// we have a time budget of 1000ms per loop interation
const clock_t  TIMEBUDGET_ms = 1000;

void loop()
{
    clock_t  t0_ms = millis();

    timeKeeping();
    refreshClock(clock_hour, clock_minute, clock_second);

    // debug printf ... print time on the console
    char timestring[16];
    sprintf(timestring, "%02d:%02d:%02d.%03d", clock_hour, clock_minute, clock_second, clock_milli);
    Serial.println(timestring);

    // end of loop interation, how long to delay?
    clock_t  time_consumed_ms = millis() - t0_ms;
    if (time_consumed_ms < TIMEBUDGET_ms)
        delay(TIMEBUDGET_ms - time_consumed_ms);
}

