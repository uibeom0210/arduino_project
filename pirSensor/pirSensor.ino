
#include <avr/sleep.h>
#include <avr/power.h>

#define LED13 13
#define LED1 6 // green
#define LED2 5
#define BUTTON 2

#define SLEEP 0
#define WAKE 1

volatile byte sleepState = SLEEP;
int pirValue = 0;

ISR(WDT_vect){

}

void setup() {
  pinMode(LED13, OUTPUT);
  digitalWrite(LED13, LOW);
  
  Serial.begin(9600);
}

void loop() {
  
  if(sleepState == WAKE){
    digitalWrite(LED13, HIGH);
    delay(5000);
    sleepState = SLEEP;
  }
  else if(sleepState == SLEEP){
    sleep();
  }  
}

void sleep(){
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(BUTTON), buttonClick, FALLING);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  digitalWrite(LED13, LOW);
  delay(1000);
  sleep_mode(); // CPU sleep
  Serial.println("just woke up!");
  detachInterrupt(digitalPinToInterrupt(BUTTON));
}

void buttonClick(){
  sleepState = WAKE;
  sleep_disable(); // When wake
}


