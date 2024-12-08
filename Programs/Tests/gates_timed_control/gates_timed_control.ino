const uint8_t closeLimitSwitchPin = A0;
const uint8_t openLimitSwitchPin = A1;

uint8_t closeLimitSwitchActive, openLimitSwitchActive;

const uint8_t IN1 = 4;
const uint8_t IN2 = 5;
const uint8_t ENA = 6;

int motorBaseSpeed = 100;

const unsigned long closeGatesDuration = 3000UL;
const unsigned long openGatesDuration = 10000UL;

unsigned long gatesStateTimer = 0;
unsigned long currentMillis = 0;

enum GatesState { OPENING, OPEN, CLOSING, CLOSED };
GatesState gatesCurrentState = CLOSED;

void closeGates();
void openGates();
void stopGates();

void setup() {
    Serial.begin(9600);

    pinMode(closeLimitSwitchPin, INPUT_PULLUP);
    pinMode(openLimitSwitchPin, INPUT_PULLUP);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(ENA, OUTPUT);

    stopGates();
    gatesStateTimer = millis();
}

void loop() {
    currentMillis = millis();

    closeLimitSwitchActive = digitalRead(closeLimitSwitchPin);
    openLimitSwitchActive = digitalRead(openLimitSwitchPin);

    switch (gatesCurrentState) {
    case OPENING:
        if (openLimitSwitchActive) {
            stopGates();
            gatesCurrentState = OPEN;
            gatesStateTimer = currentMillis;
        } else {
            openGates();
        }
        break;

    case OPEN:
        if (currentMillis - gatesStateTimer >= openGatesDuration) {
            gatesCurrentState = CLOSING;
        }
        break;

    case CLOSING:
        if (closeLimitSwitchActive) {
            stopGates();
            gatesCurrentState = CLOSED;
            gatesStateTimer = currentMillis;
        } else {
            closeGates();
        }
        break;

    case CLOSED:
        if (currentMillis - gatesStateTimer >= closeGatesDuration) {
            gatesCurrentState = OPENING;
        }
        break;
    }
}

void closeGates() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, motorBaseSpeed);
}

void openGates() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, motorBaseSpeed);
}

void stopGates() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
}
