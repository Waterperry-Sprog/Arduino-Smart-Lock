int ledPins[] = {};
int unlockButton = 2;
int lockButton = 3;
int unlockDoor(void);
int lockDoor(void);



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
  //Servo.write(unlockPosition);
  //
  digitalWrite(LED_BUILTIN,HIGH);
}

int lockDoor(void) {
  //Servo.write(lockPosition);
  digitalWrite(LED_BUILTIN,LOW);
}
