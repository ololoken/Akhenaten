#include "core/system_time.h"

static time_millis current_time;

time_millis time_get_millis(void) {
    return current_time;
}

void time_set_millis(time_millis millis) {
    current_time = millis;
}
