int led1 = 6; // green
int led2 = 5;
int sensor = 7;
int priState = LOW;
int val = 0;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(sensor, INPUT);
  Serial.begin(9600);
}

void loop() {
  val = digitalRead(sensor);

  if (val == LOW) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    if (priState == LOW) {
      Serial.println("HI");
      priState = HIGH;
    }
  }
  else {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    if (priState == HIGH) {
      Serial.println("Bye");
      priState = LOW;
    }
  }
}
