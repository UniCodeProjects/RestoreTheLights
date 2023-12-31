#ifndef SCORE_H_
#define SCORE_H_

// Increases the current score by one point.
void increase_score();

// Resets the current score to 0.
void reset_score();

// Prints the current score on the serial monitor.
void print_score_to_serial(const bool gameover = false);

#endif
