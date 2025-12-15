#include "MQ5.h"
#include "gas_timer.h"

// ================= MQ5 =================
#define MQ5_PIN A0

#define WARMUP_MS       60 * 1000    
#define SAMPLES_NUMBER  20
#define DELTA_THRESHOLD 80 

uint16_t mq5_buffer[SAMPLES_NUMBER];
// ================= MQ5 =================

MQ5 mq5;

void setup() {
  Serial.begin(115200);

  MQ5_timerSetup();

  MQ5_init(
    &mq5,
    MQ5_PIN,
    mq5_buffer,
    SAMPLES_NUMBER,
    WARMUP_MS,
    DELTA_THRESHOLD
  );

  Serial.println("MQ5 starting... warming up");
}

void loop() {
  
  uint16_t avg = MQ5_readAverage(&mq5);

  // After warm-up, capture baseline once
  if (MQ5_isWarmedUp(&mq5) && !MQ5_hasBaseline(&mq5)) {
    MQ5_captureBaseline(&mq5);
    Serial.print("Baseline captured: ");
    Serial.println(MQ5_getBaseline(&mq5));
  }

  // Print readings
  Serial.print("AVG = ");
  Serial.print(avg);

  if (MQ5_hasBaseline(&mq5)) {
    Serial.print(" | Δ = ");
    Serial.print(MQ5_delta(&mq5));

    Serial.print(" | LEAK = ");
    Serial.print(MQ5_leakDetected(&mq5) ? "YES" : "NO");
  } else {
    Serial.print(" | waiting for baseline...");
  }

  Serial.println();

  delay(100);
}

ISR(TIMER1_COMPA_vect)
{
  MQ5_update(&mq5);
}
