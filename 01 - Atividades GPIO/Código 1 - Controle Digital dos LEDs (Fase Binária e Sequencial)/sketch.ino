const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 6;
const int LED4 = 7;

const int BTN_A = 8;
const int BTN_B = 9;
const int BUZZER = 10;

void setBinary(uint8_t v) {
  digitalWrite(LED1, (v >> 0) & 1);
  digitalWrite(LED2, (v >> 1) & 1);
  digitalWrite(LED3, (v >> 2) & 1);
  digitalWrite(LED4, (v >> 3) & 1);
}

void allOff() {
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
}

void setup() {
  Serial.begin(115200);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  // botões com pull-up interno (pressionado = LOW)
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);
}

void loop() {
  // Fase 1: contador 0..15 (500ms) 
  for (uint8_t v = 0; v < 16; v++) {
    setBinary(v);
    Serial.print("contador = ");
    Serial.println(v);
    delay(500);
  }

  // Fase 2: varredura indo e voltando 
  int idx = 0;   // 0..3
  int dir = +1;  // +1 indo, -1 voltando

  for (int steps = 0; steps < 12; steps++) {
    allOff();

    if (idx == 0) digitalWrite(LED1, HIGH);
    if (idx == 1) digitalWrite(LED2, HIGH);
    if (idx == 2) digitalWrite(LED3, HIGH);
    if (idx == 3) digitalWrite(LED4, HIGH);

    delay(250);

    idx += dir;
    if (idx == 3) dir = -1;
    else if (idx == 0) dir = +1;
  }
}