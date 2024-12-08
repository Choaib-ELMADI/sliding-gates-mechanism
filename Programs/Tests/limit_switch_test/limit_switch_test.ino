const uint8_t limitSwitchPin = 2;
uint8_t limitSwitchValue;

void setup() {
    Serial.begin(9600);

    pinMode(limitSwitchPin, INPUT_PULLUP);
}

void loop() {
    limitSwitchValue = digitalRead(limitSwitchPin);

    Serial.print("Click is: ");
    Serial.println(limitSwitchValue);

    // delay(15);
}
