#include <LiquidCrystal.h>
#include <Keypad.h>

const int ledGreen = 4;
const int ledRed = 5;
const int ringButton = 2;
volatile long interruptTime;
volatile long lastInterruptTime;
int incomingByte;
int passwordLen = 0;

//set up the lcd
LiquidCrystal lcd(7,8,9,10,11,12);

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
byte rowPins[ROWS] = {3, 6, 13, A5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {A4, A3, A2, A1}; //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  
  digitalWrite(ledRed, HIGH);
  attachInterrupt(0, ringPress, FALLING);
  for(int i=7;i<=12;i++){
    pinMode(i,OUTPUT);
  }
  lcd.setCursor(0,0);
  lcd.print("Welcome!");
}

void loop() {

  char customKey = customKeypad.getKey();
  if (customKey) {
    Serial.write(customKey);
  }
  if (customKey=='*'||customKey=='#'){
    clearPasswordDisplay();
  }
  else if (customKey >=48 && customKey <= 57){
    passwordLen+=1;
    lcd.setCursor(10+passwordLen,0);
    lcd.print("*");
  }

  // check incoming serial data
  if (Serial.available() > 0 ) {
    incomingByte = Serial.read();
    if (incomingByte == 'H') {
      
      lcd.setCursor(0,1);
      lcd.print("Door unlocked.  ");    //added spaces to ensure remaining space doesnt contain unwanted chars
      clearPasswordDisplay();
      
      digitalWrite(ledGreen, HIGH);
      digitalWrite(ledRed, LOW);
    }

    if (incomingByte == 'L') {
      
      lcd.setCursor(0,1);
      lcd.print("Door locked.    ");
      
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
    }
  }
  if(passwordLen>=4){
    passwordLen=0;
  }
}

void ringPress() {
  interruptTime = millis();
  if (lastInterruptTime < interruptTime - 400) {
    Serial.write("R");
    lastInterruptTime = interruptTime;
  }
}

void clearPasswordDisplay(void){
  lcd.setCursor(10,0);
  lcd.write("     ");
  passwordLen = 0;
}
