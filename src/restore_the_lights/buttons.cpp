#include <Arduino.h>
#include <inttypes.h>
#include "macros.h"
#include "level.h"
#include "leds.h"
#include "game_status.h"

void button1_push() {
  led_on(GAME_LED_1);
  press_button(0);
}

void button2_push() {
  led_on(GAME_LED_2);
  press_button(1);
}

void button3_push() {
  led_on(GAME_LED_3);
  press_button(2);
}

void button4_push() {
  led_on(GAME_LED_4);
  press_button(3);
}
