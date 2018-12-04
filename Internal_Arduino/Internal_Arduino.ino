#include <Servo.h>

const int unlockButton = 2;
const int lockButton = 3;
const int unlockPosition = 180;
const int lockPosition = 0;
const int buzzerPin = 4;
byte incomingByte;
int passwordCorrect = 0;

int unlockDoor(void);
int lockDoor(void);

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
    }
    //if(0-->9) do ...
    else if(incomingByte == '#'){
      for(int i = 0; i<5; i++){
        if(passInput[i]!=passcode[i]){
          break;
        }
      }
      passwordCorrect = 1;
    }
    else if(incomingByte == '*'){
      for(int x=0; x<5; x++){
        passInput[x]='\0';
      }
      Serial.write("C");
    }
  }

  if(passwordCorrect == 1){
    unlockDoor();
    delay(1000);
    passwordCorrect = 0;
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
