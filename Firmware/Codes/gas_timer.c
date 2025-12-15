#include "gas_timer.h"

#ifdef ARDUINO
#include <Arduino.h>
#include <avr/interrupt.h>
#endif

void MQ5_timerSetup()
{
    cli();

    TCCR1A = 0;
    TCCR1B = 0;

    TCNT1 = 0;

    OCR1A = 1562; // 15625 × 0.1 = 1562


    TCCR1B |= (1 << WGM12); //  CTC mode

    TCCR1B |= (1 << CS12) | (1 << CS10); // 1024 pre-scaller

    TIMSK1 |= (1 << OCIE1A); // Enable output compare A

    sei();
}


