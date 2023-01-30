#include <SoftwareSerial.h>

#include <MsTimer2.h>
#include <avr/sleep.h>
#include <avr/power.h>

#define MATRIX_SIZE 8 // 8 * 8

#define MATRIX_SUN { \
  {0b01000000, 0b00011001, 0b00111100, 0b01111110, 0b01111110, 0b00111100, 0b10011000, 0b00000010}, \
  {0b00000010, 0b10011000, 0b00111100, 0b01111110, 0b01111110, 0b00111100, 0b00011001, 0b01000000}  \
}
#define MATRIX_SNOW { \
  {0b00011000, 0b01011010, 0b00100100, 0b11011011, 0b11011011, 0b00100100, 0b01011010, 0b00011000}, \
  {0b11100111, 0b10100101, 0b11011011, 0b00100100, 0b00100100, 0b11011011, 0b10100101, 0b11100111}  \
}

#define MATRIX_ZERO { \
  0b00000000, 0b00001110, 0b00001010, 0b00001010, 0b00001010, 0b00001110, 0b00000000, 0b00000000, \
}
#define MATRIX_ONE { \
  0b00000000, 0b00000100, 0b00001100, 0b00000100, 0b00000100, 0b00001110, 0b00000000, 0b00000000, \
}
#define MATRIX_TWO { \
  0b00000000, 0b00001110, 0b00000010, 0b00001110, 0b00001000, 0b00001110, 0b00000000, 0b00000000, \
}
#define MATRIX_THREE { \
  0b00000000, 0b00001110, 0b00000010, 0b00001110, 0b00000010, 0b00001110, 0b00000000, 0b00000000, \
}
#define MATRIX_FOUR { \
  0b00000000, 0b00001010, 0b00001010, 0b00001110, 0b00000010, 0b00000010, 0b00000000, 0b00000000, \
}
#define MATRIX_FIVE { \
  0b00000000, 0b00001110, 0b00001000, 0b00001110, 0b00000010, 0b00001110, 0b00000000, 0b00000000, \
}
#define MATRIX_SIX { \
  0b00000000, 0b00001110, 0b00001000, 0b00001110, 0b00001010, 0b00001110, 0b00000000, 0b00000000, \
}
#define MATRIX_SEVEN { \
  0b00000000, 0b00001110, 0b00001010, 0b00000010, 0b00000010, 0b00000010, 0b00000000, 0b00000000, \
}
#define MATRIX_EIGHT { \
  0b00000000, 0b00001110, 0b00001010, 0b00001110, 0b00001010, 0b00001110, 0b00000000, 0b00000000, \
}
#define MATRIX_NINE { \
  0b00000000, 0b00001110, 0b00001010, 0b00001110, 0b00000010, 0b00001110, 0b00000000, 0b00000000, \
}

#define SET_MINUS 0b01100000

// Column Pin
#define C1 A3
#define C2 8
#define C3 9
#define C4 A1
#define C5 A0
#define C6 A2
#define C7 A4
#define C8 A5

// for Row Pin
#define SER_PIN 10
#define SCK_PIN 11
#define RCK_PIN 12

// matrix line specific
#define UPPER 0
#define LINE1 1
#define LINE2 2
#define LINE3 3
#define LINE4 4
#define LINE5 5
#define LINE6 6
#define LOWER 7

#define LED13 13
#define BUTTON 2

#define SLEEP 0
#define WAKE 1

// Bit extract macro function : data 에서 count 번째 비트 부터 bit와 비교하여 추출
#define extract_bits(data, bit, count) (((data) >> (count)) & (bit))

uint8_t colPins[8]={ C1, C2, C3, C4, C5, C6, C7, C8};

enum Picture { WARM, COLD };
enum Picture picture;

enum Temperature { ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE };
enum Temperature temp;

uint8_t patterns[2][2][8] = {MATRIX_SUN, MATRIX_SNOW};
int numbers[10][8] = {MATRIX_ZERO, MATRIX_ONE, MATRIX_TWO, MATRIX_THREE, MATRIX_FOUR, MATRIX_FIVE, MATRIX_SIX, MATRIX_SEVEN, MATRIX_EIGHT, MATRIX_NINE};
uint8_t minus = SET_MINUS;

