const uint8_t NUM_GATES = 3;

const uint8_t closeLimitSwitchPins[NUM_GATES] = {A0, A2, A4};
const uint8_t openLimitSwitchPins[NUM_GATES] = {A1, A3, A5};

const uint8_t IN1Pins[NUM_GATES] = {4, 7, 10};
const uint8_t IN2Pins[NUM_GATES] = {5, 8, 11};
const uint8_t ENAPins[NUM_GATES] = {6, 9, 12};

uint8_t closeLimitSwitchActive, openLimitSwitchActive;

int motorBaseSpeed = 75;
const unsigned long closeGatesDuration[NUM_GATES] = {3000UL, 3000UL, 3000UL};
const unsigned long openGatesDuration[NUM_GATES] = {10000UL, 10000UL, 10000UL};

unsigned long gatesStateTimers[NUM_GATES] = {0};
enum GatesState { OPENING, OPEN, CLOSING, CLOSED };
GatesState gatesCurrentStates[NUM_GATES] = {CLOSED, CLOSED, CLOSED};

unsigned long currentMillis;

void setup() {
    Serial.begin(9600);

    for (uint8_t i = 0; i < NUM_GATES; i++) {
        pinMode(closeLimitSwitchPins[i], INPUT_PULLUP);
        pinMode(openLimitSwitchPins[i], INPUT_PULLUP);
        pinMode(IN1Pins[i], OUTPUT);
        pinMode(IN2Pins[i], OUTPUT);
        pinMode(ENAPins[i], OUTPUT);

        stopGates(i);
        gatesStateTimers[i] = millis();
    }
}

void loop() {
    currentMillis = millis();

    for (uint8_t i = 0; i < NUM_GATES; i++) {
        closeLimitSwitchActive = digitalRead(closeLimitSwitchPins[i]);
        openLimitSwitchActive = digitalRead(openLimitSwitchPins[i]);

        switch (gatesCurrentStates[i]) {
        case OPENING:
            if (openLimitSwitchActive) {
                stopGates(i);
                gatesCurrentStates[i] = OPEN;
                gatesStateTimers[i] = currentMillis;
            } else {
                openGates(i);
            }
            break;

        case OPEN:
            if (currentMillis - gatesStateTimers[i] >= openGatesDuration[i]) {
                gatesCurrentStates[i] = CLOSING;
            }
            break;

        case CLOSING:
            if (closeLimitSwitchActive) {
                stopGates(i);
                gatesCurrentStates[i] = CLOSED;
                gatesStateTimers[i] = currentMillis;
            } else {
                closeGates(i);
            }
            break;

        case CLOSED:
            if (currentMillis - gatesStateTimers[i] >= closeGatesDuration[i]) {
                gatesCurrentStates[i] = OPENING;
            }
            break;
        }
    }
}

void closeGates(uint8_t gate) {
    digitalWrite(IN1Pins[gate], HIGH);
    digitalWrite(IN2Pins[gate], LOW);
    analogWrite(ENAPins[gate], motorBaseSpeed);
}

void openGates(uint8_t gate) {
    digitalWrite(IN1Pins[gate], LOW);
    digitalWrite(IN2Pins[gate], HIGH);
    analogWrite(ENAPins[gate], motorBaseSpeed);
}

void stopGates(uint8_t gate) {
    digitalWrite(IN1Pins[gate], LOW);
    digitalWrite(IN2Pins[gate], LOW);
}
