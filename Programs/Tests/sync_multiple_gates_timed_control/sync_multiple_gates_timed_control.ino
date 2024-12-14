const uint8_t NUM_GATES = 3;

const uint8_t closeLimitSwitchPins[NUM_GATES] = {A0, A2, A4};
const uint8_t openLimitSwitchPins[NUM_GATES] = {A1, A3, A5};

const uint8_t IN1Pins[NUM_GATES] = {4, 7, 10};
const uint8_t IN2Pins[NUM_GATES] = {5, 8, 11};
const uint8_t ENAPins[NUM_GATES] = {6, 9, 12};

int motorBaseSpeed = 75;
const unsigned long closeGatesDuration = 3000UL;
const unsigned long openGatesDuration = 10000UL;

unsigned long currentMillis;

unsigned long gatesStateTimer = 0;
enum GatesState { OPENING, OPEN, CLOSING, CLOSED };
GatesState gatesCurrentState = CLOSED;

void setup() {
    Serial.begin(9600);

    for (uint8_t i = 0; i < NUM_GATES; i++) {
        pinMode(closeLimitSwitchPins[i], INPUT_PULLUP);
        pinMode(openLimitSwitchPins[i], INPUT_PULLUP);
        pinMode(IN1Pins[i], OUTPUT);
        pinMode(IN2Pins[i], OUTPUT);
        pinMode(ENAPins[i], OUTPUT);

        stopGates(i);
    }

    gatesStateTimer = millis();
}

void loop() {
    currentMillis = millis();

    bool allCloseLimitSwitchActive = true;
    bool allOpenLimitSwitchActive = true;

    for (uint8_t i = 0; i < NUM_GATES; i++) {
        if (!digitalRead(closeLimitSwitchPins[i])) {
            allCloseLimitSwitchActive = false;
        }

        if (!digitalRead(openLimitSwitchPins[i])) {
            allOpenLimitSwitchActive = false;
        }
    }

    switch (gatesCurrentState) {
    case OPENING:
        if (allOpenLimitSwitchActive) {
            stopAllGates();
            gatesCurrentState = OPEN;
            gatesStateTimer = currentMillis;
        } else {
            openAllGates();
        }
        break;

    case OPEN:
        if (currentMillis - gatesStateTimer >= openGatesDuration) {
            gatesCurrentState = CLOSING;
        }
        break;

    case CLOSING:
        if (allCloseLimitSwitchActive) {
            stopAllGates();
            gatesCurrentState = CLOSED;
            gatesStateTimer = currentMillis;
        } else {
            closeAllGates();
        }
        break;

    case CLOSED:
        if (currentMillis - gatesStateTimer >= closeGatesDuration) {
            gatesCurrentState = OPENING;
        }
        break;
    }
}

void closeAllGates() {
    for (uint8_t i = 0; i < NUM_GATES; i++) {
        digitalWrite(IN1Pins[i], HIGH);
        digitalWrite(IN2Pins[i], LOW);
        analogWrite(ENAPins[i], motorBaseSpeed);
    }
}

void openAllGates() {
    for (uint8_t i = 0; i < NUM_GATES; i++) {
        digitalWrite(IN1Pins[i], LOW);
        digitalWrite(IN2Pins[i], HIGH);
        analogWrite(ENAPins[i], motorBaseSpeed);
    }
}

void stopAllGates() {
    for (uint8_t i = 0; i < NUM_GATES; i++) {
        digitalWrite(IN1Pins[i], LOW);
        digitalWrite(IN2Pins[i], LOW);
    }
}
