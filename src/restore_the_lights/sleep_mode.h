#ifndef SLEEP_MODE_H_
#define SLEEP_MODE_H_

#include <inttypes.h>

// Attaches the given pin to the interrupt.
void set_interrupt_pin(const uint8_t pin);

// Enters the power down sleep mode.
void powerdown();

// Wakes Arduino up from sleep.
void wake_up();

#endif