static byte state = 0;
volatile byte sleepState = SLEEP;

#define P_RXD 6
#define P_TXD 7
SoftwareSerial P_serial(P_RXD, P_TXD);
String temperatureString = "";
byte getData = 0;
int temperature = 0;
ISR(WDT_vect){}

void setup() {
  P_serial.begin(9600);
  pinMode(LED13, OUTPUT);
  digitalWrite(LED13, LOW);

  for(int i=0; i<MATRIX_SIZE; i++) {
    pinMode(colPins[i], OUTPUT);
  }
  pinMode(SER_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(RCK_PIN, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  if(sleepState == WAKE){
    if(getData == 0){
      P_serial.println("@");
      while(P_serial.available()) {
        temperatureString = P_serial.readStringUntil('\n');
      }
      char str[20];
      unsigned int length = temperatureString.length();
      temperatureString.toCharArray(str, length);
      int i = 0;
      char *sArr[2];
      char *ptr = NULL;

      ptr = strtok(str, "#");
      while(ptr != NULL){
        sArr[i++] = ptr;
        ptr = strtok(NULL, "#");
      }
      int sign = atoi(sArr[0]);
      int number = atoi(sArr[1]);
      if(sign >= 0){
        temperature = number;
      }
      else{
        temperature = (-1)*number;
      }
      getData = 1;
    }
    else{
      digitalWrite(LED13, HIGH);
      displayMatrix(temperature);
    }
  }
  else if(sleepState == SLEEP){
    sleep();
  }
}

void clear(){
  for(int k=0; k<MATRIX_SIZE; k++) {
    digitalWrite(colPins[k], HIGH); // Cleanup cols
  }
}

void writeRowData(byte data) {
  digitalWrite(RCK_PIN, LOW);
  shiftOut(SER_PIN, SCK_PIN, LSBFIRST, data);
  digitalWrite(RCK_PIN, HIGH);
}

void displayMatrix(int temperature) {
  int rowbits = 0b10000000;
  if(temperature >= 10){
    for(int row=0; row<MATRIX_SIZE; row++) {
      clear();
      writeRowData(rowbits); // prepare to write the row
      for(int col=0; col<MATRIX_SIZE; col++) {
        digitalWrite(colPins[7-col], !extract_bits(patterns[WARM][state][row], 0x01, col));
      }
      delay(1);
      writeRowData(0);
      rowbits >>= 1; 
    }
  }
  else if(temperature >= 0){
    for(int row=0; row<MATRIX_SIZE; row++) {
      clear();
      writeRowData(rowbits); // prepare to write the row
      for(int col=0; col<MATRIX_SIZE; col++) {
        digitalWrite(colPins[7-col], !extract_bits(numbers[temperature][row], 0x01, col));
      }
      delay(1);
      writeRowData(0);
      rowbits >>= 1; 
    }
  }
  else if(temperature >= -9){
    temperature = (-1)*temperature;
    numbers[temperature][3] = numbers[temperature][3] + minus;
    for(int row=0; row<MATRIX_SIZE; row++) {
      clear();
      writeRowData(rowbits); // prepare to write the row
      for(int col=0; col<MATRIX_SIZE; col++) {
        digitalWrite(colPins[7-col], !extract_bits(numbers[temperature][row], 0x01, col));
      }
      delay(1);
      writeRowData(0);
      rowbits >>= 1; 
    }
  }
  else {
    for(int row=0; row<MATRIX_SIZE; row++) {
      clear();
      writeRowData(rowbits); // prepare to write the row
      for(int col=0; col<MATRIX_SIZE; col++) {
        digitalWrite(colPins[7-col], !extract_bits(patterns[COLD][state][row], 0x01, col));
      }
      delay(1);
      writeRowData(0);
      rowbits >>= 1; 
    }
  }
}

void sleep(){
  sleep_enable();
  MsTimer2::stop();
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
  MsTimer2::set(1000, timerDot);
  MsTimer2::start();
}

void timerDot() {
  static byte count = 0;
  count++;
  state = !state;
  if (count == 10){
    sleepState = SLEEP;
    count = 0;
  }
}