#include "macros.h"
#include "difficulty.h"
#include "leds.h"
#include "random_utils.h"
#include "leds.h"

#define EI_NOTPORTC
#define EI_NOTPORTB
#define EI_NOTEXTERNAL
#define EI_ARDUINO_INTERRUPTED_PIN

#include <EnableInterrupt.h>

const int game_leds[NUM_GAME_LEDS] = {GAME_LED_1, GAME_LED_2, GAME_LED_3, GAME_LED_4};
const int buttons[NUM_BUTTONS] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

// The leds' turning on delta time.
unsigned long t_led = 3000;
// The available delta time to press the buttons.
unsigned long t_btn = 15000;
static bool is_difficulty_chosen;
static bool is_interrupt_detached;
float game_factor;

static bool game_started;

static void start_game() {
  led_off(STATUS_LED);
  game_started = true;
  disableInterrupt(BUTTON_1);
  enableInterrupt(BUTTON_1, choose_difficulty, RISING);
  Serial.println("Choose difficulty:");
}

static void choose_difficulty() {
  is_difficulty_chosen = true;
  uint8_t chosen_difficulty = get_chosen_difficulty();
  game_factor = get_difficulty_factor(chosen_difficulty);
  led_off(get_corresponding_led(chosen_difficulty));
}

void setup() {
  for(int i = 0; i < NUM_GAME_LEDS; i++) {
    pinMode(game_leds[i], OUTPUT);
  }
  for(int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT);
  }
  is_difficulty_chosen = false;
  is_interrupt_detached = false;
  game_started = false;

  Serial.begin(9600);
  rand_init();

  enableInterrupt(BUTTON_1, start_game, RISING);
  Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
  while (!game_started) {
    led_fade(STATUS_LED, 15);
  }
}

void loop() {
  if (game_started) {
    if (!is_difficulty_chosen) {
      view_difficulties();
    } else {
      if (!is_interrupt_detached) {
        disableInterrupt(BUTTON_1);
        is_interrupt_detached = true;
      }
      // Game start
      led_on(game_leds[0]);
      led_on(game_leds[1]);
      led_on(game_leds[2]);
      led_on(game_leds[3]);
      delay(2000);
      unsigned long *leds_to_turn_off = get_rand_multiple(4);
      led_off(game_leds[leds_to_turn_off[0]]);
      delay(1000);
      led_off(game_leds[leds_to_turn_off[1]]);
      delay(1000);
      led_off(game_leds[leds_to_turn_off[2]]);
      delay(1000);
      led_off(game_leds[leds_to_turn_off[3]]);
      free_rand_array(leds_to_turn_off);
      delay(1000);
    }
  }
}
