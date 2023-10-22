#include "macros.h"
#include "difficulty.h"
#include "leds.h"
#include "random_utils.h"
#include "buttons.h"
#include "level.h"
#include <avr/sleep.h>
#include <TimerOne.h>

#define EI_NOTPORTC
#define EI_NOTPORTB
#define EI_NOTEXTERNAL

#include <EnableInterrupt.h>

const int game_leds[NUM_GAME_LEDS] = {GAME_LED_1, GAME_LED_2, GAME_LED_3, GAME_LED_4};
const int buttons[NUM_BUTTONS] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

// The leds' turning on delta time.
unsigned long t_led = 3000;
// The available delta time to press the buttons.
unsigned long t_btn = 15000;
static bool is_difficulty_chosen;
float game_factor;

static bool game_started;
static bool is_led_sequence_shown;

uint8_t buttons_pressed;
uint8_t *buttons_sequence;
static uint8_t *leds_to_turn_off;

#define DEBUG

void powerdown() {
  disableInterrupt(BUTTON_1);
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    enableInterrupt(buttons[i], wake_up, RISING);
  }
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
#ifdef DEBUG
  Serial.println("Arduino is in PWR down sleep mode.");
#endif
}

void wake_up() {
  sleep_disable();
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    disableInterrupt(buttons[i]);
  }
  enableInterrupt(BUTTON_1, start_game, RISING);
#ifdef DEBUG
    Serial.println("Arduino is awake!");
#endif
}

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
  disableInterrupt(BUTTON_1);
}

static bool are_arrays_equal(uint8_t *a1, uint8_t *a2, uint8_t size) {
  for (uint8_t i = 0; i < size; i++) {
    if (a1[i] != a2[i]) {
      return false;
    }
  }
  return true;
}

static void game_over() {
  led_on(STATUS_LED);
  delay(1000);
  led_off(STATUS_LED);
  Serial.println("GAME OVER!");
}

void setup() {
  for(int i = 0; i < NUM_GAME_LEDS; i++) {
    pinMode(game_leds[i], OUTPUT);
  }
  for(int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT);
  }
  buttons_sequence = (uint8_t *) malloc(NUM_BUTTONS * sizeof(uint8_t));
  is_difficulty_chosen = false;
  game_started = false;
  is_led_sequence_shown = false;

  Serial.begin(9600);
  rand_init();
  
  Serial.println("Welcome to the Restore the Light Game. Press Key B1 to Start");
  enableInterrupt(BUTTON_1, start_game, RISING);
  Timer1.initialize(10000000); // Fire An Interrupt Every 10 sec.
  Timer1.attachInterrupt(powerdown);
  while (!game_started) {
    led_fade(STATUS_LED, 15);
  }
  Timer1.detachInterrupt();
}

void loop() {
  if (game_started) {
    if (!is_difficulty_chosen) {
      view_difficulties();
    } else {
      if (!is_led_sequence_shown) {
        // Game start
        Serial.println("Go!");
        // Turning on all leds.
        for (uint8_t i = 0; i < NUM_GAME_LEDS; i++) {
          led_on(game_leds[i]);
        }
        // Turning off the leds in a random sequence.
        delay(2000);
        leds_to_turn_off = get_rand_multiple(4);
        for (uint8_t i = 0; i < NUM_GAME_LEDS; i++) {
          led_off(game_leds[leds_to_turn_off[i]]);
          delay(t_led);
        }
      } else {
        Timer1.initialize(t_btn);
        Timer1.attachInterrupt(game_over);
        enableInterrupt(BUTTON_1, button1_push, RISING);
        enableInterrupt(BUTTON_2, button2_push, RISING);
        enableInterrupt(BUTTON_3, button3_push, RISING);
        enableInterrupt(BUTTON_4, button4_push, RISING);
        if (buttons_pressed == NUM_GAME_LEDS) {
          if (are_arrays_equal(leds_to_turn_off, buttons_sequence, NUM_BUTTONS)) {
            next_level();
          } else {
            game_over();
          }
        }
      }
    }
      
      free_rand_array(leds_to_turn_off);
  }
}