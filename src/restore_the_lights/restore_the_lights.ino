#include "macros.h"

// int current;

enum pins {
  GAME_LED_4 = 9,
  GAME_LED_3,
  GAME_LED_2,
  GAME_LED_1,
  BUTTON_4 = 2,
  BUTTON_3,
  BUTTON_2,
  BUTTON_1,
  STATUS_LED,
} pins;

const int leds[NUM_LEDS] = {GAME_LED_1, GAME_LED_2, GAME_LED_3, GAME_LED_4, STATUS_LED};
const int buttons[NUM_BUTTONS] = {BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4};

// The leds' turning on delta time.
float t_led;
// The available delta time to press the buttons.
float t_btn;

void setup() {
  for(int i = 0; i < NUM_LEDS; i++) {
    pinMode(leds[i], OUTPUT);
  }
  for(int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttons[i], INPUT);
  }
  Serial.begin(9600);
}

void loop() {
  /*
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
 */
}
