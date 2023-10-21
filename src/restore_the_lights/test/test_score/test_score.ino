#include "score_copy.h"

void setup() {
    Serial.begin(9600);
}

void loop() {
    increase_score();
    print_score_to_serial();
    delay(1000);
}
