/* CoinSorter (development stage)
 *  
 * ---Sum: Primeira fase da parte electrónica, pretende-se
 * simular o efeito dos sensores com botões, e o display
 * com leds.
 *  
 * $Versão para arduino nano$
 * 
 * hw: 4 sensores/butões para 6 leds, que substituem o display
 * 
 * 7/11/2019
 * 
 * by gambas&dicker
 */

// butões/sensores
int sens[] = {2, 3, 4, 5};

// leds/display
int led[] = {6, 7, 8, 9, 10, 11};

// coin value
int value_of[] = {1,2,5,10};

// total
int total = 0;

// mask
int mask = 1;

// inp_detect
int input_detection = 0;

//----------------------------------//
//------------- setup()-------------//
void setup() {
  
  for(int i = 0; i < 4; i++) {
    pinMode(sens[i], INPUT);
  }
  for(int i = 0; i < 6; i++) {
    pinMode(led[i], OUTPUT);
  }
}

//---------------------------------//
//------------- loop()-------------//
void loop() {
  input_detection = 0;
  while(input_detection == 0){
    for(int i = 0; (i < 4 && input_detection == 0); i++){
      if(digitalRead(sens[i]) == HIGH){
        total += value_of[i];
        input_detection = 1;
      }
    }
  }
  mask = 1;
  for(int i = 5; i >= 0; i--){
    if(mask & total != 0){
      digitalWrite(led[i], HIGH);
    } else {
      digitalWrite(led[i], LOW);
    }
    mask = mask << 1;
  }
  delay(1000);
}
