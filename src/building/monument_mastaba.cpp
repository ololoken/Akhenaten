#include "monument_mastaba.h"

#include "graphics/view/view.h"
#include "monuments.h"
#include "core/direction.h"
#include "graphics/image.h"
#include "grid/random.h"
#include "grid/tiles.h"
#include "grid/grid.h"
#include "grid/terrain.h"
#include "grid/building.h"
#include "grid/property.h"
#include "grid/image.h"
#include "graphics/view/lookup.h"
#include "graphics/boilerplate.h"

void map_mastaba_tiles_add(int building_id, tile2i tile, int size, int image_id, int terrain) {
    int x_leftmost, y_leftmost;
    switch (city_view_orientation()) {
    case DIR_0_TOP_RIGHT: x_leftmost = 0; y_leftmost = 1; break;
    case DIR_2_BOTTOM_RIGHT: x_leftmost = y_leftmost = 0; break;
    case DIR_4_BOTTOM_LEFT: x_leftmost = 1; y_leftmost = 0; break;
    case DIR_6_TOP_LEFT: x_leftmost = y_leftmost = 1; break;
    default:
        return;
    }

    if (!map_grid_is_inside(tile, size)) {
        return;
    }

    int x_proper = x_leftmost * (size - 1);
    int y_proper = y_leftmost * (size - 1);
    //int clear_land_id = image_id_from_group(GROUP_TERRAIN_EMPTY_LAND);
    for (int dy = 0; dy < size; dy++) {
        for (int dx = 0; dx < size; dx++) {
            int grid_offset = tile.shifted(dx, dy).grid_offset();
            map_terrain_remove(grid_offset, TERRAIN_CLEARABLE);
            map_terrain_add(grid_offset, terrain);
            map_building_set(grid_offset, building_id);
            map_property_clear_constructing(grid_offset);
            map_property_set_multi_tile_size(grid_offset, size);
            map_monuments_set_progress(grid_offset, 0);
            map_property_set_multi_tile_xy(grid_offset, dx, dy, dx == x_proper && dy == y_proper);
        }
    }
}

tile2i building_small_mastaba_bricks_waiting_tile(building *b) {
    if (b->type != BUILDING_SMALL_MASTABA) {
        return tile2i{-1, -1};
    }

    grid_tiles tiles = map_grid_get_tiles(b, 0);
    tile2i tile = map_grid_area_first(tiles, [b] (tile2i tile) {
        int progress = map_monuments_get_progress(tile.grid_offset());
        tile2i offset = tile.dist2i(b->tile).mod(4, 4);
        return !progress && (offset.x() == 1 || offset.x() == 3) && (offset.y() == 1 || offset.y() == 3);
    });

    return tile;
}

tile2i building_small_mastaba_tile4work(building *b) {
    if (b->type != BUILDING_SMALL_MASTABA) {
        return tile2i{-1, -1};
    }

    if (b->data.monuments.phase >= 2) {
        return tile2i{-1, -1};
    }

    grid_tiles tiles = map_grid_get_tiles(b, 0);
    return map_grid_area_first(tiles, [] (tile2i tile) { return !map_monuments_get_progress(tile.grid_offset()); });
}

void building_small_mastabe_update_day(building *b) {
    grid_tiles tiles = map_grid_get_tiles(b, 0);
    tile2i tile2works = map_grid_area_first(tiles, [] (tile2i tile) { return map_monuments_get_progress(tile.grid_offset()) < 200; });
    bool all_tiles_finished = (tile2works == tile2i{-1, -1});
    building *part = b;
    if (all_tiles_finished) {
        int next_phase = b->data.monuments.phase + 1;
        while (part) {
            map_grid_area_foreach(tiles, [] (tile2i tile) { map_monuments_set_progress(tile.grid_offset(), 0); });
            building_monument_set_phase(part, next_phase);
            part = part->has_next() ? part->next() : nullptr;
        }
    }

    if (b->data.monuments.phase == 2) {
        int minimal_percent = 100;
        for (e_resource r = RESOURCE_MIN; r < RESOURCES_MAX; ++r) {
            bool need_resource = building_monument_needs_resource(b, r);
            if (need_resource) {
                minimal_percent = std::min<int>(minimal_percent, b->data.monuments.resources_pct[r]);
            }
        }

        grid_tiles tiles = map_grid_get_tiles(b, 0);
        tiles.resize(tiles.size() * minimal_percent / 100);

        for (auto &tile : tiles) {
            int progress = map_monuments_get_progress(tile.grid_offset());
            if (progress == 1) {
                map_monuments_set_progress(tile.grid_offset(), 2);
            }
        }
    }
}

