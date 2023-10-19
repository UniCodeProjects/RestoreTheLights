#include <avr/sleep.h>
#include "Arduino.h"
#include "sleep_mode.h"

static void handle_awakening() {}

void set_interrupt_pin(const uint8_t pin) {
    attachInterrupt(digitalPinToInterrupt(pin), handle_awakening, RISING);
}

void powerdown() {
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    sleep_mode();
}

void wake_up() {
    sleep_disable();
}
