#include "MQ5.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

void MQ5_init(MQ5 *s,
              uint8_t analog_pin,
              uint16_t *buffer,
              uint16_t samplesNumber,
              uint32_t warmup_ms,
              uint16_t delta_threshold)
{
    s->analog_pin = analog_pin;
    s->warmup_ms = warmup_ms;
    s->start_ms  = millis();

    s->samplesNumber = (samplesNumber == 0) ? 1 : samplesNumber;
    s->buffer = buffer;
    s->index  = 0;
    s->sum    = 0;
    s->filled = false;

    for (uint16_t i = 0; i < s->samplesNumber; i++)
        s->buffer[i] = 0;

    s->baseline = 0;
    s->delta_threshold = delta_threshold;
}

uint16_t MQ5_readCurrentValue(const MQ5 *s) {
    int v = analogRead(s->analog_pin);
    if (v < 0) v = 0;
    if (v > 1023) v = 1023;
    return (uint16_t)v;
}

void MQ5_update(MQ5 *s) {
    uint16_t x = MQ5_readCurrentValue(s);

    s->sum -= s->buffer[s->index];
    s->buffer[s->index] = x;
    s->sum += x;

    s->index++;
    if (s->index >= s->samplesNumber) {
        s->index = 0;
        s->filled = true;
    }
}

uint16_t MQ5_readAverage(const MQ5 *s) {
    uint16_t n = s->filled ? s->samplesNumber : (s->index == 0 ? 1 : s->index);
    return (uint16_t)(s->sum / n);
}

bool MQ5_isWarmedUp(const MQ5 *s) {
    return (uint32_t)(millis() - s->start_ms) >= s->warmup_ms;
}

void MQ5_captureBaseline(MQ5 *s) {
    s->baseline = MQ5_readAverage(s);
}

uint16_t MQ5_getBaseline(const MQ5 *s) {
    return s->baseline;
}

int32_t MQ5_delta(const MQ5 *s) {
    return (int32_t)MQ5_readAverage(s) - (int32_t)s->baseline;
}

bool MQ5_leakDetected(const MQ5 *s) {
    return MQ5_delta(s) >= (int32_t)s->delta_threshold;
}
