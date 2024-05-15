#include "building/building_weaponsmith.h"

#include "building/building_workshop.h"
#include "building/count.h"
#include "graphics/animation.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "widget/city/ornaments.h"

#include "js/js_game.h"
#include "city/labor.h"

#include "dev/debug.h"
#include "city/resource.h"
#include <iostream>

declare_console_command(addweapons, game_cheat_add_resource<RESOURCE_WEAPONS>);

buildings::model_t<building_weaponsmith> weaponsmith_m;

ANK_REGISTER_CONFIG_ITERATOR(config_load_building_weaponsmith);
void config_load_building_weaponsmith() {
    weaponsmith_m.load();
}

void building_weaponsmith::on_create(int orientation) {
    data.industry.first_material_id = RESOURCE_COPPER;
    base.output_resource_first_id = RESOURCE_WEAPONS;
}

void building_weaponsmith::window_info_background(object_info& c) {
    e_resource output_resource = RESOURCE_WEAPONS;
    building_workshop_draw_background(c, 98, "weapons_workshop", 124, output_resource, RESOURCE_COPPER);
}

void building_weaponsmith::window_info_foreground(object_info &ctx) {
    building_workshop_draw_foreground(ctx);
}

bool building_weaponsmith::draw_ornaments_and_animations_height(painter &ctx, vec2i point, tile2i tile, color color_mask) {
    const animation_t &anim = weaponsmith_m.anim["work"];
    building_draw_normal_anim(ctx, point, &base, tile, anim, color_mask);

    int amount = std::min<int>(2, ceil((float)base.stored_amount() / 100.0) - 1);
    if (amount >= 0) {
        const auto &anim = weaponsmith_m.anim["copper"];
        ImageDraw::img_generic(ctx, anim.first_img() + amount, point + anim.pos, color_mask);
    }

    return true;
}

void building_weaponsmith::update_count() const {
    building_increase_industry_count(RESOURCE_WEAPONS, num_workers() > 0);
}
