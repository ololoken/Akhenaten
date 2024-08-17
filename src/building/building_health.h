#pragma once

#include "building/building.h"

class building_apothecary : public building_impl {
public:
    BUILDING_METAINFO(BUILDING_APOTHECARY, building_apothecary)

    building_apothecary(building &b) : building_impl(b) {}
    virtual building_apothecary *dcast_apothecary() override { return this; }

    virtual void spawn_figure() override;
    virtual e_overlay get_overlay() const override { return OVERLAY_APOTHECARY; }
    virtual e_sound_channel_city sound_channel() const override { return SOUND_CHANNEL_CITY_APOTHECARY; }
    virtual int animation_speed(int speed) const override { return 3; }
    virtual bool draw_ornaments_and_animations_height(painter &ctx, vec2i point, tile2i tile, color color_mask) override;
    virtual void update_graphic() override;
};

class building_mortuary : public building_impl {
public:
    BUILDING_METAINFO(BUILDING_MORTUARY, building_mortuary)

    building_mortuary(building &b) : building_impl(b) {}

    virtual void spawn_figure() override;
    virtual e_overlay get_overlay() const override { return OVERLAY_MORTUARY; }
    virtual e_sound_channel_city sound_channel() const override { return SOUND_CHANNEL_CITY_MORTUARY; }
    virtual int animation_speed(int speed) const override { return 3; }
    virtual bool draw_ornaments_and_animations_height(painter &ctx, vec2i point, tile2i tile, color color_mask) override;
    virtual void update_graphic() override;
    virtual void update_count() const override;
};

void building_health_draw_info(object_info &c, e_figure_type ftype);