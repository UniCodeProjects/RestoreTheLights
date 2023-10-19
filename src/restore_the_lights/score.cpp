#include <inttypes.h>
#include "score.h"
#include "Arduino.h"

static uint16_t score = 0;

void increase_score() {
    score++;
}

void print_score_to_serial() {
    char buffer[64];
    sprintf(buffer, "New point! Score: %" PRIu16 "", score);
    Serial.println(buffer);
}
