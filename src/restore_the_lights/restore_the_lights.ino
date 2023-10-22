#include "macros.h"
#include "difficulty.h"
#include "leds.h"

#define EI_NOTPORTC
#define EI_NOTPORTB
#define EI_NOTEXTERNAL
#define EI_ARDUINO_INTERRUPTED_PIN

#include <EnableInterrupt.h>

// #define CIRCUIT_SAMPLE

#ifdef CIRCUIT_SAMPLE
int current;
#endif

const int game_leds[NUM_GAME_LEDS] = {GAME_LED_1, GAME_LED_2, GAME_LED_3, GAME_LED_4};
const int buttons[NUM_BUTTONS] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

// The leds' turning on delta time.
unsigned long t_led = 3000;
// The available delta time to press the buttons.
unsigned long t_btn = 15000;
static bool is_difficulty_chosen;
static bool is_interrupt_detached;
float game_factor;

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
  
  Serial.begin(9600);

  enableInterrupt(BUTTON_1, choose_difficulty, RISING);
}

void loop() {
  if(!is_difficulty_chosen) {
    view_difficulties();
  }
  else {
    if (!is_interrupt_detached) {
      disableInterrupt(BUTTON_1);
      is_interrupt_detached = true;
      Serial.println("Chosen game factor: " + String(game_factor));
    }
  }
  #ifdef CIRCUIT_SAMPLE
  int buttonState1 = digitalRead(BUTTON_1);
  int buttonState2 = digitalRead(BUTTON_2);
  int buttonState3 = digitalRead(BUTTON_3);
  int buttonState4 = digitalRead(BUTTON_4);

  digitalWrite(STATUS_LED, HIGH);

  if (buttonState1 == HIGH){
    digitalWrite(12, HIGH);
    Serial.println("ON BUTTON1");
    digitalWrite(12, LOW);
  }
  if (buttonState2 == HIGH){
    digitalWrite(11, HIGH);
    Serial.println("ON BUTTON2");
    digitalWrite(11, LOW);
  }
  if (buttonState3 == HIGH){
    digitalWrite(10, HIGH);
    Serial.println("ON BUTTON3");
    digitalWrite(10, LOW);
  }
  if (buttonState4 == HIGH){
    digitalWrite(9, HIGH);
    Serial.println("ON BUTTON4");
    digitalWrite(9, LOW);
  }

  int newValue = analogRead(POT_PIN);
  if (newValue != current) {
    current = newValue;
    Serial.println(current);
 }
 #endif
}
