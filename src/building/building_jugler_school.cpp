#include "building_jugler_school.h"

#include "building/building_dance_school.h"
#include "figuretype/figure_entertainer.h"
#include "city/labor.h"
#include "widget/city/ornaments.h"
#include "building/count.h"
#include "graphics/graphics.h"
#include "graphics/image.h"

#include "js/js_game.h"

buildings::model_t<building_juggler_school> juggler_school_m;

ANK_REGISTER_CONFIG_ITERATOR(config_load_building_juggler_school);
void config_load_building_juggler_school() {
    juggler_school_m.load();
}

void building_juggler_school::window_info_background(object_info &c) {
    building_entertainment_school_draw_info(c, "juggler_school", 77);
}

void building_juggler_school::update_month() {
    data.entertainment.spawned_entertainer_this_month = false;
}

void building_juggler_school::spawn_figure() {
    if (!common_spawn_figure_trigger(50)) {
        return;
    }

    if (data.entertainment.spawned_entertainer_this_month) {
        return;
    }

    int venue_destination = figure_entertainer::determine_venue_destination(base.road_access, {BUILDING_PAVILLION, BUILDING_BANDSTAND, BUILDING_BOOTH});
    building* dest = building_get(venue_destination);
    if (dest->id > 0) {
        create_figure_with_destination(FIGURE_JUGGLER, dest, FIGURE_ACTION_92_ENTERTAINER_GOING_TO_VENUE);
        data.entertainment.spawned_entertainer_this_month = true;
    } else {
        common_spawn_roamer(FIGURE_JUGGLER, 50, FIGURE_ACTION_90_ENTERTAINER_AT_SCHOOL_CREATED);
    }
}

bool building_juggler_school::draw_ornaments_and_animations_height(painter &ctx, vec2i point, tile2i tile, color color_mask) {
    const auto &anim = juggler_school_m.anim["work"];
    building_draw_normal_anim(ctx, point + vec2i{104, 0}, &base, tile, image_id_from_group(GROUP_DANCERS_SHOW) - 1, color_mask);

    return true;
}

void building_juggler_school::update_count() const {
    building_increase_type_count(type(), num_workers() > 0);
}
