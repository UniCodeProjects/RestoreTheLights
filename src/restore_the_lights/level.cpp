#include <assert.h>
#include "level.h"

extern float game_factor;

extern float t_led;
extern float t_btn;

void next_level() {
    t_led *= game_factor;
    t_btn *= game_factor;
}
