#include "macros.h"
#include "difficulty.h"
#include "leds.h"
#include "random_utils.h"
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
static bool is_interrupt_detached;
float game_factor;

static bool game_started;

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
      if (!is_interrupt_detached) {
        disableInterrupt(BUTTON_1);
        is_interrupt_detached = true;
      }
      // Game start
      Serial.println("Go!");
      // Turning on all leds.
      for (uint8_t i = 0; i < NUM_GAME_LEDS; i++) {
        led_on(game_leds[i]);
      }
      // Turning off the leds in a random sequence.
      delay(2000);
      unsigned long *leds_to_turn_off = get_rand_multiple(4);
      for (uint8_t i = 0; i < NUM_GAME_LEDS; i++) {
        led_off(game_leds[leds_to_turn_off[i]]);
        delay(1000); // DEFINE T1.
      }
      
      free_rand_array(leds_to_turn_off);
    }
  }
}
