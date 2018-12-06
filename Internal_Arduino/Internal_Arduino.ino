#include <Servo.h>

// standard I/O pin assignment
const int unlockButton = 2;
const int lockButton = 3;
const int buzzerPin = 4;
// servo position values
const int unlockPosition = 180;
const int lockPosition = 0;
const int servoPin = 5;
Servo lockServo;

// declare functions
int unlockDoor(void);
int lockDoor(void);

// variables required for pin system
const int pinLength = 4;
const char pin[pinLength] = {'5', '2', '3', '8'};
char pinInput[pinLength] {'\0', '\0', '\0', '\0'};
int freeCell = -1;

// variables required in order not to repeat input too fast
volatile long interruptTime;
volatile long lastInterruptTime;


void setup() {
  // attach lock and unlock buttons to interrupt pins
  attachInterrupt(digitalPinToInterrupt(lockButton), unlockDoor, FALLING);
  attachInterrupt(digitalPinToInterrupt(unlockButton), lockDoor, FALLING);
  // assign the rest of the pins
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  lockServo.attach(servoPin);
  // initialize the serial connection
  Serial.begin(9600);
}

void loop() {
  // if there is data in the serial connection, use it
  if (Serial.available() > 0) {
    byte incomingByte = Serial.read();
    if (incomingByte == 'R') {
      // on ring press, play a tone
      tone(buzzerPin, 600);
      delay(500);
      tone(buzzerPin, 450, 500);
    } else if (incomingByte >= 48 && incomingByte <= 57) {
      // if a digit was pressed on the keypad
      // add it to the pinInput array if possible
      // '\0' represents null
      int j = 0;
      freeCell = -1;
      for (int i = 0; i < pinLength; i++) {
        if (pinInput[i] == '\0') {
          freeCell = i;
          break;
        }
      }
      if (freeCell == -1) {
        // cells full
      } else {
        pinInput[freeCell] = incomingByte;
        for (int i = 0; i < pinLength; i++) {
          if (pinInput[i] != '\0') {
          }
        }
      }
    } else if (incomingByte == '*') {
      // if '*' is pressed, reset pinInput
      pinInput[0] = '\0'; pinInput[1] = '\0';
      pinInput[2] = '\0'; pinInput[3] = '\0';
    } else if (incomingByte == '#') {
      // if the pin matches, unlock the door
      if (strcmp(pincode, pinInput)) {
        unlockDoor();
      }
    }
  }

}

int unlockDoor(void) {
  // on call, put servo in unlock position and send 'U' to external arduino
  interruptTime = millis();
  if (lastInterruptTime < interruptTime - 400) {
    lockServo.write(unlockPosition);
    Serial.write("U");
    digitalWrite(LED_BUILTIN, HIGH);
    lastInterruptTime = interruptTime;
  }
}

int lockDoor(void) {
  // on call, put servo in lock position and send 'L' to external arduino
  interruptTime = millis();
  if (lastInterruptTime < interruptTime - 400) {
    lockServo.write(lockPosition);
    Serial.write("L");
    digitalWrite(LED_BUILTIN, LOW);
    lastInterruptTime = interruptTime;
  }
}
