#include "leds_copy.h"

// #define ON_OFF
// #define BLINK
// #define BLINK_PWM
// #define FADE
#define BLINK_FADE

#define LED 12
#define LED_PWM 6

#define DELAY_MS 1000

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(LED_PWM, OUTPUT);
}

void loop() {
#ifdef ON_OFF
  led_on(LED);
  led_on(LED_PWM);
  delay(DELAY_MS);
  led_off(LED);
  led_off(LED_PWM);
  delay(DELAY_MS);
#endif
#ifdef BLINK
  led_blink(LED, 500);
#endif
#ifdef BLINK_PWM
  led_blink(LED_PWM, 500);
#endif
#ifdef FADE
  led_fade(LED_PWM, 20);
#endif
#ifdef BLINK_FADE
  led_blink(LED, 500);
  led_fade(LED_PWM, 20);
#endif
}
