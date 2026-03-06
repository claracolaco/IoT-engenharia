const int LED1 = 4;
const int LED2 = 5;
const int LED3 = 6;
const int LED4 = 7;
const int BUZZER = 10;

const int RES_BITS = 10;
const int MAX_DUTY = (1 << RES_BITS) - 1;

const int FREQ_LED = 1000;

const int CH_LED1 = 0;
const int CH_LED2 = 1;
const int CH_LED3 = 2;
const int CH_LED4 = 3;
const int CH_BUZZ = 4;

void setup() {
  Serial.begin(115200);

  // LEDs: 4 canais, 1kHz, 10-bit
  ledcAttachChannel(LED1, FREQ_LED, RES_BITS, CH_LED1);
  ledcAttachChannel(LED2, FREQ_LED, RES_BITS, CH_LED2);
  ledcAttachChannel(LED3, FREQ_LED, RES_BITS, CH_LED3);
  ledcAttachChannel(LED4, FREQ_LED, RES_BITS, CH_LED4);

  ledcWriteChannel(CH_LED1, 0);
  ledcWriteChannel(CH_LED2, 0);
  ledcWriteChannel(CH_LED3, 0);
  ledcWriteChannel(CH_LED4, 0);

  // Buzzer: 1 canal, 10-bit
  ledcAttachChannel(BUZZER, 500, RES_BITS, CH_BUZZ);
  ledcWriteChannel(CH_BUZZ, 0);
}

void setAllLedDuty(int duty) {
  ledcWriteChannel(CH_LED1, duty);
  ledcWriteChannel(CH_LED2, duty);
  ledcWriteChannel(CH_LED3, duty);
  ledcWriteChannel(CH_LED4, duty);
}

void loop() {
  const int FADE_STEP = 8;
  const int FADE_DELAY_MS = 3;  

  const int BUZZ_STEP_HZ  = 20;  
  const int BUZZ_DELAY_MS = 12;  
  const int PAUSE_MS      = 10; 

  // Fase 3 – Fading (0% -> 100% -> 0%) 
  for (int duty = 0; duty <= MAX_DUTY; duty += FADE_STEP) {
    setAllLedDuty(duty);
    delay(FADE_DELAY_MS);
  }
  setAllLedDuty(MAX_DUTY); 

  int startDown = MAX_DUTY - (MAX_DUTY % FADE_STEP);
  for (int duty = startDown; duty >= 0; duty -= FADE_STEP) {
    setAllLedDuty(duty);
    delay(FADE_DELAY_MS);
  }
  setAllLedDuty(0); 

  // Fase 4 – Som variável (500 Hz -> 2 kHz -> 500 Hz) 
  const int DUTY_BUZZ = MAX_DUTY / 2; 
  ledcWriteChannel(CH_BUZZ, DUTY_BUZZ);

  for (int f = 500; f <= 2000; f += BUZZ_STEP_HZ) {
    ledcChangeFrequency(BUZZER, f, RES_BITS);
    delay(BUZZ_DELAY_MS);
  }
  for (int f = 2000; f >= 500; f -= BUZZ_STEP_HZ) {
    ledcChangeFrequency(BUZZER, f, RES_BITS);
    delay(BUZZ_DELAY_MS);
  }

  ledcWriteChannel(CH_BUZZ, 0);
  delay(PAUSE_MS);
}