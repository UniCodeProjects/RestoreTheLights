/* 
 * Restore The Lights - Embedded Systems and Internet-of-Things
 * 
 * A.Y. 2023-2024, 10/24/2023
 * 
 * Authors: Sofia Cipollone, Raffaele Francesco Marrazzo
 */
#include "game_status.h"

void setup() {
  game_setup();
}

void loop() {
  const status current_game_status = get_game_status();
  switch (current_game_status) {
    case WAITING:
      waiting();
      break;
    case SELECTING:
      selecting();
      break;
    case LEDS_ON:
      leds_on();
      break;
    case PRESSING:
      pressing();
      break;
    case GAME_END:
      game_end();
      break;
    case SLEEPING:
      sleeping();
      break;
    default:
      break;
  }
}