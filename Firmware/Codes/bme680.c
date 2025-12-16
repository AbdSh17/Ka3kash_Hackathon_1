#include "BME680.h"

#ifdef ARDUINO
#include <Arduino.h>
#endif

#include <Adafruit_BME680.h>

static Adafruit_BME680 bme;

bool BME680_init(BME680 *s,
                 uint32_t *buffer,
                 uint16_t samplesNumber,
                 uint32_t warmup_ms,
                 uint8_t i2c_addr)
{
    if (!bme.begin(i2c_addr))
        return false;

    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);

    s->warmup_ms = warmup_ms;
    s->start_ms = millis();

    s->samplesNumber = (samplesNumber == 0) ? 1 : samplesNumber;
    s->buffer = buffer;
    s->index = 0;
    s->sum = 0;
    s->filled = false;

    for (uint16_t i = 0; i < s->samplesNumber; i++)
        s->buffer[i] = 0;

    s->baseline = 0;

    s->temperature = 0;
    s->humidity = 0;
    s->pressure = 0;
    s->gas_raw = 0;

    return true;
}

void BME680_update(BME680 *s)
{
    if (!bme.performReading())
        return;

    s->temperature = bme.temperature;
    s->humidity = bme.humidity;
    s->pressure = bme.pressure / 100.0f;
    s->gas_raw = bme.gas_resistance;

    s->sum -= s->buffer[s->index];
    s->buffer[s->index] = s->gas_raw;
    s->sum += s->buffer[s->index];

    s->index++;
    if (s->index >= s->samplesNumber)
    {
        s->index = 0;
        s->filled = true;
    }
}

bool BME680_isWarmedUp(const BME680 *s)
{
    return (uint32_t)(millis() - s->start_ms) >= s->warmup_ms;
}

uint32_t BME680_readAverageGas(const BME680 *s)
{
    uint16_t n = s->filled ? s->samplesNumber : (s->index == 0 ? 1 : s->index);
    return (uint32_t)(s->sum / n);
}

void BME680_captureBaseline(BME680 *s)
{
    s->baseline = BME680_readAverageGas(s);
}

float BME680_airQualityRatio(const BME680 *s)
{
    if (s->baseline == 0)
        return 0;

    return (float)BME680_readAverageGas(s) / (float)s->baseline;
}

float BME680_temperature(const BME680 *s)
{
    return s->temperature;
}

float BME680_humidity(const BME680 *s)
{
    return s->humidity;
}

float BME680_pressure(const BME680 *s)
{
    return s->pressure;
}
