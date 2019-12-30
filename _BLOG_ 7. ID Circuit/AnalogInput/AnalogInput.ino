/*
  Analog Input
 */
 
const int ledPin = 13;
int previousA0Value = 0;
    
void setup()
{
    pinMode(ledPin, OUTPUT);
    Serial.begin(115200);
}

void loop()
{
    int A0value = analogRead(A0);
    if ( A0value != previousA0Value ) {
        Serial.print("value = "); Serial.println(A0value);
        previousA0Value = A0value;
    }
    digitalWrite(ledPin, HIGH);delay(500);
    digitalWrite(ledPin, LOW); delay(500);
}

