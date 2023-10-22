#include "game_status.h"
#include "macros.h"
#include "leds.h"
#include "difficulty.h"
#include "random_utils.h"
#include <avr/sleep.h>
#include <Arduino.h>

#define EI_NOTPORTC
#define EI_NOTPORTB
#define EI_NOTEXTERNAL

#include <EnableInterrupt.h>

static status current_status = WAITING;

const int game_leds[NUM_GAME_LEDS] = {GAME_LED_1, GAME_LED_2, GAME_LED_3, GAME_LED_4};
const int buttons[NUM_BUTTONS] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

float game_factor;
// The leds' turning on delta time.
unsigned long t_led = 3000;
// The available delta time to press the buttons.
unsigned long t_btn = 15000;

unsigned long prev_ms = 0;

static void choose_difficulty() {
  uint8_t chosen_difficulty = get_chosen_difficulty();
  game_factor = get_difficulty_factor(chosen_difficulty);
  led_off(get_corresponding_led(chosen_difficulty));
  disableInterrupt(BUTTON_1);
  update_game_status(LEDS_ON);
}

static void start_game() {
  led_off(STATUS_LED);
  disableInterrupt(BUTTON_1);
  enableInterrupt(BUTTON_1, choose_difficulty, RISING);
  Serial.println("Choose difficulty:");
  update_game_status(SELECTING);
}

void game_setup() {
  for(int i = 0; i < NUM_GAME_LEDS; i++) {
    pinMode(game_leds[i], OUTPUT);
  }
  for(int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT);
  }
  Serial.begin(9600);
  rand_init();
  Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
  enableInterrupt(BUTTON_1, start_game, RISING);
}

status get_game_status() {
  return current_status;
}

void update_game_status(const status new_status) {
  current_status = new_status;
}

void waiting() {
  const unsigned long curr_ms = millis();
  if (curr_ms - prev_ms >= 10000) {
    prev_ms = curr_ms;
    update_game_status(SLEEPING);
    return;
  }
  led_fade(STATUS_LED, 15);
}

void selecting() {
  view_difficulties();
}

void leds_on() {
  Serial.println("Go!");
  // Turning on all leds.
  for (uint8_t i = 0; i < NUM_GAME_LEDS; i++) {
    led_on(game_leds[i]);
  }
  // Turning off the leds in a random sequence.
  delay(2000);
  uint8_t *leds_to_turn_off = get_rand_multiple(4);
  for (uint8_t i = 0; i < NUM_GAME_LEDS; i++) {
      led_off(game_leds[leds_to_turn_off[i]]);
      delay(t_led);
  }
  free_rand_array(leds_to_turn_off);
}

void pressing() {

}

void game_end() {

}

void wake_up() {
}

void sleeping() {
  disableInterrupt(BUTTON_1);
  enableInterrupt(BUTTON_1, wake_up, RISING);
  enableInterrupt(BUTTON_2, wake_up, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_3), wake_up, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_4), wake_up, RISING);
  Serial.println("sleeping");
  Serial.flush();
  digitalWrite(STATUS_LED, LOW);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  sleep_disable();
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    disableInterrupt(buttons[i]);
  }
  enableInterrupt(BUTTON_1, start_game, RISING);
  update_game_status(WAITING);
}
