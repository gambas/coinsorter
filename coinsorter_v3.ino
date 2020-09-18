#include <TM1637.h>

// TM1637
const int CLK = A1; // amarelo
const int DIO = A0; // verde
TM1637 tm1637(CLK, DIO);

const int IRinput[] = {A5, A4, A3, A2};

const int coinvalue[] = {1,5,10,20};

int num = 0;

int ctrl = 0;
int b_pres = 0;
int i = 0;


void setup() {
  Serial.begin(9600);
  
  for(i = 0; i < 4; i++){
    pinMode(IRinput[i], INPUT);
  }
  
  tm1637.init();
  tm1637.set(BRIGHT_TYPICAL);
}

void loop() {
  tm_print(num);
  
  ctrl = 0;
  
  while(ctrl == 0){
    for(i=0;i<4;i++){
      if(digitalRead(IRinput[i]) == LOW){
        ctrl = 1;
        break;
      }
    }
  }
  delay(50); // debounce
  while(digitalRead(IRinput[i]) == LOW);
  Serial.println("Detection");
  delay(100);
  num+=coinvalue[i];
}

void tm_print(int x){
  tm1637._PointFlag = 1;
  tm1637.displayNum(x);
}
