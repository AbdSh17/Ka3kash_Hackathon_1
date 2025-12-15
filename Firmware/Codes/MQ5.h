#ifndef MQ5_H
#define MQ5_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t  analog_pin;
    uint32_t warmup_ms;
    uint32_t start_ms;

    uint16_t samplesNumber;
    uint32_t sum;
    uint16_t *buffer;
    uint16_t index;
    bool     filled;

    uint16_t baseline;
    uint16_t delta_threshold;
} MQ5;

void MQ5_init(MQ5 *s,
              uint8_t analog_pin,
              uint16_t *buffer,
              uint16_t samplesNumber,
              uint32_t warmup_ms,
              uint16_t delta_threshold);

void MQ5_update(MQ5 *s);

uint16_t MQ5_readCurrentValue(const MQ5 *s);
uint16_t MQ5_readAverage(const MQ5 *s);

bool MQ5_isWarmedUp(const MQ5 *s);

void     MQ5_captureBaseline(MQ5 *s);
uint16_t MQ5_getBaseline(const MQ5 *s);

int32_t  MQ5_delta(const MQ5 *s);
bool     MQ5_leakDetected(const MQ5 *s);

#ifdef __cplusplus
}
#endif

#endif
