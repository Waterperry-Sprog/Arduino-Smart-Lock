int commPin = 7; // pin attached to other arduino's interrupt
int buttonPins = [4, 5]; // pins the buttons are attached to
int piezoPin = 6; // pin attached to speaker
int ioPins = [8,9,10,11,12]; // data pins
int totalPins = 5; // number of data pins being used
bool inputSequence[totalPins]; // button sequence

void setup() {
  attachInterrupt(0, messageReceived, FALLING);
  attachInterrupt(1, buttonPress,  FALLING);
  for (int i = 0; i < 2; i++) {
    pinMode(buttonPins[i], INPUT);
  }
}

void loop() {
  delay(1000); // placeholder
}

void buttonPress() { // when button interrupt is triggered
  boolean isNewSequence = true;
  int inputIndex = 0;
  for (int i = 0; i < totalPins; i++) { // if button sequence is not full, do not reset it
    if (inputSequence[i] == null) {
      isNewSequence = false;
      inputIndex = i;
      break;
    }
  }
  if (isNewSequence) { // reset sequence if full
    inputSequence = new bool[totalPins];
    inputIndex = 0;
  }
  if (digitalRead(buttonPins[0]) == HIGH) { // set a value based on which button was presseds
    inputSequence[inputIndex] = true;
  } else if (digitalRead(buttonPins[1]) == HIGH) {
    inputSequence[inputIndex] = false;
  }
  if (inputIndex == totalPins - 1) { // if button sequence is full, send it to other arduino
    sendData(inputSequence);
  }
}

void sendData(int[] sequence) {
  digitalWrite(commPin, HIGH); // trigger interrupt on other arduino
  delay(100);
  for (int i = 0; i < totalPins; i++) { // turn pins to output and give correct values
    pinMode(ioPins[i], OUTPUT);
    if (sequence[i] == true) {
      digitalWrite(ioPins[i], HIGH);
    } else {
      digitalWrite(ioPins[i], LOW);
    }
  }
  digitalWrite(commPin, LOW); // may need to switch pin off at a later or earlier interval
  delay(200);
  for (int i = 0; i < totalPins; i++) {
    digitalWrite(ioPins[i], LOW);
  }
}

void messageReceived() { // when the interrupt pin is triggered run code
  int code;
  for (int i = 0; i < totalPins; i++) { // turn all pins to input to receive code
    pinMode(ioPins[i], INPUT);
  }
  delay(100);
  for (int i = 0; i < totalPins; i++) { // calculate the code to denary value
    if (digitalRead == HIGH) {
      code = code + 2^i;
    }
  }
  // DO SOMETHING WITH CODE
}
