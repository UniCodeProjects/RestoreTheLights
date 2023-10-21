#ifndef LEDS_H_
#define LEDS_H_

#include <inttypes.h>

// Turns the led connected to 'pin' on.
void led_on(const uint8_t pin);

// Turns the led connected to 'pin' off.
void led_off(const uint8_t pin);

// Makes the led connected to 'pin' blink on a given delay.
void led_blink(const uint8_t pin, const unsigned long delay_ms);

// Makes the led connected to 'pin' fade on a given delay.
void led_fade(const uint8_t pin, const unsigned long delay_ms);

#endif