int building_small_mastabe_get_image(tile2i tile, tile2i start, tile2i end) {
    int image_id = image_group(IMG_SMALL_MASTABA);
    int base_image_id = image_id - 7;
    bool insidex = (tile.x() > start.x() && tile.x() < end.x());
    bool insidey = (tile.y() > start.y() && tile.y() < end.y());
    int random = (image_id + 5 + (tile.x() + tile.y()) % 7);
    int result = random;
    if (tile == start) { // top corner
        result = image_id;
    } else if (tile == tile2i(start.x(), end.y())) {
        result = image_id - 2;
    } else if (tile == end) {
        result = image_id - 4;
    } else if (tile == tile2i(end.x(), start.y())) {
        result = image_id - 6;
    } else if (tile.x() == start.x()) {
        result = image_id - 1;
    } else if (tile.y() == end.y()) {
        result = image_id - 3;
    } else if (tile.y() == start.y()) {
        result = (insidex || insidey) ? image_id - 7 : random;
    } else if (tile.x() == end.x()) {
        result = image_id - 5;
    }

    if (result < random) {
        int offset = result - base_image_id;
        result = (base_image_id + (offset + (8 - city_view_orientation())) % 8);
        return result;
    }

    return result;
}

int building_small_mastabe_get_bricks_image(e_building_type type, tile2i tile, tile2i start, vec2i size, int layer) {
    int image_base_bricks = image_group(IMG_SMALL_MASTABA_BRICKS);
    int image_id = image_base_bricks + (layer - 1) * 8 + 4;
    bool main = (type == BUILDING_SMALL_MASTABA);
    int random = (image_base_bricks + 96 + (layer - 1) + (tile.x() + tile.y()) % 1 * 6);
    int result = random;
    if (tile == start) { // top corner
        result = main ? (image_id + 3) : random;
    } else if (tile == start.shifted(0, size.y - 2)) {
        result = main ? random : (image_id + 1);
    } else if (tile == start.shifted(size.x - 2, size.y - 2)) {
        result = main ? random : (image_id + 1);
    } else if (tile == start.shifted(size.x - 2, 0)) {
        result = main ? (image_id + 3) : random;
    } else {
        result = random;
    } 

    if (result < random) {
        int offset = result - image_id;
        result = (image_id + (offset + (city_view_orientation()/2)) % 4);
        return result;
    }

    return result;
}

void draw_small_mastaba_anim_flat(painter &ctx, int x, int y, building *b, int color_mask) {
    int clear_land_id = image_id_from_group(GROUP_TERRAIN_EMPTY_LAND);
    int image_grounded = image_group(IMG_SMALL_MASTABA) + 5;
    building *main = b->main();
    color_mask = (color_mask ? color_mask : 0xffffffff);
    if (b->data.monuments.phase == 0) {
        for (int dy = 0; dy < b->size; dy++) {
            for (int dx = 0; dx < b->size; dx++) {
                tile2i ntile = b->tile.shifted(dx, dy);
                vec2i offset = tile_to_pixel(ntile);
                uint32_t progress = map_monuments_get_progress(ntile.grid_offset());
                if (progress < 200) {
                    ImageDraw::img_sprite(ctx, clear_land_id + ((dy * 4 + dx) & 7), offset.x, offset.y, color_mask);
                }

                if (progress > 0 && progress <= 200) {
                    int clr = ((0xff * progress / 200) << 24) | (color_mask & 0x00ffffff);
                    ImageDraw::img_sprite(ctx, image_grounded + ((dy * 4 + dx) & 7), offset, clr, 1.f, true);
                }
            }
        }

        int image_stick = image_group(IMG_SMALL_MASTABA) + 5 + 8;
        const image_t *img = image_get(image_stick);
        tile2i left_top = b->tile.shifted(0, 0);
        if (left_top == main->tile && map_monuments_get_progress(left_top.grid_offset()) == 0) {
            vec2i offset = tile_to_pixel(left_top);
            ImageDraw::img_sprite(ctx, image_stick, offset.x + img->animation.sprite_x_offset, offset.y + img->animation.sprite_y_offset - img->height + 30, color_mask);
        }

        tile2i right_top = b->tile.shifted(3, 0);
        if (right_top == main->tile.shifted(3, 0) && map_monuments_get_progress(right_top.grid_offset()) == 0) {
            vec2i offset = tile_to_pixel(right_top);
            ImageDraw::img_sprite(ctx, image_stick, offset.x + img->animation.sprite_x_offset, offset.y + img->animation.sprite_y_offset - img->height + 30, color_mask);
        }

        tile2i left_bottom = b->tile.shifted(0, 3);
        if (left_bottom == main->tile.shifted(0, 9) && map_monuments_get_progress(left_bottom.grid_offset()) == 0) {
            vec2i offset = tile_to_pixel(left_bottom);
            ImageDraw::img_sprite(ctx, image_stick, offset.x + img->animation.sprite_x_offset, offset.y + img->animation.sprite_y_offset - img->height + 30, color_mask);
        }

        tile2i right_bottom = b->tile.shifted(3, 3);
        if (right_bottom == main->tile.shifted(3, 9) && map_monuments_get_progress(right_bottom.grid_offset()) == 0) {
            vec2i offset = tile_to_pixel(right_bottom);
            ImageDraw::img_sprite(ctx, image_stick, offset.x + img->animation.sprite_x_offset, offset.y + img->animation.sprite_y_offset - img->height + 30, color_mask);
        }
    } else if (b->data.monuments.phase == 1) {
        for (int dy = 0; dy < b->size; dy++) {
            for (int dx = 0; dx < b->size; dx++) {
                tile2i ntile = b->tile.shifted(dx, dy);
                vec2i offset = tile_to_pixel(ntile);
                uint32_t progress = map_monuments_get_progress(ntile.grid_offset());
                if (progress < 200) {
                    ImageDraw::img_sprite(ctx, image_grounded + ((dy * 4 + dx) & 7), offset, color_mask, 1.f, true);
                }

                if (progress > 0 && progress <= 200) {
                    int clr = ((0xff * progress / 200) << 24) | (color_mask & 0x00ffffff);
                    int img = building_small_mastabe_get_image(b->tile.shifted(dx, dy), main->tile, main->tile.shifted(3, 9));
                    ImageDraw::img_sprite(ctx, img, offset, clr, 1.f, true);
                }
            }
        }
    } else if (b->data.monuments.phase == 2) {
        for (int dy = 0; dy < b->size; dy++) {
            for (int dx = 0; dx < b->size; dx++) {
                tile2i ntile = b->tile.shifted(dx, dy);
                vec2i offset = tile_to_pixel(ntile);
                uint32_t progress = map_monuments_get_progress(ntile.grid_offset());
                if (progress < 200) {
                    int img = building_small_mastabe_get_image(b->tile.shifted(dx, dy), main->tile, main->tile.shifted(3, 9));
                    ImageDraw::img_sprite(ctx, img, offset, color_mask, 1.f, true);
                }
            }
        }
    }
}

