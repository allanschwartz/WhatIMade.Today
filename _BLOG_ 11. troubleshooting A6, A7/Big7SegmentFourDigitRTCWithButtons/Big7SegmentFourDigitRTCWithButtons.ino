/**
 * @file        FourDigitDecoderRTCWithButtons.c
 *
 * @brief       View DATE and TIME using 4 BIG 7-segment displays
 *              Change hours and minutes with MODE and NEXT buttons
 *
 * @history     Dec 21, 2019
 *
 * @author      Allan Schwartz <allanschwartz@sbcglobal.net>
 *              Michael Diamond <m.diamond.il@gmail.com>
 *
 * @notes       The following Arduino Pins are attached:
 *
 *              name           Arduino          7 Segment decoder
 *              -----          ----             -----
 *              one's place:   A0, A1, A2, A3   ==> decoder A0-A3
 *              ten's place:   D3, D2, D13, D12 ==> decoder A0-A3
 *              100's place:   D4, D5, D6, D7   ==> decoder A0-A3
 *              1000's place:  D8, D9, D10, D11 ==> decoder A0-A3
 *
 *              name    Arduino   RTC module
 *              -----   ----      -----
 *                      nc        32K
 *                      nc        SQW
 *              SCL     SCL/A5    SCL
 *              SDA     SDA/A4    SDA
 *                      3v3       VCC
 *                      GND       GND
 *                      
 *              name    Arduino   buttons
 *              -----   ----      -----
 *              MODE    A6        mode button
 *              NEXT    A7        next button
 */

#include <RTClib.h>

// this is an enumeration of the 4 different 7-segment decoders
enum { OnesDigit = 0, TensDigit = 1, HundredsDigit = 2, ThousandsDigit = 3 };

// this is the translation of segment decoder signals into pin number
enum { SignalA0 = 0, SignalA1 = 1, SignalA2 = 2, SignalA3 = 3 };

// this is an enumeration of the "Modes" of the program
enum { SecondMode, TimeMode, /* ChangeYear, ChangeMonth, ChangeDay,*/ ChangeHour, ChangeMin };
// the following line of code, defines the mode the program starts out in
int DisplayMode = SecondMode;

// a table of which Arduino IO pins are connected to which decoder signals
const byte PinTable[5][4] = {
    // A0, A1, A2, A3     // columns correspond to signals A0-A3 on the decoder chip
                          // rows correspond to the 4 different 7-segment display assemblies

    A0, A1, A2, A3,     // digital IO pins A0-A3    ==> one's place decoder A0-A3
    3,  2, 13, 12,      // digital pins D3, D2, D13, D12 ==> ten's place decoder A0-A3
    4,  5,  6,  7,      // digital IO pins  4-7     ==> 100's place decoder A0-A3
    8,  9, 10, 11,      // digital IO pins  8-11    ==> 1000's place decoder A0-A3
};

// two pins are read to sense button presses.
const byte MODE_BUTTON_PIN = A6;
const byte NEXT_BUTTON_PIN = A7;
bool isModeButtonPressed = false;
bool isNextButtonPressed = false;

// create an instance of the RTC_DS3231 class called 'rtc',
// and use the default constructor, which is the normal I2C hardware interface PINS
RTC_DS3231 rtc;


