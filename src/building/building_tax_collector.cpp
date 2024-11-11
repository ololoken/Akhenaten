#include "building_tax_collector.h"

#include "building/building.h"
#include "city/object_info.h"
#include "figure/figure.h"
#include "game/resource.h"
#include "city/finance.h"
#include "graphics/window.h"
#include "graphics/elements/arrow_button.h"
#include "graphics/elements/panel.h"
#include "graphics/view/view.h"
#include "graphics/elements/lang_text.h"
#include "graphics/graphics.h"
#include "io/gamefiles/lang.h"
#include "config/config.h"
#include "window/building/common.h"
#include "sound/sound_building.h"
#include "building/count.h"
#include "game/game.h"
#include "city/labor.h"
#include "widget/city/ornaments.h"
#include "js/js_game.h"

buildings::model_t<building_tax_collector> btax_collector_m;
buildings::model_t<building_tax_collector_up> btax_collector_up_m;

void building_tax_collector::spawn_figure() {
    if (!has_road_access()) {
        return;
    }

    check_labor_problem();
    if (has_figure_of_type(BUILDING_SLOT_SERVICE, FIGURE_TAX_COLLECTOR)) {
        return;
    }

    common_spawn_labor_seeker(50);

    int pct_workers = worker_percentage();
    int spawn_delay;
    if (pct_workers >= 100) {
        spawn_delay = 0;
    } else if (pct_workers >= 75) {
        spawn_delay = 1;
    } else if (pct_workers >= 50) {
        spawn_delay = 3;
    } else if (pct_workers >= 25) {
        spawn_delay = 7;
    } else if (pct_workers >= 1) {
        spawn_delay = 15;
    } else {
        return;
    }

    base.figure_spawn_delay++;
    if (base.figure_spawn_delay > spawn_delay) {
        base.figure_spawn_delay = 0;
        create_roaming_figure(FIGURE_TAX_COLLECTOR, FIGURE_ACTION_40_TAX_COLLECTOR_CREATED, BUILDING_SLOT_SERVICE);
    }
}

void building_tax_collector::update_month() {
    if (!config_get(CONFIG_GP_CH_NEW_TAX_COLLECTION_SYSTEM)) {
        return;
    }

    if (base.has_figure_of_type(BUILDING_SLOT_CARTPUSHER, FIGURE_CART_PUSHER)) {
        return;
    }

    if (base.has_road_access && base.deben_storage > 100) {
        int may_send = std::min<int>((base.deben_storage / 100) * 100, 400);
        figure *f = base.create_cartpusher(RESOURCE_GOLD, may_send, FIGURE_ACTION_20_CARTPUSHER_INITIAL, BUILDING_SLOT_CARTPUSHER);
        base.deben_storage -= may_send;
        f->sender_building_id = base.id;
    }
}

void building_tax_collector::update_graphic() {
    const xstring &animkey = can_play_animation() ? animkeys().work : animkeys().none;
    set_animation(animkey);

    building_impl::update_graphic();
}

const building_impl::static_params &building_tax_collector::params() const {
    return (type() == BUILDING_TAX_COLLECTOR) 
                ? *(static_params*)&btax_collector_m
                : *(static_params*)&btax_collector_up_m;
}