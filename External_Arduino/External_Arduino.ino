#include <LiquidCrystal.h>
#include <Keypad.h>

const int ledGreen = 4;
const int ledRed = 5;
const int ringButton = 2;
volatile long interruptTime;
volatile long lastInterruptTime;
const int pinLength = 4;
int curPinLength = 0;

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
  // set up the pins 
  pinMode(iledRed, OUTPUT);
  attachInterrupt(0, ringPress, FALLING);
  for(int i=7;i<=12;i++){
    pinMode(i,OUTPUT);
  }
  // light up red LED as default light
  digitalWrite(ledRed, HIGH);
  // set up the LCD screen
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Welcome!");
  // initialize the serial connection
  Serial.begin(9600);
}

void loop() {
  // check if a key has been pressed
  // if it has, send it over to the internal arduino
  char customKey = customKeypad.getKey();
  if (customKey) {
    Serial.write(customKey);
    // display pin progress on LCD
    // either add a * to the top right or clear that area
    if (customKey=='*'||customKey=='#'){
      clearPinDisplay();
    }
    else if (customKey >=48 && customKey <= 57){
      curPinLength+=1;
      lcd.setCursor(10+curPinLength,0);
      lcd.print("*");
    }
  }

  // if there is data in the serial connection, use it
  if (Serial.available() > 0 ) {
    byte incomingByte = Serial.read();
    if (incomingByte == 'U') {
      // this is the unlock signal, so turn we turn
      // on the green LED and write it on the LCD
      lcd.setCursor(0,1);
      lcd.print("Door unlocked.  ");    //added spaces to ensure remaining space doesnt contain unwanted chars
      clearPinDisplay();
      digitalWrite(ledGreen, HIGH);
      digitalWrite(ledRed, LOW);
    } else if (incomingByte == 'L') {
      // this is the lock signal, so we change the
      // LCD display again and switch the LED to red
      lcd.setCursor(0,1);
      lcd.print("Door locked.    ");
      digitalWrite(ledRed, HIGH);
      digitalWrite(ledGreen, LOW);
    }
  }
  if(curPinLength>=pinLength){
    // reset the pin if it is longer than it is supposed to be
    curPinLength=0;
  }
}

void ringPress() {
  // interrupt function, sends a simple signal ro ring on the internal arduino
  interruptTime = millis();
  if (lastInterruptTime < interruptTime - 400) {
    Serial.write("R");
    lastInterruptTime = interruptTime;
  }
}

void clearPinDisplay(void){
  // removes the pin on the LCD
  lcd.setCursor(10,0);
  lcd.write("     ");
  curPinLength = 0;
}
