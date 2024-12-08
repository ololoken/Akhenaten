#include "city_overlay_health.h"

#include "grid/property.h"
#include "grid/building.h"
#include "figure/figure.h"

city_overlay_health g_city_overlay_health;

city_overlay* city_overlay_for_health() {
    return &g_city_overlay_health;
}

int city_overlay_health::get_column_height(const building *b) const {
    if (!b->house_size || b->house_population <= 0) {
        return COLUMN_TYPE_NONE;
    }

    if (b->disease_days > 0) {
        return 10;
    }

    return b->common_health / 10;
}

e_column_color city_overlay_health::get_column_color(const building *b) const {
    if (!b->house_size || b->house_population <= 0) {
        return COLUMN_COLOR_NONE;
    }

    if (b->disease_days > 0) {
        return COLUMN_COLOR_RED;
    }
}

xstring city_overlay_health::get_tooltip_for_building(tooltip_context *c, const building *b) const {
    if (b->disease_days > 0) {
        return ui::str(66, 131);
    }

    if (b->common_health < 25) {
        return ui::str(66, 130);
    } else if (b->common_health < 50) {
        return ui::str(66, 129);
    } else if (b->common_health < 75) {
        return ui::str(66, 128);
    } else {
        return ui::str(66, 127);
    }
}
