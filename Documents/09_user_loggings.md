# Logging Summary

This section summarizes how logging works in the system and what events generate logs.

---

## General Behavior

- Logging is **event-based**, not continuous.
- A log is generated **only when a state changes**.
- Logs are not repeated unless the state changes again.
- Each log includes:
  - Severity level
  - Subsystem identifier (Part ID)
  - Short human-readable message

---

## Log Levels

| Level | Meaning |
|------:|---------|
| 4 | Normal / informational |
| 1 | Advisory (minor deviation) |
| 2 | Moderate issue |
| 3 | Severe / warning |

---

## Logged Subsystems and Conditions

### Gas (MQ-5)
- Logged when gas state changes:
  - Normal ↔ Severe
- Triggered by delta from baseline crossing the severe threshold.

---

### Temperature
- Logged when temperature severity level changes:
  - Normal → Advice → Moderate → Severe

---

### Humidity
- Logged when humidity severity level changes:
  - Normal → Advice → Moderate → Severe

---

### Air Quality
- Logged when air quality classification changes:
  - Normal
  - Slightly degraded
  - Degraded
  - Severe

---

### Pressure
- Logged when pressure becomes abnormal or returns to normal.

---

### HVAC Status
- Logged when inferred HVAC state changes:
  - OFF → ON
  - ON → OFF

---

### System Events
- Arduino startup
- Sensor initialization failure
- Baseline capture completion (MQ-5, BME)

---

## Log Characteristics

- Logs represent **events**, not raw sensor data.
- Intended for:
  - Alerts
  - History
  - Diagnostics
- Not used for live telemetry or graphs.

---
