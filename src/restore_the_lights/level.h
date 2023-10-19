#ifndef LEVEL_H_
#define LEVEL_H_

/*
 * Multiplies the leds and buttons' delta time by the game factor.
 *
 * Global variables (declared in .ino) used by this function:
 * - game_factor: read
 * - t_led: write
 * - t_btn: write
 */
void next_level();

#endif