void draw_small_mastaba_anim(painter &ctx, int x, int y, building *b, int color_mask) {
    if (b->data.monuments.phase < 2) {
        return;
    }

    int clear_land_id = image_id_from_group(GROUP_TERRAIN_EMPTY_LAND);
    int image_grounded = image_group(IMG_SMALL_MASTABA) + 5;
    color_mask = (color_mask ? color_mask : 0xffffffff);
    building *main = b->main();

    vec2i city_orientation_offset{0, 0};
    switch (city_view_orientation()/2) {
    case 0: city_orientation_offset = vec2i(-30, -15); break;
    case 1: city_orientation_offset = vec2i(0, -30); break;
    case 2: city_orientation_offset = vec2i(-30, -45); break;
    case 3: city_orientation_offset = vec2i(-60, -30); break;
    }

    svector<tile2i, 16> tiles2draw;
    for (int dy = 0; dy < 4; dy++) {
        for (int dx = 0; dx < 4; dx++) {
            tile2i ntile = b->tile.shifted(dx, dy);
            if (dx % 2 == 0 && dy % 2 == 0) {
                tiles2draw.push_back(ntile);
            }
        }
    }

    std::sort(tiles2draw.begin(), tiles2draw.end(), [] (tile2i lhs, tile2i rhs) {
        vec2i lhs_offset = tile_to_pixel(lhs);
        vec2i rhs_offset = tile_to_pixel(rhs);
        return lhs_offset.y < rhs_offset.y;
    });

    if (b->data.monuments.phase == 2) {
        for (auto &tile: tiles2draw) {
            uint32_t progress = map_monuments_get_progress(tile.grid_offset());
            if (progress >= 200) {
                vec2i offset = tile_to_pixel(tile);
                int img = building_small_mastabe_get_bricks_image(b->type, tile, b->tile, vec2i(4, 4), 1);
                ImageDraw::img_sprite(ctx, img, offset + city_orientation_offset, color_mask, 1.f, true);
            }
        }
    } else if (b->data.monuments.phase == 3) {
        for (auto &tile: tiles2draw) {
            uint32_t progress = map_monuments_get_progress(tile.grid_offset());
            if (progress >= 200) {
                vec2i offset = tile_to_pixel(tile);
                int img = building_small_mastabe_get_bricks_image(b->type, tile, b->tile, vec2i(4, 4), 2);
                ImageDraw::img_sprite(ctx, img, offset + city_orientation_offset, color_mask, 1.f, true);
            } else {
                vec2i offset = tile_to_pixel(tile);
                int img = building_small_mastabe_get_bricks_image(b->type, tile, b->tile, vec2i(4, 4), 1);
                ImageDraw::img_sprite(ctx, img, offset + city_orientation_offset, color_mask, 1.f, true);
            }
        }
    }
}