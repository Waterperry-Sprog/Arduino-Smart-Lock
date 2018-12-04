#include <Servo.h>

const int unlockButton = 2;
const int lockButton = 3;
const int unlockPosition = 180;
const int lockPosition = 0;
const int buzzerPin = 4;
byte incomingByte;

int unlockDoor(void);
int lockDoor(void);

const int passLength = 4;
const char passcode[passLength] = {'5', '2', '3', '8'};
char passInput[passLength] {'\0', '\0', '\0', '\0'};
int freeCell = -1;

volatile long interruptTime;
volatile long lastInterruptTime;
Servo lockServo;


void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(lockButton), unlockDoor, FALLING);
  attachInterrupt(digitalPinToInterrupt(unlockButton), lockDoor, FALLING);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  lockServo.attach(5);
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte == 'R'){
      tone(buzzerPin,600);
      delay(500);
      tone(buzzerPin,450,500);
    } else if (incomingByte >= 48 && incomingByte <= 57) {
      int j = 0;
      freeCell = -1;
      for (int i = 0; i < passLength; i++) {
        if (passInput[i] == '\0') {
          freeCell = i;
          break;
        }
      }
      if (freeCell == -1) {
        // cells full
      } else {
        passInput[freeCell] = incomingByte;
        for (int i = 0; i < passLength; i++) {
          if (passInput[i] != '\0') {
          }
        }
      }
    } else if (incomingByte == '*') {
      passInput[0] = '\0'; passInput[1] = '\0';
      passInput[2] = '\0'; passInput[3] = '\0';
    } else if (incomingByte == '#') {
      if (strcmp(passcode, passInput)) {
        unlockDoor(); // unlock door when code correct
      }
    }
  }

}

int unlockDoor(void) {
  interruptTime = millis();
  if(lastInterruptTime < interruptTime - 400){
    lockServo.write(unlockPosition);
    Serial.write("H");
    digitalWrite(LED_BUILTIN,HIGH); 
    lastInterruptTime = interruptTime;
  }
}

int lockDoor(void) {
  interruptTime = millis();
  if(lastInterruptTime < interruptTime - 400){
    lockServo.write(lockPosition);
    Serial.write("L");
    digitalWrite(LED_BUILTIN,LOW);
    lastInterruptTime = interruptTime;
  }
}