void setup() {

    // initialize the serial port:
    Serial.begin(115200);           // initialize serial communication
    Serial.println("\FourDigitDecoderRTCWithButtons\n");
    bool rc = rtc.begin();
    Serial.println("rtc.begin() returns " + String(rc));

    // for every Arduino IO pin in the above table, set to OUTPUT mode
    for (int row = 0; row < 5; row++) {
        for (int column = 0; column < 4; column++) {
            pinMode(PinTable[row][column], OUTPUT);
        }
    }

    if (rtc.lostPower()) {
        Serial.println("RTC lost power, lets set the time!");
        // following line sets the RTC to the date & time this sketch was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    // set the pin mode of the pins connected to the MODE and NEXT buttons
    pinMode(MODE_BUTTON_PIN, INPUT);
    pinMode(NEXT_BUTTON_PIN, INPUT);
}


void ModeButtonPress() {
    static long lastButtonPress_ms = 0;

    // debounce logic ... don't allow mode press within 500ms of previous
    if (millis() - lastButtonPress_ms > 500) {
        lastButtonPress_ms = millis();
        if (DisplayMode == ChangeMin)
            DisplayMode = TimeMode;
        else
            DisplayMode++;
    }
}


void NextButtonPress() {
    static long lastButtonPress_ms = 0;

    // debounce logic ... don't allow mode press within 500ms of previous
    if (millis() - lastButtonPress_ms > 500) {
        lastButtonPress_ms = millis();
        DateTime now = rtc.now();
        uint8_t v;
        switch (DisplayMode) {
//            case ChangeYear:
//                rtc.adjust(DateTime((now.year()+1) > 2030 ? 2010 : now.year()+1,
//                                    now.month(),now.day(),
//                                    now.hour(),now.minute(),now.second()));
//                Serial.println("inc year");
//                break;

//            case ChangeMonth:
//                rtc.adjust(DateTime(now.year(),(now.month()+1) > 12 ? 1 : now.month()+1,
//                                    now.day(),
//                                    now.hour(),now.minute(),now.second()));
//                Serial.println("inc month");
//                break;

//            case ChangeDay:
//                rtc.adjust(DateTime(now.year(),now.month(),(now.day()+1) > 30 ? 1 : now.day()+1,
//                                    now.hour(),now.minute(),now.second()));
//                Serial.println("inc day");
//                break;

            case ChangeHour:
                rtc.adjust(DateTime(now.year(), now.month(), now.day(),
                                    (now.hour() + 1) % 24, now.minute(), now.second()));
                Serial.println("inc hour");
                break;

            case ChangeMin:
                rtc.adjust(DateTime(now.year(), now.month(), now.day(),
                                    now.hour(), (now.minute() + 1) % 60, now.second()));
                Serial.println("inc min");
                break;
        }
    }
}


void loop() {
    static int loopCounter = 0;
    static int tick = 0;
    DateTime now = rtc.now();

    // the program has 4 different "modes" of operation
    switch (DisplayMode) {

        case SecondMode:
            dispayDigit( OnesDigit,      (tick / 1) % 10);      // display the One's digit
            dispayDigit( TensDigit,      (tick / 10) % 10);     // display the Ten's digit
            dispayDigit( HundredsDigit,  (tick / 100) % 10);    // display the Hundred's digit
            dispayDigit( ThousandsDigit, (tick / 1000) % 10);   // display the Thousand's digit
            delay_and_poll(1000);                               // Delay of 1 Second
            if (++tick > 9999) {                                // once beyond 9999, we start over
                tick = 0;
            }
            break;

        case TimeMode: {
            uint8_t lsd = now.second() % 10;        // least-significant-digit
            if (lsd < 7 ) {
                dispayDigit( OnesDigit,      (now.minute() / 1)  % 10); // display the One's digit
                dispayDigit( TensDigit,      (now.minute() / 10));      // display the Ten's digit
                dispayDigit( HundredsDigit,  (now.hour() / 1)  % 10);   // display the Hundred's digit
                dispayDigit( ThousandsDigit, (now.hour() / 10) % 10);   // display the Thousand's digit
            }
            else {
                dispayDigit( OnesDigit,      (now.month() % 10));       // display the One's digit
                dispayDigit( TensDigit,      (now.month() / 10));       // display the Ten's digit
                dispayDigit( HundredsDigit,  (now.day() / 1)  % 10);    // display the Hundred's digit
                dispayDigit( ThousandsDigit, (now.day() / 10) % 10);    // display the Thousand's digit
            }
//          else {
//              dispayDigit( OnesDigit,      (now.year() / 1) % 10);    // display the One's digit
//              dispayDigit( TensDigit,      (now.year() / 10) % 10);   // display the Ten's digit
//              dispayDigit( HundredsDigit,  (now.year() / 100) % 10);  // display the Hundred's digit
//              dispayDigit( ThousandsDigit, (now.year() / 1000) % 10); // display the Thousand's digit
//          }
            delay_and_poll(1000);                                       // Delay of 1 Second
            break;
        }

//        case ChangeYear:
//            dispayDigit( OnesDigit,      (now.year()  / 1) % 10);     // display the One's digit
//            dispayDigit( TensDigit,      (now.year()  / 10) % 10);    // display the Ten's digit
//            dispayDigit( HundredsDigit,  (now.year()  / 100) % 10);   // display the Hundred's digit
//            dispayDigit( ThousandsDigit, (now.year()  / 1000) % 10);  // display the Thousand's digit
//            delay_and_poll(1000);                                     // Delay of 1 Second
//            clearDisplay();
//            delay_and_poll(1000);                                     // Delay of 1 Second
//            break;
//
//        case ChangeMonth:
//            clearDisplay();
//            dispayDigit( HundredsDigit,  (now.month() / 1)  % 10);    // display the Hundred's digit
//            dispayDigit( ThousandsDigit, (now.month() / 10) % 10);    // display the Thousand's digit
//            delay_and_poll(1000);                                     // Delay of 1 Second
//            clearDisplay();
//            delay_and_poll(1000);                                     // Delay of 1 Second
//            break;

//        case ChangeDay:
//            clearDisplay();
//            dispayDigit( OnesDigit,      (now.day()   / 1)  % 10);    // display the One's digit
//            dispayDigit( TensDigit,      (now.day()   / 10) % 10);    // display the Ten's digit
//            delay_and_poll(700);                                      // Delay of 1 Second
//            clearDisplay();
//            delay_and_poll(70);                                       // Delay of 1 Second
//            break;

        case ChangeHour:
            clearDisplay();
            dispayDigit( HundredsDigit,  (now.hour() / 1)  % 10);   // display the Hundred's digit
            dispayDigit( ThousandsDigit, (now.hour() / 10) % 10);   // display the Thousand's digit
            delay_and_poll(500);                                    // Delay of .5 Second
            clearDisplay();
            delay_and_poll(500);                                    // Delay of .5 Second
            break;

        case ChangeMin:
            clearDisplay();
            dispayDigit( OnesDigit,      (now.minute() / 1)  % 10); // display the One's digit
            dispayDigit( TensDigit,      (now.minute() / 10) % 10); // display the Ten's digit
            delay_and_poll(500);                                    // Delay of .5 Second
            clearDisplay();
            delay_and_poll(500);                                    // Delay of .5 Second
            break;
    }

    // every ten seconds, print the state of the A6/A7 buttons
    if ((++loopCounter % 10) == 1) {
        char buf[80];
        sprintf(buf, "month %d day %d year %d %02d:%02d:%02d MODE %d NEXT %d\n",
                now.month(), now.day(), now.year(),
                now.hour(), now.minute(), now.second(), isModeButtonPressed, isNextButtonPressed);
        Serial.println(buf);
    }
}


void dispayDigit(uint8_t place, uint8_t value) {
    digitalWrite( PinTable[place][SignalA0], (value & 1) ? HIGH : LOW );
    digitalWrite( PinTable[place][SignalA1], (value & 2) ? HIGH : LOW );
    digitalWrite( PinTable[place][SignalA2], (value & 4) ? HIGH : LOW );
    digitalWrite( PinTable[place][SignalA3], (value & 8) ? HIGH : LOW );
}


void clearDigit(int place) {
    digitalWrite( PinTable[place][SignalA0], HIGH );
    digitalWrite( PinTable[place][SignalA1], HIGH );
    digitalWrite( PinTable[place][SignalA2], HIGH );
    digitalWrite( PinTable[place][SignalA3], HIGH );
}


void clearDisplay() {
    clearDigit( OnesDigit );
    clearDigit( TensDigit );
    clearDigit( HundredsDigit );
    clearDigit( ThousandsDigit );
}


int checkButtons() {
    isModeButtonPressed = analogRead(MODE_BUTTON_PIN) > 100 ? false : true;
    if (isModeButtonPressed) 
        ModeButtonPress();
    isNextButtonPressed = analogRead(NEXT_BUTTON_PIN) > 100 ? false : true;
    if (isNextButtonPressed) 
        NextButtonPress();
}


void delay_and_poll(int n_ms) {
    for (int i = 0; i < n_ms / 10; i++) {
        // we read the buttons every 10 milliseconds -- plenty often enough
        checkButtons();
        delay(10);
    }
}
