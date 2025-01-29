#pragma once

#include "building/building_entertainment.h"

class building_pavilion : public building_entertainment {
public:
    BUILDING_METAINFO(BUILDING_PAVILLION, building_pavilion)
    building_pavilion(building &b) : building_entertainment(b) {}

    struct static_params : public buildings::model_t<building_pavilion> {
        using inherited = buildings::model_t<building_pavilion>;

        int dancer_tile = 0;
        int booth_tile = 0;
        int musician_tile_s = 0;
        int musician_tile_e = 0;

        struct preview_offset {
            vec2i stand, stand_b, stand_e, booth;
            int stand_b_img = 0, stand_e_img = 0;
            void load(archive arch, pcstr section);
        };

        struct place_offset {
            struct item {
                e_building_type type = BUILDING_NONE;
                vec2i offset;
                bool main;
                void load(archive arch);
            };
            svector<item, 8> items;
            void load(archive arch, pcstr section);
        };

        preview_offset preview_dir[8];
        place_offset place_dir[8];

        using inherited::load;
        virtual void load(archive arch) override;
        virtual void setup_preview_graphics(build_planner &planer) const;
    };
    
    virtual void on_create(int orientation) override;
    virtual void update_day() override;
    virtual void update_month() override;
    virtual void on_place_update_tiles(int orientation, int variant) override;
    virtual void on_place_checks() override;
    virtual building_pavilion *dcast_pavilion() override { return this; }
    virtual e_sound_channel_city sound_channel() const override { return SOUND_CHANNEL_CITY_PAVILION; }
    virtual bool draw_ornaments_and_animations_height(painter &ctx, vec2i point, tile2i tile, color color_mask) override;
    virtual void on_undo() override;

    virtual void update_count() const override;
    virtual void spawn_figure() override;

    static void ghost_preview(painter &ctx, tile2i tile, vec2i pixel, int orientation);

    static const static_params &current_params() { return (const static_params &)params(TYPE); }
};