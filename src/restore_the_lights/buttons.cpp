#include <Arduino.h>
#include <inttypes.h>
#include "macros.h"
#include "level.h"
#include "leds.h"
#include "game_status.h"

extern unsigned long last_button_press;

void button1_push() {
  led_on(GAME_LED_1);
  press_button(0);
  last_button_press = millis();
}

void button2_push() {
  led_on(GAME_LED_2);
  press_button(1);
  last_button_press = millis();
}

void button3_push() {
  led_on(GAME_LED_3);
  press_button(2);
  last_button_press = millis();
}

void button4_push() {
  led_on(GAME_LED_4);
  press_button(3);
  last_button_press = millis();
}
