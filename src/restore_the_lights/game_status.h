#ifndef GAME_STATUS_H_
#define GAME_STATUS_H_

typedef enum status {
    WAITING,
    SELECTING,
    LEDS_ON,
    PRESSING,
    GAME_END,
    SLEEPING
} status;

void game_setup();

status get_game_status();

void update_game_status(const status new_status);

void waiting();

void selecting();

void leds_on();

void pressing();

void game_end();

void sleeping();

#endif
