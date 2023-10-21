#include <stdint.h>
#include "Arduino.h"
#include "leds.h"

#define ARDUINO_UNO

// Led-related variables.
uint8_t led_state;
uint8_t brightness = 0;
int8_t fade_amount = 5;

// Delay without busy waiting variables.
unsigned long prev_ms_1 = 0;
unsigned long prev_ms_2 = 0;

// Utility functions declarations.
#ifdef ARDUINO_UNO
bool is_pwm(const uint8_t pin);
#endif

/********************************************************************/
/*                                                                  */
/*                    Functions from header file                    */
/*                                                                  */
/********************************************************************/

void led_on(const uint8_t pin) {
  digitalWrite(pin, HIGH);
  led_state = HIGH;
}

void led_off(const uint8_t pin) {
  digitalWrite(pin, LOW);
  led_state = LOW;
}

void led_blink(const uint8_t pin, const unsigned long delay_ms) {
  // Without delay (busy-waiting).
  const unsigned long current_ms = millis();
  if (current_ms - prev_ms_1 >= delay_ms) {
    prev_ms_1 = current_ms;
    // Actual code.
    if (led_state == LOW) {
      led_on(pin);
    } else {
      led_off(pin);
    }
  }
}

void led_fade(const uint8_t pin, const unsigned long delay_ms) {
#ifdef ARDUINO_UNO
  if (!is_pwm(pin)) {
    return;
  }
#endif
  // Without delay (busy-waiting).
  const unsigned long current_ms = millis();
  if (current_ms - prev_ms_2 >= delay_ms) {
    prev_ms_2 = current_ms;
    // Actual code.
    analogWrite(pin, brightness);
    brightness += fade_amount;
    if (brightness <= 0 || brightness >= 255) {
      fade_amount = -fade_amount;
    }
  }
}

// Utility functions.
#ifdef ARDUINO_UNO
bool is_pwm(const uint8_t pin) {
  const uint8_t pwm_pins_arduino_uno[6] = {3, 5, 6, 9, 10, 11};
  for (uint8_t i = 0; i < sizeof(pwm_pins_arduino_uno); i++) {
    if (pin == pwm_pins_arduino_uno[i]) {
      return true;
    }
  }
  return false;
}
#endif
