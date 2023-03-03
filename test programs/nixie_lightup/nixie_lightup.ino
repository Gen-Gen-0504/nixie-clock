/* カソード制御ピン */
const int PIN_A = 32;
const int PIN_B = 25;
const int PIN_C = 26;
const int PIN_D = 33;

/* 指定した数字を点灯させる関数 */
void lightup(int n) { 
  digitalWrite(PIN_A, n&B0001);
  digitalWrite(PIN_B, n&B0010);
  digitalWrite(PIN_C, n&B0100);
  digitalWrite(PIN_D, n&B1000);
}


void setup() {
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_C, OUTPUT);
  pinMode(PIN_D, OUTPUT);
  digitalWrite(PIN_A, HIGH);
  digitalWrite(PIN_B, HIGH);
  digitalWrite(PIN_C, HIGH);
  digitalWrite(PIN_D, HIGH);
}

void loop() {
  /* 0～9まで順に光らせる */
  for(int n = 0; n < 10; ++n) {
    lightup(n);
    delay(300);
  }
}
