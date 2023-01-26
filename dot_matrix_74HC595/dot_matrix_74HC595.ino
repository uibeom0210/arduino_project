#include <MsTimer2.h>

#define MATRIX_SIZE 8 // 8 * 8
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

// Bit extract macro function : data 에서 loc 번째 비트 부터 area 영역 만큼 추출
// ex) extract_bits( 0b00000010, 1, 2) 결과 1 -> 0b00000010 데이터의 2번째 비트 1크기 만큼 추출  
#define extract_bits(data, area, loc)   (((data) >> (loc)) & (area))

uint8_t colPins[8]={ C1, C2, C3, C4, C5, C6, C7, C8};

//sun
//uint8_t matrixStatus[2][8] ={{0b00010000, 0b00000000, 0b00011000, 0b00111101, 0b10111100, 0b00011000, 0b00000000, 0b00001000},/
//                            {0b00000000, 0b01000010, 0b00011000, 0b00111100, 0b00111100, 0b00011000, 0b01000010, 0b00000000}};/

//clude
//uint8_t matrixStatus[2][8] ={{0b01111000, 0b11111100, 0b01111000, 0b00000000, 0b00011110, 0b00111111, 0b00011110, 0b00000000},
//                            {0b00011110, 0b00111111, 0b00011110, 0b00000000, 0b01111000, 0b11111100, 0b01111000, 0b00000000}};

//rain
uint8_t matrixStatus[2][8] ={{0b01000010, 0b01001010, 0b01001010, 0b00001000, 0b00100001, 0b10100101, 0b10100101, 0b10000100},
                            {0b10001010, 0b00000000, 0b01000010, 0b01001010, 0b01001010, 0b00001000, 0b00100001, 0b10100101}};

//snow
//uint8_t matrixStatus[2][8] ={{0b01000001, 0b00000100, 0b00010000, 0b10000000, 0b00001001, 0b00100000, 0b00000000, 0b10000010},
//                            {0b10001000, 0b00000000, 0b01000001, 0b00000100, 0b00010000, 0b10000000, 0b00001001, 0b00100000}};

static byte state = 0;
void setup() {
  // put your setup code here, to run once:
  for(int i=0; i<MATRIX_SIZE; i++) {
    pinMode(colPins[i], OUTPUT);
  }
  pinMode(SER_PIN, OUTPUT);
  pinMode(SCK_PIN, OUTPUT);
  pinMode(RCK_PIN, OUTPUT);
  //MsTimer2::set(800, timerDot); (sun, cloud)1
  MsTimer2::set(300, timerDot);
  MsTimer2::start();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  displayMatrix();
}

void clear(){
  for(int k=0; k<MATRIX_SIZE; k++) 
      digitalWrite(colPins[k],HIGH); // Cleanup cols
}

void writeRowData(byte data) {
  digitalWrite(RCK_PIN, LOW);
  shiftOut(SER_PIN, SCK_PIN, LSBFIRST, data);
  digitalWrite(RCK_PIN, HIGH);
}

void displayMatrix(){
  int rowbits = 0b10000000;
  for(int row=0; row<MATRIX_SIZE; row++) {
    clear();
    writeRowData(rowbits); // prepare to write the row
    for(int col=0; col<MATRIX_SIZE; col++)
      digitalWrite(colPins[7-col], !extract_bits(matrixStatus[state][row], 0x01, col));
    delay(1);
    writeRowData(0);
    rowbits >>= 1; 
  }
}

void timerDot() {
  state = !state;
}