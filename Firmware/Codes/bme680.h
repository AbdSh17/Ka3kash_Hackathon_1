#ifndef BME680_H
#define BME680_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t warmup_ms;
    uint32_t start_ms;

    uint16_t samplesNumber;
    uint64_t sum;
    uint32_t *buffer;
    uint16_t index;
    bool     filled;

    uint32_t baseline;

    float temperature;
    float humidity;
    float pressure;
    uint32_t gas_raw;
} BME680;

bool BME680_init(BME680 *s,
                 uint32_t *buffer,
                 uint16_t samplesNumber,
                 uint32_t warmup_ms,
                 uint8_t i2c_addr);

void BME680_update(BME680 *s);

bool BME680_isWarmedUp(const BME680 *s);

uint32_t BME680_readAverageGas(const BME680 *s);
uint32_t BME680_readRawGas(const BME680 *s);

void BME680_captureBaseline(BME680 *s);
uint32_t BME680_getBaseline(const BME680 *s);

float BME680_airQualityRatio(const BME680 *s);
float BME680_airQualityPercent(const BME680 *s);

float BME680_temperature(const BME680 *s);
float BME680_humidity(const BME680 *s);
float BME680_pressure(const BME680 *s);

#ifdef __cplusplus
}
#endif

#endif
