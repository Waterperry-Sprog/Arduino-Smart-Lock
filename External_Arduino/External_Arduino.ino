#include <Keypad.h>

const int ledGreen = 12;
const int ledRed = 13;   
const int ringButton = 2;
volatile long interruptTime;
volatile long lastInterruptTime;
int incomingByte;

// keypad data
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  Serial.begin(9600);
  digitalWrite(ledRed, HIGH);
  attachInterrupt(0, ringPress, FALLING);
}

void loop() {

  char customKey = customKeypad.getKey();
  if (customKey) {
    Serial.write(customKey)
  }
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
