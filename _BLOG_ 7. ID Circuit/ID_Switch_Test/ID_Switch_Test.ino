


const int BUTTONS = A0;

void setup()
{
    Serial.begin(115200);
    Serial.println(__FILE__);

#if V1_1
    readIDSwitch();
#endif

#if V1_4 || V1_5
    for (int i = 0; i < 10; i++) {
        delay(1000);
        readIDSwitch();
    }
#endif
}

#if V1_1
byte readIDSwitch()
{
    byte id;

    pinMode(ID_BIT8, INPUT_PULLUP);
    byte id_bit8 = !digitalRead(ID_BIT8);
    //Serial.println("ID_BIT8 " + String(id_bit8 ? "ON" : "OFF") );

    pinMode(ID_BIT4, INPUT_PULLUP);
    byte id_bit4 = !digitalRead(ID_BIT4);
    //Serial.println("ID_BIT4 " + String(id_bit4 ? "ON" : "OFF") );

    pinMode(ID_BIT2, INPUT_PULLUP);
    byte id_bit2 = !digitalRead(ID_BIT2);
    //Serial.println("ID_BIT2 " + String(id_bit2 ? "ON" : "OFF") );

    pinMode(ID_BIT1, INPUT_PULLUP);
    byte id_bit1 = !digitalRead(ID_BIT1);
    //Serial.println("ID_BIT1 " + String(id_bit1 ? "ON" : "OFF") );

    id = id_bit8 << 3 | id_bit4 << 2 | id_bit2 << 1 | id_bit1;
    Serial.println("id " + String(id) );
    return id;
}
#endif

#if V1_4 || V1_5

byte readIDSwitch()
{
    byte id;
    // this table is derived from measuring voltages from
    // the resistor ladder circuit
    const int id_table[] = {
        0, 108, 208, 280,   /* 0,   1,  2,  3 */
        341, 392, 441, 479, /* 4,   5,  6,  7 */
        512, 542, 571, 594, /* 8,   9, 10, 11 */
        615, 634, 653, 669  /* 12, 13, 14, 15 */
    };
    const int epsilon = 10;     // based on resistor variance; ADC error

    short v = analogRead(BUTTONS);
    Serial.println("buttons " + String(v) );

    // use a lookup table, because the voltages are not linear
    for (int i = 0 ; i < 16; i++ ) {
        if ( (id_table[i] - epsilon < v) &&
                (v < id_table[i] + epsilon) ) {
            id = i;
            Serial.println("id " + String(id) );
            return id;
        }
    }
    return 0;
}

#endif






void loop()
{
    
}

