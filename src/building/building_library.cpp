#include "building_library.h"

#include "building/building_education.h"
#include "js/js_game.h"

#include "building/count.h"

#include "city/labor.h"

struct scribal_school_model : public buildings::model_t<building_library> {
} library_m;

ANK_REGISTER_CONFIG_ITERATOR(config_load_library);
void config_load_library() {
    library_m.load();
}

void building_library::window_info_background(object_info &c) {
    building_education_draw_info(c, "library", FIGURE_LIBRARIAN, RESOURCE_NONE, vec2i{0, 0}, vec2i{0, 0});
}

void building_library::spawn_figure() {
    common_spawn_roamer(FIGURE_LIBRARIAN, 50, FIGURE_ACTION_125_ROAMING);
    check_labor_problem();
    //    if (has_figure_of_type(FIGURE_LIBRARIAN))
    //        return;
    //    map_point road;
    //    if (map_has_road_access(x, y, size, &road)) {
    //        spawn_labor_seeker(50);
    //        int spawn_delay = figure_spawn_timer();
    //        if (spawn_delay == -1)
    //            return;
    //        figure_spawn_delay++;
    //        if (figure_spawn_delay > spawn_delay) {
    //            figure_spawn_delay = 0;
    //            create_roaming_figure(road.x, road.y, FIGURE_LIBRARIAN);
    //        }
    //    }
}
