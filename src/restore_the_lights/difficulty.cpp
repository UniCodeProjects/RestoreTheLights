#include "difficulty.h"
#include "macros.h"
#include "leds.h"
#include <Arduino.h>

#define LEVELS 4
#define POT_VALUES 1024

enum difficulties {
  EASY = 1,
  NORMAL,
  MEDIUM,
  HARD
};

static const uint16_t levels_range = POT_VALUES / LEVELS;
static uint8_t current_difficulty = 0;

static char* get_difficulty_name(const uint8_t difficulty_value) {
  char* difficulty_name;
  switch(difficulty_value) {
    case EASY: 
      difficulty_name = "Easy";
      break;
    case NORMAL: 
      difficulty_name = "Normal";
      break;
    case MEDIUM: 
      difficulty_name = "Medium";
      break;
    case HARD: 
      difficulty_name = "Hard";
      break;
    default:
      difficulty_name = "Unknown";
  }
  return difficulty_name;
}

uint8_t get_difficulty(const uint16_t value) {
  uint8_t ret_val;
  if (value < EASY * levels_range) {
    ret_val = EASY;
  }
  else if (value >= EASY * levels_range && value < NORMAL * levels_range) {
    ret_val = NORMAL;
  }
  else if (value >= NORMAL * levels_range && value < MEDIUM * levels_range) {
    ret_val = MEDIUM;
  }
  else {
    ret_val = HARD;
  }
  return ret_val;
}

float get_difficulty_factor(const uint8_t difficulty) {
  float factor;
  switch(difficulty) {
    case EASY:
      factor = 1.0;
      break;
    case NORMAL:
      factor = 0.8;
      break;
    case MEDIUM:
      factor = 0.6;
      break;
    case HARD:
      factor = 0.5;
      break;
  }
  return factor;
}

uint8_t get_corresponding_led(const uint8_t difficulty) {
  uint8_t led;
  switch(difficulty) {
    case EASY:
      led = GAME_LED_1;
      break;
    case NORMAL:
      led = GAME_LED_2;
      break;
    case MEDIUM:
      led = GAME_LED_3;
      break;
    case HARD:
      led = GAME_LED_4;
      break;
  }
  return led;
}

void view_difficulties() {
  int current_value = analogRead(POT_PIN);
  uint8_t actual_difficulty = get_difficulty(current_value);
  if (current_difficulty != actual_difficulty) {
    if (current_difficulty != 0) { // current_difficulty is 0 the first time that this method is called
      led_off(get_corresponding_led(current_difficulty));
    }
    led_on(get_corresponding_led(actual_difficulty));
    Serial.println("Chosen difficulty: " + String(get_difficulty_name(actual_difficulty)));
    current_difficulty = actual_difficulty;
  }
}

uint8_t get_chosen_difficulty() {
  return current_difficulty;
}

void reset_game_difficulty() {
  current_difficulty = 0;
}
