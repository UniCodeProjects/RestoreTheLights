#include "game_status.h"
#include "macros.h"
#include "buttons.h"
#include "leds.h"
#include "difficulty.h"
#include "random_utils.h"
#include "level.h"
#include "score.h"
#include <avr/sleep.h>
#include <Arduino.h>
#include <stdlib.h>

#define EI_NOTPORTC
#define EI_NOTPORTB
#define EI_NOTEXTERNAL

#include <EnableInterrupt.h>

#define INIT_T_LED_VAL 3000
#define INIT_T_BTN_VAL 15000

static status current_status = WAITING;

const int game_leds[NUM_GAME_LEDS] = {GAME_LED_1, GAME_LED_2, GAME_LED_3, GAME_LED_4};
const int buttons[NUM_BUTTONS] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

float game_factor;
// The leds' turning on delta time.
unsigned long t_led = INIT_T_LED_VAL;
// The available delta time to press the buttons.
unsigned long t_btn = INIT_T_BTN_VAL;

uint8_t *correct_button_sequence;
uint8_t *user_button_sequence;
uint8_t num_buttons_pressed;
static bool is_pressing_started;
static unsigned long prev_ms = 0;
unsigned long start_level_time = 0;

const uint16_t DEBOUNCE_DELAY = 50; // the debounce time; increase if the output flickers

static bool debounce(const uint8_t btn_pin) {
  static uint32_t last_debounce_time = 0;  // the last time the output pin was toggled
  static uint8_t button_state = HIGH;     // the current reading from the input pin
  static uint8_t last_button_state = HIGH; // the previous reading from the input pin

  uint8_t reading = digitalRead(btn_pin);

  // If the button state has changed:
  if (reading != last_button_state) {
    // Reset the debouncing timer
    last_debounce_time = millis();
  }

  if ((millis() - last_debounce_time) > DEBOUNCE_DELAY) {
    // If the button state has changed:
    if (reading != button_state) {
      button_state = reading;

      // Only toggle the LED if the new button state is HIGH
      if (button_state == HIGH) {
        return true;
      }
    }
  }
  return false;
}


static void choose_difficulty() {
  uint8_t chosen_difficulty = get_chosen_difficulty();
  game_factor = get_difficulty_factor(chosen_difficulty);
  led_off(get_corresponding_led(chosen_difficulty));
  disableInterrupt(BUTTON_1);
  update_game_status(LEDS_ON);
}

static void start_game() {
  Serial.println(debounce(BUTTON_1));
  if (debounce(BUTTON_1)) {
    led_off(STATUS_LED);
    disableInterrupt(BUTTON_1);
    enableInterrupt(BUTTON_1, choose_difficulty, RISING);
    Serial.println("Choose difficulty:");
    update_game_status(SELECTING);
  }
}

static void reset_game_variables() {
  correct_button_sequence = (uint8_t *) malloc(NUM_BUTTONS * sizeof(uint8_t));
  user_button_sequence = (uint8_t *) malloc(NUM_BUTTONS * sizeof(uint8_t));
  t_led = INIT_T_LED_VAL;
  t_btn = INIT_T_BTN_VAL;
  num_buttons_pressed = 0;
  is_pressing_started = false;
  reset_score();
  prev_ms = millis();
  enableInterrupt(BUTTON_1, start_game, RISING);
}

