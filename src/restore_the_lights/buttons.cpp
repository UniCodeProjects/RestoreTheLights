#include <Arduino.h>
#include <inttypes.h>
#include "buttons.h"
#include "macros.h"
#include "leds.h"

enum buttons_index {
  BTN_1_INDEX,
  BTN_2_INDEX,
  BTN_3_INDEX,
  BTN_4_INDEX
};

extern uint8_t *user_button_sequence;
extern uint8_t num_buttons_pressed;

static bool button_already_pressed(const uint8_t btn_index) {
  uint8_t count = 0;
  for (uint8_t i = 0; i < num_buttons_pressed; i++) {
    if (user_button_sequence[i] == btn_index) {
      count++;
    }
  }
  return num_buttons_pressed != 0 ? count > 0 : false;
}

static void press_button(const uint8_t btn_index) {
  if (!button_already_pressed(btn_index)) {
    user_button_sequence[num_buttons_pressed] = btn_index;
    num_buttons_pressed++;
  }
}

void button1_push() {
  led_on(GAME_LED_1);
  press_button(BTN_1_INDEX);
}

void button2_push() {
  led_on(GAME_LED_2);
  press_button(BTN_2_INDEX);
}

void button3_push() {
  led_on(GAME_LED_3);
  press_button(BTN_3_INDEX);
}

void button4_push() {
  led_on(GAME_LED_4);
  press_button(BTN_4_INDEX);
}
