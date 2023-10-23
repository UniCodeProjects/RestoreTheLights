#include <inttypes.h>
#include "score.h"
#include "Arduino.h"

static uint16_t score = 0;

void increase_score() {
    score++;
}

void reset_score() {
    score = 0;
}

void print_score_to_serial(const bool gameover = false) {
    char buffer[64];
    const char *msg = gameover ? "Game Over. Final" : "New point!";
    sprintf(buffer, "%s Score: %" PRIu16 "", msg, score);
    Serial.println(buffer);
}
