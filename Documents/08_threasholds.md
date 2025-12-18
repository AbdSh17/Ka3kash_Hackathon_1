# Threshold Summary (Main Firmware)

This file summarizes the thresholds and decision rules used in the **main Arduino code**.

ChatGPT prompt: `just read the threashold that is determined for each, and write a markdown file to summarize them`

---

## Sensor Warm-up + Baseline Rules

### MQ-5 (Gas)
- **Samples buffer:** `MQ5_SAMPLES = 20`
- **Warm-up time:** `MQ5_WARMUP_MS = 10,000 ms` (10 seconds)
- **Baseline capture condition:** baseline is captured once **after warm-up**:
  - `if (MQ5_isWarmedUp(&mq5)) MQ5_captureBaseline(&mq5);`

### BME (Environment)
- **I2C address:** `BME_ADDR = 0x76`
- **Samples buffer:** `BME_SAMPLES = 20`
- **Warm-up time:** `BME_WARMUP_MS = 10,000 ms` (10 seconds)
- **Baseline capture condition:** baseline is captured once **after warm-up**:
  - `if (BME680_isWarmedUp(&bme)) BME680_captureBaseline(&bme);`

---

## Classification Thresholds (0..3 severity)

### Gas State (0 = normal, 3 = severe)
- Uses **delta from baseline**:
  - `gas_delta = MQ5_delta(&mq5)`
- **Severe threshold:**
  - `MQ5_DELTA_SEVERE = 60`
- Rule:
  - `gasState = (gas_delta >= 60) ? 3 : 0`

---

### Temperature State (0..3)
Rule based on `temp` (°C):
- **0 (Normal):** `temp < 30`
- **1:** `30 <= temp < 35`
- **2:** `35 <= temp < 40`
- **3 (Severe):** `temp >= 40`

---

### Humidity State (0..3)
Rule based on `hum` (%RH):
- **0 (Normal):** `hum < 55`
- **1:** `55 <= hum < 70`
- **2:** `70 <= hum < 80`
- **3 (Severe):** `hum >= 80`

---

### Air Quality State (0..3)
Rule based on `air_ratio = BME680_airQualityRatio(&bme)`:

- **0 (Normal):** `air_ratio >= 0.90`
- **1:** `0.80 <= air_ratio < 0.90`
- **2:** `0.70 <= air_ratio < 0.80`
- **3 (Severe):** `air_ratio < 0.70`

Air percent is reported as:
- `air_pct = round(air_ratio * 100)`
- Clamped to `[0, 100]`

---

### Pressure State (demo rule)
Rule based on `pres` (hPa-ish depending on your BME wrapper output):
- **3 (Abnormal):** `pres < 980.0`
- **0 (Normal):** otherwise

---

## HVAC ON/OFF Inference Thresholds (Temperature slope)

HVAC is inferred from the **magnitude of filtered temperature slope** `|dT/dt|`:

### Timing / filtering
- **Slope computed every:** `SLOPE_PERIOD_MS = 10,000 ms` (10 seconds)
- Raw slope:
  - `dTdt = (tempC - lastTempForSlope) / 10 sec`  → °C/sec
- EMA filter:
  - `slopeEma = (1 - α) * slopeEma + α * dTdt`
  - `SLOPE_ALPHA = 0.10`

### Hysteresis thresholds
- Turn **ON** threshold:
  - `DTDT_ON  = 0.0030 °C/sec`  (~0.18 °C/min)
- Turn **OFF** threshold:
  - `DTDT_OFF = 0.0020 °C/sec`  (~0.12 °C/min)

### Stability (anti-flapping)
- Must meet the condition for **STABLE_N = 4** consecutive slope checks:
  - ON: `|slopeEma| >= DTDT_ON` for 4 checks
  - OFF: `|slopeEma| <= DTDT_OFF` for 4 checks

---

## Reporting / Logging Triggers

### Event logs
A log is sent only when a state changes:
- Gas: normal ↔ severe
- Temp: 0..3 changes
- Hum: 0..3 changes
- Air: 0..3 changes
- Pressure: normal ↔ abnormal
- HVAC: OFF ↔ ON

### Detail packets (periodic)
- Runs at ~10 Hz loop, sends details every 10 ticks → **~1 second**
- Packet format:
  - `DET,air_pct,temp,hum,pres,hvac`
- **Note:** current code sends `temp - 10` (offset) in the details line.
