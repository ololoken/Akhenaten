#include "building_bricklayers_guild.h"

#include "building/building.h"
#include "building/count.h"
#include "building/monuments.h"
#include "city/object_info.h"
#include "city/city.h"
#include "city/labor.h"
#include "core/random.h"
#include "core/calc.h"
#include "figure/figure.h"
#include "game/resource.h"
#include "graphics/elements/panel.h"
#include "graphics/elements/lang_text.h"
#include "graphics/view/view.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/text.h"
#include "io/gamefiles/lang.h"
#include "config/config.h"
#include "window/building/common.h"
#include "window/building/figures.h"
#include "sound/sound_building.h"
#include "widget/city/ornaments.h"
#include "game/game.h"
#include "dev/debug.h"

#include <iostream>

buildings::model_t<building_bricklayers_guild> bricklayers_guild_m;

declare_console_command(addbricks, game_cheat_add_resource<RESOURCE_BRICKS>);

void building_bricklayers_guild::update_graphic() {
    const xstring &animkey = can_play_animation() ? animkeys().work : animkeys().none;
    set_animation(animkey);

    building_impl::update_graphic();
}

void building_bricklayers_guild::on_create(int orientation) {
    data.guild.max_workers = 1;
}

bool building_bricklayers_guild::can_spawn_bricklayer_man(int max_gatherers_per_building) {
    uint32_t total_sites = building_count_total(BUILDING_SMALL_MASTABA);
    uint32_t active_sites = building_count_active(BUILDING_SMALL_MASTABA);
    if (total_sites == active_sites) {
        return false;
    }
    
    bool has_free_man = (base.get_figures_number(FIGURE_BRICKLAYER) < data.guild.max_workers);
    if (!has_free_man) {
        return false;
    }

    return true;
}

void building_bricklayers_guild::spawn_figure() {
    base.check_labor_problem();
    if (!base.has_road_access) {
        return;
    }

    base.common_spawn_labor_seeker(100);
    int pct_workers = base.worker_percentage();
    if (pct_workers < 50) {
        return;
    }

    int spawn_delay = base.figure_spawn_timer();
    if (spawn_delay == -1) {
        return;
    }

    base.figure_spawn_delay++;
    if (base.figure_spawn_delay < spawn_delay) {
        return;
    }

    base.figure_spawn_delay = 0;
    if (!can_spawn_bricklayer_man(data.guild.max_workers)) {
        return;
    }

    building* monument = buildings_valid_first([&] (building &b) {
        if (!b.is_monument() || !building_monument_is_unfinished(&b)) {
            return false;
        }

        if (!b.is_main()) {
            return false;
        }

        return building_monument_need_bricklayers(&b);
    });

    if (!monument) {
        return;
    }

    auto f = base.create_figure_with_destination(FIGURE_BRICKLAYER, monument, FIGURE_ACTION_10_BRIRKLAYER_CREATED, BUILDING_SLOT_SERVICE);
    monument->monument_add_workers(f->id);
    f->wait_ticks = random_short() % 30; // ok
}

bool building_bricklayers_guild::draw_ornaments_and_animations_height(painter &ctx, vec2i point, tile2i tile, color color_mask) {
    building_impl::draw_ornaments_and_animations_height(ctx, point, tile, color_mask);

    return true;
}
