int ledPins[] = {};
int unlockButton = 2;
int lockButton = 3;
int unlockDoor(void);
int lockDoor(void);
volatile long interruptTime;
volatile long lastInterruptTime;



void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(lockButton), unlockDoor, FALLING);
  attachInterrupt(digitalPinToInterrupt(unlockButton), lockDoor, FALLING);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}

int unlockDoor(void) {
  interruptTime = millis();
  if(lastInterruptTime < interruptTime - 400){
    //Servo.write(unlockPosition);
    //
    digitalWrite(LED_BUILTIN,HIGH); 
    lastInterruptTime = interruptTime;
  }
}

int lockDoor(void) {
  if(lastInterruptTime < interruptTime - 400){
    //Servo.write(lockPosition);
    digitalWrite(LED_BUILTIN,LOW);
    lastInterruptTime = interruptTime;
  }
}
