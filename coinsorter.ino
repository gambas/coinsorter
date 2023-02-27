/**
 * To do:
 * - use direct port manipulation for coin detection, enables for simultaneous readings, saves time and asm lines
 * - change LCD 2 modes to 9 modes, probably reducing the amount of LCD comms
 * - some performance metrics testing
 * - clean the code
 */
/*
 *
 *DDRD = B11111110;  // sets Arduino pins 1 to 7 as outputs, pin 0 as input
  DDRD = DDRD | B11111100;  // this is safer as it sets pins 2 to 7 as outputs
                            // without changing the value of pins 0 & 1, which are RX & TX  
 */
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
// A4 - sda, A5 - clk
const int IRinput[] = {0, 1, 2, 3, 4, 5, 6, 7};

const float coinvalue[] = {0.01, 0.02, 0.05, 0.10, 0.20, 0.5, 1, 2};

float num = 1.34;
char total[32] = "";

int ctrl = 0;
//int b_pres = 0;
int i = 0;

/****************/
/* button stuff */
/****************/

const int buttonPin = A0;    // the number of the pushbutton pin
const int ledPin = 13;      // the number of the LED pin
int lcdState = LOW;         // the current state of the LCD mode
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 70;    // the debounce time; increase if the output flickers
int reading; // actual reading

/*******************/
/* mode ctrl stuff */
/*******************/
unsigned long runtime;
unsigned long msTime;
unsigned long mode;

void setup() {
  Serial.begin(9600);
  
  for(i = 0; i < 4; i++) pinMode(IRinput[i], INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.print("Total acumulado:");
  
}

void loop() {
  print_total_to_lcd(num, lcdState);
  
  ctrl = 0;
  
  while(ctrl == 0){
    for(i=0;i<4;i++){
      if(digitalRead(IRinput[i]) == HIGH){
        delay(100);
        if(digitalRead(IRinput[i]) == HIGH){
          Serial.println("Detection1");
          ctrl = 1;
          break;
        }
      }
    }
  }
  while(digitalRead(IRinput[i]) == HIGH);
  //Serial.println("Detection2");
  num+=coinvalue[i];


  // button debounce
  reading = digitalRead(buttonPin);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == HIGH) {
        Serial.println("toggle");
        lcdState = !lcdState;
        msTime = millis();
      }
    }
  }

  lastButtonState = reading;
  //Serial.println(lcdState);
}


void print_total_to_lcd(float num, int state){
  if(state){
    runtime = millis() - msTime;
    mode = (runtime>>10) & 7; // magic shit, roll over 8 modes (7) for 1 sec each (<<10)
    lcd.setCursor(0, 1);
    switch(mode){
        case 0:  
          lcd.print("0.01 -> x2");
          break;
    
        case 1:
          lcd.print("0.02 -> x1");
          break;
        
        case 2:
          lcd.print("0.05 -> x2");
          break;
        
        case 3:
          lcd.print("0.10 -> x2");
          break;
    
        case 4:
          lcd.print("0.20 -> x0");
          break;
          
        case 5:
          lcd.print("0.50 -> x2");
          break;
    
        case 6:
          lcd.print("1.00 -> x0");
          break;

        case 7:
          lcd.print("2.00 -> x0");
          break;
    }
    
  } else {
    dtostrf(num, 0, 2, total);
    lcd.setCursor(0, 1);
    lcd.print(total);
    lcd.print(" euros");
  }
  
}