static void reset_game_variables_for_next_level() {
  correct_button_sequence = (uint8_t *) malloc(NUM_BUTTONS * sizeof(uint8_t));
  user_button_sequence = (uint8_t *) malloc(NUM_BUTTONS * sizeof(uint8_t));
  num_buttons_pressed = 0;
  is_pressing_started = false;
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
  correct_button_sequence = (uint8_t *) malloc(NUM_BUTTONS * sizeof(uint8_t));
  user_button_sequence = (uint8_t *) malloc(NUM_BUTTONS * sizeof(uint8_t));
  num_buttons_pressed = 0;
  is_pressing_started = false;
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
  // Initializing correct_button_sequence as the reverse of leds_to_turn_off
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    correct_button_sequence[i] = leds_to_turn_off[NUM_BUTTONS - i - 1];
  }

  for (uint8_t i = 0; i < NUM_GAME_LEDS; i++) {
      led_off(game_leds[leds_to_turn_off[i]]);
      if (i == NUM_GAME_LEDS - 1) {
        break;
      }
      delay(t_led);
  }
  free_rand_array(leds_to_turn_off);
  update_game_status(PRESSING);
}

static void game_over() {
  disableInterrupt(BUTTON_1);
  disableInterrupt(BUTTON_2);
  detachInterrupt(digitalPinToInterrupt(BUTTON_3));
  detachInterrupt(digitalPinToInterrupt(BUTTON_4));
  led_on(STATUS_LED);
  delay(1000);
  led_off(STATUS_LED);
  for (uint8_t i = 0; i < NUM_GAME_LEDS; i++) {
    led_off(game_leds[i]);
  }
  print_score_to_serial(true);
  delay(10000);
  reset_game_variables();
  enableInterrupt(BUTTON_1, start_game, RISING);
  update_game_status(WAITING);
}

void pressing() {
  if (!is_pressing_started) {
    Serial.println("pressing initialization");
    enableInterrupt(BUTTON_1, button1_push, RISING);
    enableInterrupt(BUTTON_2, button2_push, RISING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_3), button3_push, RISING);
    attachInterrupt(digitalPinToInterrupt(BUTTON_4), button4_push, RISING);
    is_pressing_started = true;
    start_level_time = millis();
  }

  if (num_buttons_pressed == NUM_BUTTONS) {
    disableInterrupt(BUTTON_1);
    disableInterrupt(BUTTON_2);
    detachInterrupt(digitalPinToInterrupt(BUTTON_3));
    detachInterrupt(digitalPinToInterrupt(BUTTON_4));
    update_game_status(GAME_END);
  } else {
    if (millis() - start_level_time >= t_btn) {
      update_game_status(GAME_END);
    }
  }
}

static bool are_arrays_equal(uint8_t *a1, uint8_t* a2, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    if (a1[i] != a2[i]) {
      return false;
    }
  }
  return true;
}

void game_end() {
  // the second operand of the && operator is needed to handle the game over in case t_btn expires
  bool is_victory = are_arrays_equal(correct_button_sequence, user_button_sequence, NUM_BUTTONS) 
    && num_buttons_pressed == NUM_BUTTONS;
  free(user_button_sequence);
  free(correct_button_sequence);
  if (is_victory) {
    increase_score();
    print_score_to_serial();
    update_game_status(LEDS_ON);
    reset_game_variables_for_next_level();
    next_level();
  } else {
    game_over();
  }
}

void wake_up() {
  if (debounce(BUTTON_1) && debounce(BUTTON_2) && debounce(BUTTON_3) && debounce(BUTTON_4)) {
  }
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
  disableInterrupt(BUTTON_1);
  disableInterrupt(BUTTON_2);
  detachInterrupt(digitalPinToInterrupt(BUTTON_3));
  detachInterrupt(digitalPinToInterrupt(BUTTON_4));
  enableInterrupt(BUTTON_1, start_game, RISING);
  update_game_status(WAITING);
}

static bool button_already_pressed(const uint8_t btn_index) {
  uint8_t count = 0;
  for (uint8_t i = 0; i < num_buttons_pressed; i++) {
    if (user_button_sequence[i] == btn_index) {
      count++;
    }
  }
  return num_buttons_pressed != 0 ? count > 0 : false;
}

void press_button(const uint8_t btn_index) {
  if (!button_already_pressed(btn_index)) {
    user_button_sequence[num_buttons_pressed] = btn_index;
    num_buttons_pressed++;
  }
}