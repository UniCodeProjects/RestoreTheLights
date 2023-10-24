#include <math.h>
#include "level.h"

#define LED_THRESHOLD_MS 100
#define BTN_THRESHOLD_MS 2000

extern float game_factor;

extern unsigned long t_led;
extern unsigned long t_btn;

static bool led_reached_threshold = false;
static bool btn_reached_threshold = false;

void next_level() {
    if (led_reached_threshold || btn_reached_threshold) {
        return;
    } else if (t_led <= LED_THRESHOLD_MS) {
        t_led = LED_THRESHOLD_MS;
        led_reached_threshold = true;
    } else if (t_btn <= BTN_THRESHOLD_MS) {
        t_btn = BTN_THRESHOLD_MS;
        btn_reached_threshold = true;
    } else {
        t_led = abs(lround(t_led * game_factor));
        t_btn = abs(lround(t_btn * game_factor));
    }
}
