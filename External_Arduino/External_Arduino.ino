const int ledGreen = 12;
const int ledRed = 13;   
const int ringButton = 2;
volatile long interruptTime;
volatile long lastInterruptTime;
int incomingByte;

void setup() {
  Serial.begin(9600);
  digitalWrite(ledRed, HIGH);
  attachInterrupt(0, ringPress, FALLING);
}

void loop() {

  // check incoming serial data
  if (Serial.available() > 0 ) {
    incomingByte = Serial.read();
    if (incomingByte == 'H') {
      digitalWrite(ledGreen, HIGH);
      digitalWrite(ledRed, LOW);
    }

    if (incomingByte == 'L') {
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
    }
  }
}

void ringPress() {
  interruptTime = millis();
  if (lastInterruptTime < interruptTime - 400) {
    Serial.write("R");
    lastInterruptTime = interruptTime;
  }
}
