#include "build_planner.h"

#include "figure/formation_herd.h"
#include "graphics/view/lookup.h"
#include "grid/floodplain.h"
#include "core/log.h"
#include "widget/widget_sidebar.h"

#include "building/building_dock.h"
#include "building/building_menu.h"
#include "building/count.h"
#include "building/model.h"
#include "building/monuments.h"
#include "building/monument_mastaba.h"
#include "building/building_plaza.h"
#include "building/building_garden.h"
#include "building/rotation.h"
#include "building/building_statue.h"
#include "building/building_storage_yard.h"
#include "building/building_road.h"
#include "building/building_booth.h"
#include "building/building_bandstand.h"
#include "building/building_pavilion.h"
#include "building/building_farm.h"
#include "building/building_fishing_wharf.h"
#include "building/building_festival_square.h"
#include "building/building_storage_yard.h"
#include "building/building_road.h"
#include "building/building_well.h"
#include "building/building_bridge.h"
#include "building/industry.h"
#include "building/monument_mastaba.h"
#include "building/rotation.h"
#include "building/building_fort.h"
#include "building/building.h"
#include "city/buildings.h"
#include "city/city_buildings.h"
#include "city/finance.h"
#include "city/city_resource.h"
#include "city/warnings.h"
#include "clear.h"
#include "figure/formation_legion.h"
#include "game/undo.h"
#include "graphics/image.h"
#include "graphics/image_groups.h"
#include "graphics/window.h"
#include "grid/bridge.h"
#include "grid/building.h"
#include "grid/building_tiles.h"
#include "grid/image.h"
#include "grid/orientation.h"
#include "grid/property.h"
#include "grid/routing/routing_terrain.h"
#include "grid/terrain.h"
#include "grid/tiles.h"
#include "grid/water.h"
#include "grid/figure.h"
#include "config/config.h"
#include "routed.h"
#include "city/city.h"

enum e_place_reservoir {
    PLACE_RESERVOIR_BLOCKED = -1,
    PLACE_RESERVOIR_NO = 0,
    PLACE_RESERVOIR_YES = 1,
    PLACE_RESERVOIR_EXISTS = 2
};

build_planner g_city_planner;

static int last_items_cleared;

static const vec2i FORT_OFFSET[4][4] = {{{3, -1}, {4, -1}, {4, 0}, {3, 0}}, 
                                        {{-1, -4}, {0, -4}, {0, -3}, {-1, -3}}, 
                                        {{-4, 0}, {-3, 0}, {-3, 1}, {4, 1}}, 
                                        {{0, 3}, {1, 3}, {1, 4}, {0, 4}}};

const vec2i VIEW_OFFSETS[] = {                    {0, 0},
                                       {-30,15}, {30, 15}, {0,30},
                             {-60,30}, {60, 30}, {-30,45}, {30, 45}, {0,  60},
                  {-90, 45}, {90, 45}, {-60,60}, {60, 60}, {-30,75}, {30, 75},  {0, 90},
       {-120,60}, {120, 60}, {-90,75}, {90, 75}, {-60,90}, {60, 90}, {-30,105}, {30,105}, {0,120} };

#define MAX_TILES 25
const int TILE_GRID_OFFSETS_PH[4][MAX_TILES] = {
  {GRID_OFFSET(0, 0), GRID_OFFSET(0, 1), GRID_OFFSET(1, 0), GRID_OFFSET(1, 1), GRID_OFFSET(0, 2),
   GRID_OFFSET(2, 0), GRID_OFFSET(1, 2), GRID_OFFSET(2, 1), GRID_OFFSET(2, 2), GRID_OFFSET(0, 3),
   GRID_OFFSET(3, 0), GRID_OFFSET(1, 3), GRID_OFFSET(3, 1), GRID_OFFSET(2, 3), GRID_OFFSET(3, 2),
   GRID_OFFSET(3, 3), GRID_OFFSET(0, 4), GRID_OFFSET(4, 0), GRID_OFFSET(1, 4), GRID_OFFSET(4, 1),
   GRID_OFFSET(2, 4), GRID_OFFSET(4, 2), GRID_OFFSET(3, 4), GRID_OFFSET(4, 3), GRID_OFFSET(4, 4)},
  {GRID_OFFSET(0, 0),  GRID_OFFSET(-1, 0), GRID_OFFSET(0, 1),  GRID_OFFSET(-1, 1), GRID_OFFSET(-2, 0),
   GRID_OFFSET(0, 2),  GRID_OFFSET(-2, 1), GRID_OFFSET(-1, 2), GRID_OFFSET(-2, 2), GRID_OFFSET(-3, 0),
   GRID_OFFSET(0, 3),  GRID_OFFSET(-3, 1), GRID_OFFSET(-1, 3), GRID_OFFSET(-3, 2), GRID_OFFSET(-2, 3),
   GRID_OFFSET(-3, 3), GRID_OFFSET(-4, 0), GRID_OFFSET(0, 4),  GRID_OFFSET(-4, 1), GRID_OFFSET(-1, 4),
   GRID_OFFSET(-4, 2), GRID_OFFSET(-2, 4), GRID_OFFSET(-4, 3), GRID_OFFSET(-3, 4), GRID_OFFSET(-4, 4)},
  {GRID_OFFSET(0, 0),   GRID_OFFSET(0, -1),  GRID_OFFSET(-1, 0),  GRID_OFFSET(-1, -1), GRID_OFFSET(0, -2),
   GRID_OFFSET(-2, 0),  GRID_OFFSET(-1, -2), GRID_OFFSET(-2, -1), GRID_OFFSET(-2, -2), GRID_OFFSET(0, -3),
   GRID_OFFSET(-3, 0),  GRID_OFFSET(-1, -3), GRID_OFFSET(-3, -1), GRID_OFFSET(-2, -3), GRID_OFFSET(-3, -2),
   GRID_OFFSET(-3, -3), GRID_OFFSET(0, -4),  GRID_OFFSET(-4, 0),  GRID_OFFSET(-1, -4), GRID_OFFSET(-4, -1),
   GRID_OFFSET(-2, -4), GRID_OFFSET(-4, -2), GRID_OFFSET(-3, -4), GRID_OFFSET(-4, -3), GRID_OFFSET(-4, -4)},
  {GRID_OFFSET(0, 0),  GRID_OFFSET(1, 0),  GRID_OFFSET(0, -1), GRID_OFFSET(1, -1), GRID_OFFSET(2, 0),
   GRID_OFFSET(0, -2), GRID_OFFSET(2, -1), GRID_OFFSET(1, -2), GRID_OFFSET(2, -2), GRID_OFFSET(3, 0),
   GRID_OFFSET(0, -3), GRID_OFFSET(3, -1), GRID_OFFSET(1, -3), GRID_OFFSET(3, -2), GRID_OFFSET(2, -3),
   GRID_OFFSET(3, -3), GRID_OFFSET(4, 0),  GRID_OFFSET(0, -4), GRID_OFFSET(4, -1), GRID_OFFSET(1, -4),
   GRID_OFFSET(4, -2), GRID_OFFSET(2, -4), GRID_OFFSET(4, -3), GRID_OFFSET(3, -4), GRID_OFFSET(4, -4)},
};

void build_planner::add_building_tiles_from_list(int building_id, bool graphics_only) {
    for (int row = 0; row < size.y; ++row) {
        for (int column = 0; column < size.x; ++column) {
            int image_id = tile_graphics_array[row][column];
            int size = tile_sizes_array[row][column];
            tile2i tile = tile_coord_cache[row][column];

            // correct for city orientation
            switch (city_view_orientation() / 2) {
            case 0: tile.shift(0, -size + 1); break;
            case 1: break;
            case 2: tile.shift(-size + 1, 0); break;
            case 3: tile.shift(-size + 1, -size + 1); break;
            }

            if (image_id > 0 && size > 0) {
                if (!graphics_only) {
                    map_building_tiles_add(building_id, tile, size, image_id, TERRAIN_BUILDING);
                } else {
                    map_image_set(tile.grid_offset(), image_id);
                }
            }
        }
    }
}

bool build_planner::ghost_mark_deleting(tile2i tile) {
    if (!config_get(CONFIG_UI_VISUAL_FEEDBACK_ON_DELETE)) {
        return false;
    }

    int construction_type = build_type;
    if (!tile.grid_offset() || draw_as_constructing || construction_type != BUILDING_CLEAR_LAND) {
        return (construction_type == BUILDING_CLEAR_LAND);
    }

    if (!in_progress) {
        map_property_clear_constructing_and_deleted();
    }

    map_building_tiles_mark_deleting(tile.grid_offset());
    return true;
}

void build_planner_latch_on_venue(e_building_type type, building *b, int dx, int dy, int orientation, bool main_venue) {
    tile2i point = b->tile.shifted(dx, dy);
    // set map graphics accordingly

    const auto &params = b->dcast()->params();
    switch (type) {
    case BUILDING_GARDENS:
        map_terrain_add(point, TERRAIN_GARDEN);
        map_tiles_update_all_gardens();
        break;

    case BUILDING_BOOTH: {
           int booth = params.anim["booth"].first_img();
           map_image_set(point, booth);
        }
        break;

    case BUILDING_BANDSTAND:
        if (main_venue) {
            int stand_sn_s = params.anim["stand_sn_s"].first_img();
            b->data.entertainment.latched_venue_main_grid_offset = point.grid_offset();
            int offset = map_bandstand_main_img_offset(orientation);
            map_image_set(point, stand_sn_s + offset);
        } else {
            int stand_sn_s = params.anim["stand_sn_s"].first_img();
            b->data.entertainment.latched_venue_add_grid_offset = point.grid_offset();
            int offset = map_bandstand_add_img_offset(orientation);
            map_image_set(point, stand_sn_s + offset);
        }
        break;

    case BUILDING_PAVILLION: {
            int base_id = params.anim["base"].first_img();
            map_building_tiles_add(b->id, point, 2, base_id, TERRAIN_BUILDING);
        }
        break;
    }
}

static void add_building_tiles_image(building* b, int image_id) {
    map_building_tiles_add(b->id, b->tile, b->size, image_id, TERRAIN_BUILDING);
}

static void add_building(building* b, int orientation, int variant) {
    int orientation_rel = city_view_relative_orientation(orientation);
    const auto &params = b->dcast()->params();
    switch (b->type) {
    // houses
    case BUILDING_HOUSE_STURDY_HUT:
    case BUILDING_HOUSE_MEAGER_SHANTY:
    case BUILDING_HOUSE_COMMON_SHANTY:
    case BUILDING_HOUSE_ROUGH_COTTAGE:
    case BUILDING_HOUSE_ORDINARY_COTTAGE:
    case BUILDING_HOUSE_MODEST_HOMESTEAD:
    case BUILDING_HOUSE_SPACIOUS_HOMESTEAD:
    case BUILDING_HOUSE_MODEST_APARTMENT:
    case BUILDING_HOUSE_SPACIOUS_APARTMENT:
    case BUILDING_HOUSE_COMMON_RESIDENCE:
    case BUILDING_HOUSE_SPACIOUS_RESIDENCE:
    case BUILDING_HOUSE_ELEGANT_RESIDENCE:
    case BUILDING_HOUSE_FANCY_RESIDENCE:
    case BUILDING_HOUSE_COMMON_MANOR:
    case BUILDING_HOUSE_SPACIOUS_MANOR:
    case BUILDING_HOUSE_ELEGANT_MANOR:
    case BUILDING_HOUSE_STATELY_MANOR:
    case BUILDING_HOUSE_MODEST_ESTATE:
    case BUILDING_HOUSE_PALATIAL_ESTATE:
        add_building_tiles_image(b, params.anim["house"].first_img());
        break;

    case BUILDING_RESERVED_TRIUMPHAL_ARCH_56:
        add_building_tiles_image(b, image_id_from_group(GROUP_BUILDING_TRIUMPHAL_ARCH) + orientation - 1);
        map_terrain_add_triumphal_arch_roads(b->tile.x(), b->tile.y(), orientation);
        city_buildings_build_triumphal_arch();
        g_city_planner.reset();
        break;

    default:
        b->dcast()->on_place(orientation, variant);
        break;
    }
}

static void mark_construction(tile2i tile, int size_x, int size_y, int terrain, bool absolute_xy) {
    if (g_city_planner.can_be_placed() == CAN_PLACE
        && map_building_tiles_mark_construction(tile, size_x, size_y, terrain, absolute_xy)) {
        g_city_planner.draw_as_constructing = true;
    }
}

static int has_nearby_enemy(int x_start, int y_start, int x_end, int y_end) {
    for (int i = 1; i < MAX_FIGURES; i++) {
        figure* f = figure_get(i);
        if (config_get(CONFIG_GP_CH_WOLVES_BLOCK)) {
            if (f->state != FIGURE_STATE_ALIVE || !f->is_enemy()) {
                continue;
            }

        } else if (f->state != FIGURE_STATE_ALIVE || !f->is_enemy()) {
            continue;
        }

        int tile_x = f->tile.x();
        int tile_y = f->tile.y();
        int dx = (tile_x > x_start) ? (tile_x - x_start) : (x_start - tile_x);
        int dy = (tile_y > y_start) ? (tile_y - y_start) : (y_start - tile_y);
        if (dx <= 12 && dy <= 12) {
            return 1;
        }

        dx = (tile_x > x_end) ? (tile_x - x_end) : (x_end - tile_x);
        dy = (tile_y > y_end) ? (tile_y - y_end) : (y_end - tile_y);
        if (dx <= 12 && dy <= 12) {
            return 1;
        }
    }
    return 0;
}

int build_planner::place_houses(bool measure_only, int x_start, int y_start, int x_end, int y_end) {
    grid_area area = map_grid_get_area(tile2i(x_start, y_start), tile2i(x_end, y_end));

    int needs_road_warning = 0;
    int items_placed = 0;
    game_undo_restore_building_state();
    int vacant_lot_id = building_impl::params(BUILDING_HOUSE_VACANT_LOT).anim["preview"].first_img();
    for (int y = area.tmin.y(), endy = area.tmax.y(); y <= endy; y++) {
        for (int x = area.tmin.x(), endx = area.tmax.x(); x <= endx; x++) {
            int grid_offset = MAP_OFFSET(x, y);
            if (map_terrain_is(grid_offset, TERRAIN_NOT_CLEAR)
                || map_terrain_exists_tile_in_radius_with_type(tile2i(x, y), 1, 1, TERRAIN_FLOODPLAIN)) {
                continue;
            }

            if (measure_only) {
                map_property_mark_constructing(grid_offset);
                items_placed++;

            } else {
                if (formation_herd_breeding_ground_at(x, y, 1)) {
                    map_property_clear_constructing_and_deleted();
                    city_warning_show(WARNING_HERD_BREEDING_GROUNDS);
                } else {
                    building* b = building_create(BUILDING_HOUSE_VACANT_LOT, tile2i(x, y), 0);
                    game_undo_add_building(b);
                    if (b->id > 0) {
                        items_placed++;
                        tile2i otile(x, y);
                        map_building_tiles_add(b->id, otile, 1, vacant_lot_id, TERRAIN_BUILDING);
                        if (!map_terrain_exists_tile_in_radius_with_type(otile, 1, 2, TERRAIN_ROAD)) {
                            needs_road_warning = 1;
                        }
                    }
                }
            }
        }
    }

    if (!measure_only) {
        //building_construction_warning_check_food_stocks(BUILDING_HOUSE_VACANT_LOT);
        if (needs_road_warning) {
            city_warning_show(WARNING_HOUSE_TOO_FAR_FROM_ROAD);
        }

        map_routing_update_land();
    }
    return items_placed;
}


bool build_planner::place_building(e_building_type type, tile2i tile, int orientation, int variant) {
    // by default, get size from building's properties
    int size = building_impl::params(type).building_size;
    assert(size > 0);

    // correct building placement for city orientations
    switch (city_view_orientation()) {
    case DIR_2_BOTTOM_RIGHT:
        tile = tile.shifted(-size + 1, 0);
        break;

    case DIR_4_BOTTOM_LEFT:
        tile = tile.shifted(-size + 1, -size + 1);
        break;

    case DIR_6_TOP_LEFT:
        tile = tile.shifted(0, -size + 1);
        break;
    }

    // create building
    last_created_building = nullptr;
    building* b = building_create(type, tile, orientation);
    game_undo_add_building(b);
    if (b->id <= 0) { // building creation failed????
        return false;
    }
    
    add_building(b, orientation, variant);
    last_created_building = b;
    return true;
}

static bool attach_temple_upgrade(int upgrade_param, int grid_offset) {
    building* target = building_at(grid_offset)->main();
    if (!building_at(grid_offset) || !building_is_temple_complex(target->type)) {
        return false;
    }

    if (target->data.monuments.temple_complex_attachments & upgrade_param) {
        return false;
    }

    target->data.monuments.temple_complex_attachments |= upgrade_param;
    map_building_tiles_add_temple_complex_parts(target);
    building_menu_update_temple_complexes();
    return true;
}

static tile2i temple_complex_part_target(building* main, int part) {
    building* b = main;
    if (part == 1) {
        b = b->next();
    } else if (part == 2) {
       // b = get_temple_complex_front_facing_part(main);
    }

    int x = b->tile.x();
    int y = b->tile.y();
    switch (city_view_orientation() / 2) {
    case 1:  x += 2; break; // east
    case 2:  x += 2; y += 2; break; // south
    case 3:  y += 2; break; // west
    }
    return tile2i(x, y);
}

//////////////////////

int build_planner::can_be_placed() {
    return can_place;
}

void build_planner::reset() {
    // reset build info
    total_cost = 0;
    build_type = BUILDING_NONE;
    in_progress = false;
    draw_as_constructing = false;

    // set boundary size and reset pivot
    size = {0, 0};
    pivot = {0, 0};
    tiles_blocked_total = 0;

    // position and orientation
    start.set(-1, -1);
    end.set(-1, -1);
    relative_orientation = 0;
    custom_building_variant = 0;
    building_variant = 0;

    // reset special requirements flags/params
    special_flags = 0;
    additional_req_param1 = -1;
    additional_req_param2 = -1;
    additional_req_param3 = -1;
    can_place = CAN_PLACE;
    immediate_warning_id = -1;
    extra_warning_id = -1;
}

void build_planner::init_tiles(int size_x, int size_y) {
    size.x = size_x;
    size.y = size_y;
    for (int row = 0; row < size.y; ++row) {
        for (int column = 0; column < size.x; ++column) {
            if (column > 29 || row > 29)
                return;

            tile_graphics_array[row][column] = 0;
            tile_sizes_array[row][column] = 1; // reset tile size to 1 by default
            tile_blocked_array[row][column] = false;

            // reset caches
            tile_coord_cache[row][column] = {0, 0};
            pixel_coords_cache[row][column] = {0, 0};
        }
    }
}

void build_planner::set_tile_size(int row, int column, int size) {
    if (row > 29 || column > 29)
        return;
    tile_sizes_array[row][column] = size;
}

void build_planner::set_flag(long long flags, int param1, int param2, int param3) {
    special_flags |= flags;
    if (param1 != -1)
        additional_req_param1 = param1;

    if (param2 != -1)
        additional_req_param2 = param2;

    if (param3 != -1)
        additional_req_param3 = param3;
}

bool build_planner::has_flag_set(int flag, int param1, int param2, int param3) {
    if (param1 != -1 && additional_req_param1 != param1)
        return false;

    if (param2 != -1 && additional_req_param2 != param2)
        return false;

    if (param3 != -1 && additional_req_param3 != param3)
        return false;

    if (special_flags & flag)
        return true;

    return false;
}

void build_planner::set_graphics_row(int row, int* image_ids, int total) {
    if (row < 0) {
        return;
    }

    for (int i = 0; i < total; ++i) {
        if (row > 29 || i > 29) {
            return;
        }

        tile_graphics_array[row][i] = image_ids[i];

        // set sizes automatically as default
        int tile_size = 0;
        if (image_ids[i] != 0) {
            auto img = image_get(image_ids[i]);
            set_tile_size(row, i, img->isometric_size());
        }
    }
}

void build_planner::set_tiles_building(int image_id, int size_xx) {
    init_tiles(size_xx, size_xx);
    int empty_row[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int draw_row[] = {image_id, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int row = 0; row < size.y; ++row) {
        if (row == size.y - 1)
            set_graphics_row(row, draw_row, size.x);
        else
            set_graphics_row(row, empty_row, size.x);
    }
}

void build_planner::set_graphics_array(int* image_set, int size_x, int size_y) {
    init_tiles(size_x, size_y);
    // int (*image_array)[size_y][size_x] = (int(*)[size_y][size_x])image_set;

    // do it row by row...
    // for (int row = 0; row < size_y; ++row)
    //    set_graphics_row(row, (*image_array)[row], size_x);

    for (int row = 0; row < size_y; ++row) {
        set_graphics_row(row, image_set + (row * size_x), size_x);
    }
}

void build_planner::setup_building_variant(tile2i tile, e_building_type type) {
    const auto &params = building_impl::params(type);
    int random_value = tile.grid_offset();
    custom_building_variant = params.planer_setup_building_variant(type, tile, random_value);
}

void build_planner::next_building_variant() {
    const auto &params = building_impl::params(build_type);
    custom_building_variant = params.planer_next_building_variant(build_type, end, custom_building_variant);
    update_orientations();
}

void build_planner::setup_build(e_building_type type) { // select building for construction, set up main terrain restrictions/requirements
    reset();
    build_type = type;

    // ignore empty building types
    switch (type) {
    case BUILDING_NONE:
    case BUILDING_MENU_FARMS:
    case BUILDING_MENU_RAW_MATERIALS:
    case BUILDING_MENU_SHRINES:
    case BUILDING_MENU_TEMPLES:
    case BUILDING_MENU_TEMPLE_COMPLEX:
    case BUILDING_MENU_CONSTURCTION_GUILDS:
    case BUILDING_MENU_DEFENSES:
    case BUILDING_MENU_FORTS:
    case BUILDING_MENU_WATER_CROSSINGS:
    case BUILDING_MENU_BEAUTIFICATION:
    case BUILDING_MENU_MONUMENTS:
        return;
    }

    const auto &params = building_impl::params(type);
    params.planer_setup_build(*this);

    setup_building_variant(end, build_type);
    relative_orientation = params.planer_setup_orientation(relative_orientation); // force these buildings to start in a specific orientation
    update_orientations(false);

    // load building data
    setup_build_flags();
    setup_build_graphics();
}

void build_planner::setup_build_flags() {
    switch (build_type) {
    default:
        ; // nothing
        break;

    case BUILDING_BARLEY_FARM:
    case BUILDING_FLAX_FARM:
    case BUILDING_GRAIN_FARM:
    case BUILDING_LETTUCE_FARM:
    case BUILDING_POMEGRANATES_FARM:
    case BUILDING_CHICKPEAS_FARM:
    case BUILDING_FIGS_FARM:
    case BUILDING_HENNA_FARM:
        set_flag(PlannerFlags::Meadow);
        break;

    case BUILDING_STONE_QUARRY:
    case BUILDING_LIMESTONE_QUARRY:
    case BUILDING_GRANITE_QUARRY:
    case BUILDING_SANDSTONE_QUARRY:
        set_flag(PlannerFlags::Rock);
        break;

    case BUILDING_GOLD_MINE:
        set_flag(PlannerFlags::Rock);
        set_flag(PlannerFlags::Ore);
        break;

    case BUILDING_GEMSTONE_MINE:
        set_flag(PlannerFlags::Rock);
        break;

    case BUILDING_COPPER_MINE:
        set_flag(PlannerFlags::Rock);
        if (!config_get(CONFIG_GP_CH_COPPER_NEAR_MOUNTAINS)) {
            set_flag(PlannerFlags::Ore);
        }
        break;

    case BUILDING_CLAY_PIT:
        set_flag(PlannerFlags::NearbyWater);
        break;

    case BUILDING_MUD_TOWER:
        set_flag(PlannerFlags::Walls);
        break;

    case BUILDING_WELL:
    case BUILDING_WATER_SUPPLY:
    case BUILDING_VILLAGE_PALACE:
    case BUILDING_TOWN_PALACE:
    case BUILDING_CITY_PALACE:
        set_flag(PlannerFlags::Groundwater);
        break;
        //        case BUILDING_LIBRARY: // TODO
        //            set_requirements(PlannerReqs::Resources, RESOURCE_PAPYRUS, 300);
        //            break;
        //        case BUILDING_OBELYSK: // TODO
        //            set_requirements(PlannerReqs::Resources, RESOURCE_GRANITE, 200);
        //            break;
    case BUILDING_WATER_LIFT:
        set_flag(PlannerFlags::ShoreLine, 2);
        set_flag(PlannerFlags::FloodplainShore, 2);
        break;

    case BUILDING_FISHING_WHARF:
    case BUILDING_TRANSPORT_WHARF:
        set_flag(PlannerFlags::ShoreLine, 2);
        break;

    case BUILDING_SHIPWRIGHT:
    case BUILDING_DOCK:
    case BUILDING_WARSHIP_WHARF:
        set_flag(PlannerFlags::ShoreLine, 3);
        break;

    case BUILDING_FERRY:
        set_flag(PlannerFlags::ShoreLine, 2);
        set_flag(PlannerFlags::Ferry);
        break;

    case BUILDING_LOW_BRIDGE:
    case BUILDING_UNUSED_SHIP_BRIDGE_83:
        set_flag(PlannerFlags::ShoreLine, 1);
        set_flag(PlannerFlags::Bridge);
        break;

    case BUILDING_IRRIGATION_DITCH:
        set_flag(PlannerFlags::Canals, false);
        break;

    case BUILDING_ROAD:
        set_flag(PlannerFlags::Road, false);
        break;

    case BUILDING_ROADBLOCK:
        set_flag(PlannerFlags::Road, true, WARNING_ROADBLOCKS_ROAD_NEEDED);
        break;

    case BUILDING_PLAZA:
        set_flag(PlannerFlags::Road, true, -1);
        set_flag(PlannerFlags::FancyRoad);
        break;

    case BUILDING_BOOTH:
        set_flag(PlannerFlags::Intersection, 0, WARNING_BOOTH_ROAD_INTERSECTION_NEEDED);
        break;

    case BUILDING_BANDSTAND:
        set_flag(PlannerFlags::Intersection, 1, WARNING_BOOTH_ROAD_INTERSECTION_NEEDED);
        break;

    case BUILDING_PAVILLION:
        set_flag(PlannerFlags::Intersection, 2, WARNING_BOOTH_ROAD_INTERSECTION_NEEDED);
        break;

    case BUILDING_FESTIVAL_SQUARE:
        set_flag(PlannerFlags::Intersection, 3, WARNING_FESTIVAL_ROAD_INTERSECTION_NEEDED);
        break;

    case BUILDING_CLEAR_LAND:
        set_flag(PlannerFlags::IgnoreNearbyEnemy);
        break;

    case BUILDING_TEMPLE_COMPLEX_ALTAR:
        set_flag(PlannerFlags::TempleUpgrade, 2);
        break;

    case BUILDING_TEMPLE_COMPLEX_ORACLE:
        set_flag(PlannerFlags::TempleUpgrade, 1);
        break;
    }

    if (building_is_draggable(build_type)) {
        set_flag(PlannerFlags::Draggable);
    }
}

void build_planner::setup_build_graphics() {
    const auto &params = building_impl::params(build_type);
    
    vec2i init_tiles_size;
    switch (build_type) {
    case BUILDING_TEMPLE_COMPLEX_OSIRIS:
    case BUILDING_TEMPLE_COMPLEX_RA:
    case BUILDING_TEMPLE_COMPLEX_PTAH:
    case BUILDING_TEMPLE_COMPLEX_SETH:
    case BUILDING_TEMPLE_COMPLEX_BAST: { 
        // size of every big item 3x3, in general 7x13
        // 25 max tiles at the moment to check blocked tiles
        int packid = -1;
        switch (build_type) {
        case BUILDING_TEMPLE_COMPLEX_OSIRIS: packid = PACK_TEMPLE_NILE; break;
        case BUILDING_TEMPLE_COMPLEX_RA: packid = PACK_TEMPLE_RA; break;
        case BUILDING_TEMPLE_COMPLEX_PTAH: packid = PACK_TEMPLE_PTAH; break;
        case BUILDING_TEMPLE_COMPLEX_SETH: packid = PACK_TEMPLE_SETH; break;
        case BUILDING_TEMPLE_COMPLEX_BAST: packid = PACK_TEMPLE_BAST; break;
            break;
        }

        int main_image_id = image_id_from_group(packid, 1);
        int oracle_image_id = image_id_from_group(packid, 2);
        int altar_image_id = image_id_from_group(packid, 3);
        int flooring_image_id = image_id_from_group(packid, 4);
        int statue1_image_id = image_id_from_group(packid, 5);
        int statue2_image_id = image_id_from_group(packid, 6);

        int EMPTY = 0;
        int mn_1A = main_image_id;
        int mn_1B = main_image_id + 3;
        int mn_2A = oracle_image_id;
        int mn_2B = oracle_image_id + 3;
        int mn_3A = altar_image_id;
        int mn_3B = altar_image_id + 3;

        int til_0 = flooring_image_id + 0;
        int til_1 = flooring_image_id + 1;
        int til_2 = flooring_image_id + 2;
        int til_3 = flooring_image_id + 3;

        int smst0 = statue1_image_id + 0; // north
        int smst1 = statue1_image_id + 1; // east
        int smst2 = statue1_image_id + 2; // south
        int smst3 = statue1_image_id + 3; // west

        int lst0A = statue2_image_id + 0; // north
        int lst0B = statue2_image_id + 1;
        int lst1A = statue2_image_id + 2; // east
        int lst1B = statue2_image_id + 3;
        int lst2A = statue2_image_id + 4; // south
        int lst2B = statue2_image_id + 5;
        int lst3A = statue2_image_id + 6; // west
        int lst3B = statue2_image_id + 7;

        //            int orientation_rel = city_view_relative_orientation(orientation_rel);
        switch (relative_orientation) {
        case 0: { // NE
            int TEMPLE_COMPLEX_SCHEME[13][7] = {
              {til_3, lst1A, lst1B, til_1, lst3A, lst3B, til_3},
              {til_2, lst1A, lst1B, til_1, lst3A, lst3B, til_2},
              {til_3, lst1A, lst1B, til_1, lst3A, lst3B, til_3},
              {til_2, til_0, til_0, til_1, til_0, til_0, til_2},
              {til_0, til_0, EMPTY, EMPTY, EMPTY, til_0, til_0},
              {smst3, til_0, EMPTY, EMPTY, EMPTY, til_0, smst1},
              {smst3, til_0, mn_1B, EMPTY, EMPTY, til_0, smst1},
              {til_1, til_1, EMPTY, EMPTY, EMPTY, til_1, til_1},
              {smst3, til_0, EMPTY, EMPTY, EMPTY, til_0, smst1},
              {smst3, til_0, mn_2B, EMPTY, EMPTY, til_0, smst1},
              {til_1, til_1, EMPTY, EMPTY, EMPTY, til_1, til_1},
              {smst3, til_0, EMPTY, EMPTY, EMPTY, til_0, smst1},
              {smst3, til_0, mn_3B, EMPTY, EMPTY, til_0, smst1},
            };
            set_graphics_array((int*)TEMPLE_COMPLEX_SCHEME, 7, 13);
            pivot = {2, 10};
            break;
        }
        case 1: { // SE
            int TEMPLE_COMPLEX_SCHEME[7][13] = {
              {smst0, smst0, til_1, smst0, smst0, til_1, smst0, smst0, til_0, til_2, til_3, til_2, til_3},
              {til_0, til_0, til_1, til_0, til_0, til_1, til_0, til_0, til_0, til_0, lst2B, lst2B, lst2B},
              {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, til_0, lst2A, lst2A, lst2A},
              {EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, til_1, til_1, til_1, til_1},
              {mn_1A, EMPTY, EMPTY, mn_2A, EMPTY, EMPTY, mn_3A, EMPTY, EMPTY, til_0, lst0B, lst0B, lst0B},
              {til_0, til_0, til_1, til_0, til_0, til_1, til_0, til_0, til_0, til_0, lst0A, lst0A, lst0A},
              {smst2, smst2, til_1, smst2, smst2, til_1, smst2, smst2, til_0, til_2, til_3, til_2, til_3},
            };
            set_graphics_array((int*)TEMPLE_COMPLEX_SCHEME, 13, 7);
            pivot = {0, 2};
            break;
        }
        case 2: { // SW
            int TEMPLE_COMPLEX_SCHEME[13][7] = {
              {smst3, til_0, EMPTY, EMPTY, EMPTY, til_0, smst1},
              {smst3, til_0, EMPTY, EMPTY, EMPTY, til_0, smst1},
              {til_1, til_1, mn_1B, EMPTY, EMPTY, til_1, til_1},
              {smst3, til_0, EMPTY, EMPTY, EMPTY, til_0, smst1},
              {smst3, til_0, EMPTY, EMPTY, EMPTY, til_0, smst1},
              {til_1, til_1, mn_2B, EMPTY, EMPTY, til_1, til_1},
              {smst3, til_0, EMPTY, EMPTY, EMPTY, til_0, smst1},
              {smst3, til_0, EMPTY, EMPTY, EMPTY, til_0, smst1},
              {til_0, til_0, mn_3B, EMPTY, EMPTY, til_0, til_0},
              {til_2, til_0, til_0, til_1, til_0, til_0, til_2},
              {til_3, lst1A, lst1B, til_1, lst3A, lst3B, til_3},
              {til_2, lst1A, lst1B, til_1, lst3A, lst3B, til_2},
              {til_3, lst1A, lst1B, til_1, lst3A, lst3B, til_3},
            };
            set_graphics_array((int*)TEMPLE_COMPLEX_SCHEME, 7, 13);
            pivot = {2, 0};
            break;
        }
        case 3: { // NW
            int TEMPLE_COMPLEX_SCHEME[7][13] = {
              {til_3, til_2, til_3, til_2, til_0, smst0, smst0, til_1, smst0, smst0, til_1, smst0, smst0},
              {lst2B, lst2B, lst2B, til_0, til_0, til_0, til_0, til_1, til_0, til_0, til_1, til_0, til_0},
              {lst2A, lst2A, lst2A, til_0, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
              {til_1, til_1, til_1, til_1, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY},
              {lst0B, lst0B, lst0B, til_0, mn_1A, EMPTY, EMPTY, mn_2A, EMPTY, EMPTY, mn_3A, EMPTY, EMPTY},
              {lst0A, lst0A, lst0A, til_0, til_0, til_0, til_0, til_1, til_0, til_0, til_1, til_0, til_0},
              {til_3, til_2, til_3, til_2, til_0, smst2, smst2, til_1, smst2, smst2, til_1, smst2, smst2},
            };
            set_graphics_array((int*)TEMPLE_COMPLEX_SCHEME, 13, 7);
            pivot = {10, 2};
            break;
        }
        }
        break;
    }
    case BUILDING_TEMPLE_COMPLEX_ALTAR:
    case BUILDING_TEMPLE_COMPLEX_ORACLE:
        init_tiles(3, 3);
        set_tiles_building(get_temple_complex_part_image(building_at(end.grid_offset())->main()->type, additional_req_param1, relative_orientation, 1), 3);
        break;

    case BUILDING_WATER_LIFT:
        set_tiles_building(params.anim[animkeys().base].first_img() + relative_orientation + building_variant * 4, params.building_size);
        break;

    default: // regular buildings 
        params.planer_setup_preview_graphics(*this);
        break;
    }
}

void build_planner::update_obstructions_check() {
    tiles_blocked_total = 0;
    for (int row = 0; row < size.y; row++) {
        for (int column = 0; column < size.x; column++) {
            // check terrain at coords
            tile2i current_tile = tile_coord_cache[row][column];
            unsigned int restricted_terrain = TERRAIN_ALL;

            // special cases
            if (special_flags & PlannerFlags::Meadow
                || special_flags & PlannerFlags::FloodplainShore
                || special_flags & PlannerFlags::Road || special_flags & PlannerFlags::Canals) {
                restricted_terrain -= TERRAIN_FLOODPLAIN;
            }

            bool can_blocked_by_floodplain_edge = !building_is_farm(build_type);
            if (special_flags & PlannerFlags::Road
                || special_flags & PlannerFlags::Intersection
                || special_flags & PlannerFlags::Canals) {
                restricted_terrain -= TERRAIN_ROAD;
                can_blocked_by_floodplain_edge = false;
            }

            if (special_flags & PlannerFlags::Road || special_flags & PlannerFlags::Canals) {
                restricted_terrain -= TERRAIN_CANAL;

            }

            if (special_flags & PlannerFlags::Water || special_flags & PlannerFlags::ShoreLine) {
                restricted_terrain -= TERRAIN_WATER;
            }

            if (special_flags & PlannerFlags::TempleUpgrade) { // special case
                return;
            }

            tile_blocked_array[row][column] = false;
            bool blocked_by_floodplain_edge = (can_blocked_by_floodplain_edge && map_get_floodplain_edge(current_tile));
            if (!map_grid_is_inside(current_tile, 1)
                || map_terrain_is(current_tile, restricted_terrain & TERRAIN_NOT_CLEAR)
                || map_has_figure_at(current_tile)
                || blocked_by_floodplain_edge) {
                tile_blocked_array[row][column] = true;
                tiles_blocked_total++;
            }
        }
    }

    if (tiles_blocked_total > 0) {
        immediate_warning_id = WARNING_CLEAR_LAND_NEEDED;
        can_place = CAN_NOT_BUT_GREEN;
    }
}

void build_planner::update_requirements_check() {
    // invalid build
    if (build_type == BUILDING_NONE) {
        can_place = CAN_NOT_PLACE;
        return;
    }

    // out of money!
    if (city_finance_out_of_money()) {
        // TODO: no money needed if building has zero cost?
        immediate_warning_id = WARNING_OUT_OF_MONEY;
        can_place = CAN_NOT_PLACE;
        return;
    }

    /////// special requirements
    //
    if (special_flags & PlannerFlags::Resources) {
        if (city_resource_warehouse_stored((e_resource)additional_req_param1) < additional_req_param2) {
            immediate_warning_id = additional_req_param3;
            can_place = CAN_NOT_BUT_GREEN;
        }
    }

    if (special_flags & PlannerFlags::Groundwater) {
        if (!map_terrain_exists_tile_in_radius_with_type(end, size.x, 0, TERRAIN_GROUNDWATER)) {
            immediate_warning_id = WARNING_GROUNDWATER_NEEDED;
            can_place = CAN_NOT_PLACE;
        }
    }
    if (special_flags & PlannerFlags::NearbyWater) {
        if (!map_terrain_exists_tile_in_radius_with_type(end, size.x, 3, TERRAIN_WATER)
            && !map_terrain_exists_tile_in_radius_with_type(end, size.x, 3, TERRAIN_FLOODPLAIN)) {
            immediate_warning_id = WARNING_WATER_NEEDED;
            can_place = CAN_NOT_PLACE;
        }
    }

    if (special_flags & PlannerFlags::Meadow) {
        if (!map_terrain_exists_tile_in_radius_with_type(end, size.x, 1, TERRAIN_MEADOW)
            && !map_terrain_all_tiles_in_radius_are(end.x(), end.y(), size.x, 0, TERRAIN_FLOODPLAIN)) {
            immediate_warning_id = WARNING_MEADOW_NEEDED;
            can_place = CAN_NOT_PLACE;
        }
    }

    if (special_flags & PlannerFlags::Rock) {
        if (!map_terrain_exists_tile_in_radius_with_type(end, size.x, 1, TERRAIN_ROCK)) {
            immediate_warning_id = WARNING_ROCK_NEEDED;
            can_place = CAN_NOT_PLACE;
        }
    }

    if (special_flags & PlannerFlags::Ore) {
        if (!map_terrain_exists_tile_in_radius_with_type(end, size.x, 1, TERRAIN_ORE)) {
            immediate_warning_id = WARNING_ROCK_NEEDED;
            can_place = CAN_NOT_PLACE;
        }
    }

    if (special_flags & PlannerFlags::Trees) {
        if (!map_terrain_exists_tile_in_radius_with_type(end, size.x, 1, TERRAIN_SHRUB | TERRAIN_TREE)) {
            immediate_warning_id = WARNING_TREE_NEEDED;
            can_place = CAN_NOT_PLACE;
        }
    }

    if (special_flags & PlannerFlags::Walls) {
        if (!map_terrain_all_tiles_in_radius_are(end.x(), end.y(), size.x, 0, TERRAIN_WALL)) {
            immediate_warning_id = WARNING_WALL_NEEDED;
            can_place = CAN_NOT_PLACE;
        }
    }

    if (!!(special_flags & PlannerFlags::IgnoreNearbyEnemy) == false) {
        if (has_nearby_enemy(start.x(), start.y(), end.x(), end.y())) {
            immediate_warning_id = WARNING_ENEMY_NEARBY;
            can_place = CAN_NOT_PLACE;
        }
    }
    if ((!!(special_flags & PlannerFlags::Road) && !!additional_req_param1) == true) {
        if (!map_terrain_is(end.grid_offset(), TERRAIN_ROAD)) {
            immediate_warning_id = additional_req_param2;
            can_place = CAN_NOT_PLACE;
        }
    }

    if ((!!(special_flags & PlannerFlags::Canals) && !!additional_req_param1) == true) {
        if (!map_terrain_is(end.grid_offset(), TERRAIN_CANAL)) {
            immediate_warning_id = additional_req_param2;
            can_place = CAN_NOT_PLACE;
        }
    }

    if (special_flags & PlannerFlags::FancyRoad) {
        if (!map_tiles_is_paved_road(end.grid_offset())) {
            can_place = CAN_NOT_PLACE;
        }
    }

    if (special_flags & PlannerFlags::RiverAccess) {
        if (!map_tile_is_connected_to_open_water(end)) {
            immediate_warning_id = WARNING_DOCK_OPEN_WATER_NEEDED;
        }
    }
}

void build_planner::update_special_case_orientations_check() {
    int dir_relative;

    // for special buildings that require oriented terrain
    if (special_flags & PlannerFlags::ShoreLine) {
        shore_orientation result = map_shore_determine_orientation(end, additional_req_param1, true);
        if (special_flags & PlannerFlags::FloodplainShore) {
            // in original Pharaoh, this actually is allowed to be built over the EDGE CORNERS.
            // it looks off, but it's legit!
            building_variant = 0;
            if (!result.match) {
                result = map_shore_determine_orientation(end, additional_req_param1, true, true, TERRAIN_FLOODPLAIN);
                if (result.match && !map_terrain_exists_tile_in_area_with_type(end, size.x, TERRAIN_WATER)) { // correct for water
                    building_variant = 1;
                } else {
                    result.match = false;
                }
            } else if (map_terrain_exists_tile_in_area_with_type(end, size.x, TERRAIN_FLOODPLAIN)) { // correct the ShoreLine check for floodplains!
                result.match = false;
            }
        }
        dir_relative = city_view_relative_orientation(result.orientation_absolute);
        if (!result.match) {
            immediate_warning_id = WARNING_SHORE_NEEDED;
            can_place = CAN_NOT_PLACE;
        } else if (relative_orientation != dir_relative) {
            relative_orientation = dir_relative;
            update_orientations(false);
        }
    }
    if (special_flags & PlannerFlags::Intersection) {
        bool match = map_orientation_for_venue_with_map_orientation(end, (e_venue_mode_orientation)additional_req_param1, &dir_relative);
        int city_direction = dir_relative / 2;
        if (!match) {
            immediate_warning_id = additional_req_param2;
            can_place = CAN_NOT_PLACE;
        } else if (relative_orientation != city_direction) {
            relative_orientation = city_direction;
            update_orientations(false);
        }
    }

    if (special_flags & PlannerFlags::TempleUpgrade) {
        building* target = building_at(end.grid_offset())->main();
        if (!building_at(end.grid_offset()) || !building_is_temple_complex(target->type)) {
            immediate_warning_id = WARNING_TEMPLE_UPGRADE_PLACEMENT_NEED_TEMPLE;
            can_place = CAN_NOT_PLACE;
        } else if (target->data.monuments.temple_complex_attachments & additional_req_param1) {
            immediate_warning_id = WARNING_TEMPLE_UPGRADE_ONLY_ONE;
            can_place = CAN_NOT_PLACE;
        } else {
            int dir_absolute = (5 - (target->data.monuments.variant / 2)) % 4;
            dir_relative = city_view_relative_orientation(dir_absolute);
            relative_orientation = (1 + dir_relative) % 2;
            end = temple_complex_part_target(target, additional_req_param1);
            update_orientations(false);
        }
    }
}

void build_planner::update_unique_only_one_check() {
    bool unique_already_placed = false;

    bool is_unique_building = building_impl::params(build_type).unique_building;
    if (is_unique_building) {
        unique_already_placed = building_count_total(build_type);
    }

    // for unique buildings - only one can be placed inside the mission
    switch (build_type) {
    case BUILDING_RECRUITER:
        if (city_buildings_has_recruiter() && !config_get(CONFIG_GP_CH_MULTIPLE_BARRACKS)) {
            unique_already_placed = true;
        }
        break;
    case BUILDING_TEMPLE_COMPLEX_OSIRIS:
    case BUILDING_TEMPLE_COMPLEX_RA:
    case BUILDING_TEMPLE_COMPLEX_PTAH:
    case BUILDING_TEMPLE_COMPLEX_SETH:
    case BUILDING_TEMPLE_COMPLEX_BAST:
        //        case BUILDING_TEMPLE_COMPLEX_ALTAR:
        //        case BUILDING_TEMPLE_COMPLEX_ORACLE:
        if (city_buildings_has_temple_complex() && !config_get(CONFIG_GP_CH_MULTIPLE_TEMPLE_COMPLEXES))
            unique_already_placed = true;
        break;

    default:
        ; // nothing
    }

    if (unique_already_placed) {
        immediate_warning_id = WARNING_ONE_BUILDING_OF_TYPE;
        can_place = CAN_NOT_PLACE;
    }
}

void build_planner::update_coord_caches() {
    vec2i view_tile = tile_to_pixel(end);
    if (view_tile.x == 0 && view_tile.y == 0)
        // this prevents graphics from being drawn on the top left corner
        // of the screen when the current "end" tile isn't valid.
        view_tile = camera_get_selected_screen_tile();
    int orientation = city_view_orientation() / 2;
    for (int row = 0; row < size.y; row++) {
        for (int column = 0; column < size.x; column++) {
            // get tile offset
            int x_offset = (column - pivot.x);
            int y_offset = (row - pivot.y);

            // get abs. tile
            tile2i tile;
            switch (orientation) {
            case 0:
                tile = end.shifted(x_offset, y_offset);
                break;
            case 1:
                tile = end.shifted(-y_offset, x_offset);
                break;
            case 2:
                tile = end.shifted(-x_offset, -y_offset);
                break;
            case 3:
                tile = end.shifted(y_offset, -x_offset);
                break;
            }

            // get tile pixel coords
            int current_x = view_tile.x + x_offset * 30 - y_offset * 30;
            int current_y = view_tile.y + x_offset * 15 + y_offset * 15;

            // save values in cache
            tile_coord_cache[row][column] = tile;
            pixel_coords_cache[row][column] = {current_x, current_y};
        }
    }
    switch (orientation) {
    case 0: // north
        north_tile = tile_coord_cache[0][0];
        east_tile = tile_coord_cache[0][size.x - 1];
        south_tile = tile_coord_cache[size.y - 1][size.x - 1];
        west_tile = tile_coord_cache[size.y - 1][0];
        break;
    case 1: // east
        north_tile = tile_coord_cache[size.y - 1][0];
        east_tile = tile_coord_cache[0][0];
        south_tile = tile_coord_cache[0][size.x - 1];
        west_tile = tile_coord_cache[size.y - 1][size.x - 1];
        break;
    case 2: // south
        north_tile = tile_coord_cache[size.y - 1][size.x - 1];
        east_tile = tile_coord_cache[size.y - 1][0];
        south_tile = tile_coord_cache[0][0];
        west_tile = tile_coord_cache[0][size.x - 1];
        break;
    case 3: // west
        north_tile = tile_coord_cache[0][size.x - 1];
        east_tile = tile_coord_cache[size.y - 1][size.x - 1];
        south_tile = tile_coord_cache[size.y - 1][0];
        west_tile = tile_coord_cache[0][0];
        break;
    }
}

void build_planner::update_orientations(bool check_if_changed) {
    int prev_orientation = relative_orientation;
    int prev_variant = building_variant;
    int global_rotation = building_rotation_global_rotation();

    const auto &params = building_impl::params(build_type);

    relative_orientation = params.planer_update_relative_orientation(*this, global_rotation);
    building_variant = params.planer_update_building_variant(*this);

    relative_orientation = relative_orientation % 4;
    absolute_orientation = city_view_absolute_orientation(relative_orientation);

    // do not refresh graphics if nothing changed
    if (check_if_changed && relative_orientation == prev_orientation && building_variant == prev_variant) {
        return;
    }

    setup_build_graphics(); // reload graphics, tiles, etc.
    update_coord_caches();  // refresh caches
}

void build_planner::construction_record_view_position(vec2i pixel, tile2i point) {
    if (point == start) {
        start_offset_screen_x = pixel.x;
        start_offset_screen_y = pixel.y;
    }
}

void build_planner::dispatch_warnings() {
    if (immediate_warning_id > -1)
        city_warning_show(immediate_warning_id);
    if (extra_warning_id > -1)
        city_warning_show(extra_warning_id);
}

int build_planner::get_total_drag_size(int* x, int* y) {
    if (!config_get(CONFIG_UI_SHOW_CONSTRUCTION_SIZE) || !(special_flags & PlannerFlags::Draggable)
        || (build_type != BUILDING_CLEAR_LAND && !total_cost)) {
        return 0;
    }
    int size_x = end.x() - start.x();
    int size_y = end.y() - start.y();
    if (size_x < 0)
        size_x = -size_x;

    if (size_y < 0)
        size_y = -size_y;

    size_x++;
    size_y++;
    *x = size_x;
    *y = size_y;
    return 1;
}

void build_planner::construction_start(tile2i tile) {
    start = end = tile;

    if (game_undo_start_build(build_type)) {
        in_progress = true;
        int can_start = true;
        switch (build_type) {
        case BUILDING_ROAD:
            can_start = map_routing_calculate_distances_for_building(ROUTED_BUILDING_ROAD, start);
            break;
        case BUILDING_IRRIGATION_DITCH:
            //            case BUILDING_WATER_LIFT:
            can_start = map_routing_calculate_distances_for_building(ROUTED_BUILDING_AQUEDUCT, start);
            break;
        case BUILDING_MUD_WALL:
            can_start = map_routing_calculate_distances_for_building(ROUTED_BUILDING_WALL, start);
            break;

        default:
            break;
        }

        if (!can_start) {
            construction_cancel();
        }
    }
}

void build_planner::construction_cancel() {
    map_property_clear_constructing_and_deleted();
    if (in_progress && special_flags & PlannerFlags::Draggable) {
        game_undo_restore_map(1);
        in_progress = false;
    } else {
        setup_build(BUILDING_NONE);
        //widget_sidebar_city_release_build_buttons();
    }

    custom_building_variant = 0;
    building_rotation_reset_rotation();
    update_orientations();
}

void build_planner::construction_update(tile2i tile) {
    end = tile;
    if (end == tile2i(-1, -1)) {
        return;
    }

    if (!build_type || city_finance_out_of_money()) {
        total_cost = 0;
        return;
    }

    map_property_clear_constructing_and_deleted();
    int current_cost = model_get_building(build_type)->cost;
    int global_rotation = building_rotation_global_rotation();
    int items_placed = 1;
    switch (build_type) {
    case BUILDING_CLEAR_LAND:
        last_items_cleared = building_construction_clear_land(true, start, end);
        items_placed = last_items_cleared;
        break;

    case BUILDING_MUD_WALL:
        items_placed = building_construction_place_wall(true, start, end);
        break;

    case BUILDING_ROAD:
        items_placed = building_road::place(true, start, end);
        break;

    case BUILDING_PLAZA:
        items_placed = building_plaza::place(start, end);
        break;

    case BUILDING_GARDENS:
        items_placed = building_garden::place(start, end);
        break;

    case BUILDING_IRRIGATION_DITCH:
        items_placed = building_construction_place_canal(true, start, end);
        map_tiles_update_all_canals(0);
        break;

    case BUILDING_LOW_BRIDGE:
    case BUILDING_UNUSED_SHIP_BRIDGE_83:
        items_placed = map_bridge_building_length();
        break;

    case BUILDING_HOUSE_VACANT_LOT:
        items_placed = place_houses(true, start.x(), start.y(), end.x(), end.y());
        break;

    case BUILDING_BRICK_GATEHOUSE:
    case BUILDING_MUD_GATEHOUSE:
        mark_construction(tile, 1, 3, ~TERRAIN_ROAD, false); // TODO
        break;

    case BUILDING_RESERVED_TRIUMPHAL_ARCH_56:
        mark_construction(tile, 3, 3, ~TERRAIN_ROAD, false);
        break;

    case BUILDING_STORAGE_YARD:
        mark_construction(tile, 3, 3, TERRAIN_ALL, false);
        break;

    case BUILDING_WATER_LIFT:
    case BUILDING_FISHING_WHARF:
    case BUILDING_TRANSPORT_WHARF:
    case BUILDING_SHIPWRIGHT:
    case BUILDING_DOCK:
    case BUILDING_WARSHIP_WHARF:
    case BUILDING_FERRY:
        draw_as_constructing = map_shore_determine_orientation(end, additional_req_param1, true).match;
        break;

    case BUILDING_FORT_ARCHERS:
    case BUILDING_FORT_CHARIOTEERS:
    case BUILDING_FORT_INFANTRY:
        if (formation_get_num_forts_cached() < 6) {
            vec2i offset = FORT_OFFSET[global_rotation][city_view_orientation() / 2];
            tile2i ground = tile.shifted(offset.x, offset.y);
            if (map_building_tiles_are_clear(tile, 3, TERRAIN_ALL)
                && map_building_tiles_are_clear(ground, 4, TERRAIN_ALL)) {
                mark_construction(tile, 3, 3, TERRAIN_ALL, false);
            }
        }
        break;

    default:
        if (special_flags & PlannerFlags::Meadow || special_flags & PlannerFlags::Rock
            || special_flags & PlannerFlags::Trees || special_flags & PlannerFlags::NearbyWater
            || special_flags & PlannerFlags::Walls || special_flags & PlannerFlags::Groundwater
            || special_flags & PlannerFlags::Water || special_flags & PlannerFlags::ShoreLine
            || special_flags & PlannerFlags::Road || special_flags & PlannerFlags::Intersection) {
            // never draw as constructing
        } else {
            if ((city_view_orientation() / 2) % 2 == 0) {
                mark_construction(north_tile, size.x, size.y, TERRAIN_ALL, true);
            } else {
                mark_construction(north_tile, size.y, size.x, TERRAIN_ALL, true);
            }
        }
    }

    if (items_placed >= 0) {
        current_cost *= items_placed;
    }
    
    total_cost = current_cost;
}

void build_planner::construction_finalize() { // confirm final placement
    in_progress = false;

    dispatch_warnings();
    if (can_place != CAN_PLACE && !building_is_draggable(build_type)) { // this is the FINAL check!
        return;
    }

    // attempt placing, restore terrain data on failure
    if (!place()) {
        map_property_clear_constructing_and_deleted();
        if (build_type == BUILDING_MUD_WALL || build_type == BUILDING_ROAD || build_type == BUILDING_IRRIGATION_DITCH)
            game_undo_restore_map(0);
        else if (build_type == BUILDING_PLAZA || build_type == BUILDING_GARDENS)
            game_undo_restore_map(1);
        else if (build_type == BUILDING_LOW_BRIDGE || build_type == BUILDING_UNUSED_SHIP_BRIDGE_83)
            map_bridge_reset_building_length();

        return;
    }

    // final generic building warnings - these are in another file
    // TODO: bring these warnings over.
    building_construction_warning_generic_checks(last_created_building, end, size.x, relative_orientation);
    bool should_recalc_ferry_routes = false;

    // update city building info with newly created
    // building for special/unique constructions
    switch (build_type) {
    case BUILDING_TEMPLE_COMPLEX_OSIRIS:
    case BUILDING_TEMPLE_COMPLEX_RA:
    case BUILDING_TEMPLE_COMPLEX_PTAH:
    case BUILDING_TEMPLE_COMPLEX_SETH:
    case BUILDING_TEMPLE_COMPLEX_BAST:
        city_buildings_add_temple_complex(last_created_building);
        break;

    case BUILDING_FERRY:
        should_recalc_ferry_routes = true;
        break;

    default:
        ; // nothing
    }

    setup_building_variant(end, build_type);
    update_orientations(false);

    // update terrain data for certain special cases
    if (special_flags & PlannerFlags::Meadow) {
        for (int y = end.y(); y < end.y() + size.y; y++) {
            for (int x = end.x(); x < end.x() + size.x; x++) {
                map_set_floodplain_growth(MAP_OFFSET(x, y), 0);
            }
        }
    }

    if (special_flags & PlannerFlags::Road) {
        map_terrain_add_in_area(end.x(), end.y(), end.x() + size.x - 1, end.y() + size.y - 1, TERRAIN_ROAD);
        map_tiles_update_area_roads(end.x(), end.y(), 5);
        map_tiles_update_all_plazas();
    }

    if (special_flags & PlannerFlags::Walls) {
        map_tiles_update_area_walls(end, 5);
    }

    // consume resources for specific buildings (e.g. marble, granite)
    if (special_flags & PlannerFlags::Resources) {
        city_storageyards_remove_resource((e_resource)additional_req_param1, additional_req_param2);
    }

    // finally, go over the rest of the stuff for all building types
    formation_move_herds_away(end);
    city_finance_process_construction(total_cost);
    game_undo_finish_build(total_cost);
    map_tiles_update_region_empty_land(false, start.shifted(-2, -2), end.shifted(size.x + 2, size.y + 2));
    map_routing_update_land();
    map_routing_update_walls();

    if (should_recalc_ferry_routes) {
        map_routing_update_ferry_routes();
    }
}

//////////////////////

void build_planner::update(tile2i cursor_tile) {
    end = cursor_tile;
    update_coord_caches();

    immediate_warning_id = -1;
    can_place = CAN_PLACE;
    update_obstructions_check();
    update_requirements_check();
    update_special_case_orientations_check();
    update_unique_only_one_check();
}

void build_planner::draw_flat_tile(vec2i pos, color color_mask, painter &ctx) {
    ImageDraw::img_generic(ctx, image_id_from_group(GROUP_TERRAIN_OVERLAY_COLORED), pos.x, pos.y, color_mask);
}

void build_planner::draw_blueprints(painter &ctx, bool fully_blocked) {
    vec2i pixel = pixel_coords_cache[0][0];
    switch (build_type) {
    case BUILDING_FORT_ARCHERS:
    case BUILDING_FORT_CHARIOTEERS:
    case BUILDING_FORT_INFANTRY:
        building_fort::ghost_preview(ctx, end, pixel, 0);
        return;

    default:
        for (int row = 0; row < size.y; row++) {
            for (int column = 0; column < size.x; column++) {
                vec2i current_coord = pixel_coords_cache[row][column];
                color color_mask = (tile_blocked_array[row][column] || fully_blocked) ? COLOR_MASK_RED_30 : COLOR_MASK_GREEN_30;
                draw_flat_tile(current_coord, color_mask, ctx);
            }
        }
        break;
    }
}

void build_planner::draw_bridge(map_point tile, vec2i pixel, int type, painter &ctx) {
    int length, direction;
    int end_grid_offset = map_bridge_calculate_length_direction(tile.x(), tile.y(), &length, &direction);

    int dir = direction - city_view_orientation();
    if (dir < 0)
        dir += 8;

    bool blocked = false;
    if (type == BUILDING_UNUSED_SHIP_BRIDGE_83 && length < 5)
        blocked = true;
    else if (!end_grid_offset)
        blocked = true;

    if (city_finance_out_of_money())
        blocked = true;

    int x_delta, y_delta;
    switch (dir) {
    case DIR_0_TOP_RIGHT:
        x_delta = 29;
        y_delta = -15;
        break;
    case DIR_2_BOTTOM_RIGHT:
        x_delta = 29;
        y_delta = 15;
        break;
    case DIR_4_BOTTOM_LEFT:
        x_delta = -29;
        y_delta = 15;
        break;
    case DIR_6_TOP_LEFT:
        x_delta = -29;
        y_delta = -15;
        break;
    default:
        return;
    }
    if (blocked) {
        draw_flat_tile(ctx, pixel, length > 0 ? COLOR_MASK_GREEN : COLOR_MASK_RED);
        if (length > 1)
            draw_flat_tile(ctx, pixel + vec2i(x_delta * (length - 1), y_delta * (length - 1)), COLOR_MASK_RED);

    } else {
        if (dir == DIR_0_TOP_RIGHT || dir == DIR_6_TOP_LEFT) {
            for (int i = length - 1; i >= 0; i--) {
                int sprite_id = map_bridge_get_sprite_id(i, length, dir, type == BUILDING_UNUSED_SHIP_BRIDGE_83);
                city_draw_bridge_tile(ctx, pixel.x + x_delta * i, pixel.y + y_delta * i, sprite_id, COLOR_MASK_GREEN);
            }
        } else {
            for (int i = 0; i < length; i++) {
                int sprite_id = map_bridge_get_sprite_id(i, length, dir, type == BUILDING_UNUSED_SHIP_BRIDGE_83);
                city_draw_bridge_tile(ctx, pixel.x + x_delta * i, pixel.y + y_delta * i, sprite_id, COLOR_MASK_GREEN);
            }
        }
    }
}

int build_planner::is_blocked_for_farm(tile2i tile, int size, blocked_tile_vec &blocked_tiles) {
    int orientation_index = city_view_orientation() / 2;
    int blocked = 0;
    int num_tiles = (size * size);
    for (int i = 0; i < num_tiles; i++) {
        const int offset = TILE_GRID_OFFSETS_PH[orientation_index][i];
        tile2i check_tile = tile.shifted(offset);
        bool tile_blocked = !map_terrain_is(check_tile, TERRAIN_FLOODPLAIN)
            || (map_building_at(check_tile) != 0)
            || map_has_figure_at(check_tile);

        blocked_tiles.push_back({ check_tile, tile_blocked });
        blocked += (tile_blocked ? 1 : 0);
    }
    return blocked;
}

int build_planner::is_blocked_for_building(tile2i tile, int size, blocked_tile_vec &blocked_tiles) {
    int orientation_index = city_view_orientation() / 2;
    int blocked = 0;
    int num_tiles = (size * size);
    for (int i = 0; i < num_tiles; i++) {
        int offset = TILE_GRID_OFFSETS_PH[orientation_index][i];
        tile2i check_tile = tile.shifted(offset);
        bool tile_blocked = map_terrain_is(check_tile, TERRAIN_NOT_CLEAR)
            || (map_terrain_count_directly_adjacent_with_type(check_tile, TERRAIN_FLOODPLAIN) > 0)
            || (map_terrain_count_diagonally_adjacent_with_type(check_tile, TERRAIN_FLOODPLAIN) > 0)
            || map_has_figure_at(check_tile);

        blocked_tiles.push_back({ check_tile, tile_blocked });
        blocked += (tile_blocked ? 1 : 0);
    }

    return blocked;
}

void build_planner::draw_partially_blocked(painter &ctx, int fully_blocked, const blocked_tile_vec &blocked_tiles) {
    for (auto &tile : blocked_tiles) {
        vec2i pixel = tile_to_pixel(tile.tile);
        draw_flat_tile(ctx, pixel, (fully_blocked || tile.blocked) ? COLOR_MASK_RED_30 : COLOR_MASK_GREEN_30);
    }
}

void build_planner::draw_flat_tile(painter &ctx, vec2i pixel, color color_mask) {
    ImageDraw::img_generic(ctx, image_id_from_group(GROUP_TERRAIN_OVERLAY_COLORED), pixel.x, pixel.y, color_mask);
}

void build_planner::draw(painter &ctx) {
    // empty building
    if (size.x < 1 || size.y < 1)
        return;

    if (can_place == CAN_NOT_PLACE) {
        // draw fully red (placement not allowed)
        draw_blueprints(ctx, true);
    } else if (tiles_blocked_total > 0) {
        // draw green blueprint with red (blocked) tiles
        draw_blueprints(ctx, false);
    } else if (!draw_as_constructing) {
        // draw normal building ghost (green)
        draw_graphics(ctx);
    }
}

bool build_planner::is_road_tile_for_canal(int grid_offset, int gate_orientation) {
    bool is_road = map_terrain_is(grid_offset, TERRAIN_ROAD);
    if (map_terrain_is(grid_offset, TERRAIN_BUILDING)) {
        building *b = building_at(grid_offset);
        if (b->type == BUILDING_MUD_GATEHOUSE) {
            if (b->subtype.orientation == gate_orientation)
                is_road = true;

        } else if (b->type == BUILDING_GRANARY) {
            if (map_routing_citizen_is_road(grid_offset))
                is_road = true;
        }
    }

    return is_road;
}

bool build_planner::map_is_straight_road_for_canal(int grid_offset) {
    int road_tiles_x = is_road_tile_for_canal(grid_offset + GRID_OFFSET(1, 0), 2)
        + is_road_tile_for_canal(grid_offset + GRID_OFFSET(-1, 0), 2);
    int road_tiles_y = is_road_tile_for_canal(grid_offset + GRID_OFFSET(0, -1), 1)
        + is_road_tile_for_canal(grid_offset + GRID_OFFSET(0, 1), 1);

    if (road_tiles_x == 2 && road_tiles_y == 0)
        return true;
    else if (road_tiles_y == 2 && road_tiles_x == 0)
        return true;
    else {
        return false;
    }
}

void build_planner::draw_canal(map_point tile, vec2i pixel, painter &ctx) {
    int grid_offset = tile.grid_offset();
    bool blocked = false;
    if (!map_can_place_initial_road_or_aqueduct(grid_offset, true)) {
        blocked = true;
    }
    if (in_progress) {   // already dragging aqueduct
        if (!total_cost) // ???
            blocked = true;
    } else {
        if (map_terrain_is(grid_offset, TERRAIN_ROAD)) {               // starting new aqueduct line
            blocked = !map_is_straight_road_for_canal(grid_offset); // can't start over a road curve!
            if (map_property_is_plaza_or_earthquake(grid_offset))      // todo: plaza not allowing aqueducts? maybe?
                blocked = true;
        } else if (map_terrain_is(grid_offset, TERRAIN_NOT_CLEAR)
            && !map_terrain_is(grid_offset, TERRAIN_FLOODPLAIN)) {// terrain is not clear!
            blocked = true;
        }
    }
    if (city_finance_out_of_money()) { // check sufficient funds to continue
        blocked = true;
    }

    if (blocked) { // cannot draw!
        draw_flat_tile(ctx, pixel, COLOR_MASK_RED);
    } else {
        const terrain_image *img = map_image_context_get_canal(grid_offset); // get starting tile
        draw_building_ghost(ctx, get_canal_image(grid_offset, map_terrain_is(grid_offset, TERRAIN_ROAD), 0, img), pixel);
    }
}

void build_planner::draw_building_ghost(painter &ctx, int image_id, vec2i pixel, color color_mask) {
    ImageDraw::isometric_from_drawtile(ctx, image_id, pixel, color_mask);
    ImageDraw::isometric_from_drawtile_top(ctx, image_id, pixel, color_mask, 1.f);
}

void build_planner::draw_entertainment_venue(tile2i tile, vec2i pixel, e_building_type type, painter &ctx) {
    int can_build = 0;

    int size = building_impl::params(type).building_size;
    int orientation = 0;

    switch (type) {
    case BUILDING_BOOTH:
        can_build = map_orientation_for_venue_with_map_orientation(tile, e_venue_mode_booth, &orientation);
        break;
    case BUILDING_BANDSTAND:
        can_build = map_orientation_for_venue_with_map_orientation(tile, e_venue_mode_bandstand, &orientation);
        break;
    case BUILDING_PAVILLION:
        can_build = map_orientation_for_venue_with_map_orientation(tile, e_venue_mode_pavilion, &orientation);
        break;
    case BUILDING_FESTIVAL_SQUARE:
        can_build = map_orientation_for_venue_with_map_orientation(tile, e_venue_mode_festival_square, &orientation);
        break;
    }
    // TODO: proper correct for map orientation (for now, just use a different orientation)
    orientation = abs(orientation + (8 - city_view_orientation())) % 8;

    if (can_build != 1) { // no can place
        for (int i = 0; i < size * size; i++) {
            draw_flat_tile(ctx, pixel + VIEW_OFFSETS[i], COLOR_MASK_RED);
        }
    } else { // can place (theoretically)
        switch (type) {
        case BUILDING_BOOTH:
            building_booth::ghost_preview(ctx, tile, pixel, orientation);
            break;

        case BUILDING_BANDSTAND:
            building_bandstand::ghost_preview(ctx, tile, pixel, orientation);
            break;

        case BUILDING_PAVILLION:
            building_pavilion::ghost_preview(ctx, tile, pixel, orientation);
            break;

        case BUILDING_FESTIVAL_SQUARE:
            building_festival_square::ghost_preview(ctx, tile, pixel, orientation);
            break;
        }
    }
}

void build_planner::draw_graphics(painter &ctx) {
    // TODO: bring these all over the unified system
    // special graphics buildings
    vec2i pixel = pixel_coords_cache[0][0];
    switch (build_type) {
    case BUILDING_ROAD:
        building_road::ghost_preview(end, pixel, ctx);
        return;

    case BUILDING_IRRIGATION_DITCH:
        draw_canal(end, pixel, ctx);
        return;
        //        case BUILDING_WALL_PH:
        //            return draw_walls((const map_tile*)&end, end_coord.x, end_coord.y);
        //            break;
    case BUILDING_STORAGE_YARD:
        building_storage_yard::ghost_preview(pixel, ctx);
        return;

    case BUILDING_BOOTH:
    case BUILDING_BANDSTAND:
    case BUILDING_PAVILLION:
    case BUILDING_FESTIVAL_SQUARE:
        draw_entertainment_venue(end, pixel, build_type, ctx);
        return;

    case BUILDING_SMALL_MASTABA:
        building_small_mastaba::ghost_preview(ctx, build_type, pixel, start, end);
        return;

    case BUILDING_MEDIUM_MASTABA:
        building_medium_mastaba::ghost_preview(ctx, build_type, pixel, start, end);
        return;

    case BUILDING_BARLEY_FARM:
    case BUILDING_FLAX_FARM:
    case BUILDING_GRAIN_FARM:
    case BUILDING_LETTUCE_FARM:
    case BUILDING_POMEGRANATES_FARM:
    case BUILDING_CHICKPEAS_FARM:
    case BUILDING_FIGS_FARM:
    case BUILDING_HENNA_FARM:
        building_farm::ghost_preview(ctx, build_type, pixel, end);
        return;

    case BUILDING_FORT_ARCHERS:
    case BUILDING_FORT_CHARIOTEERS:
    case BUILDING_FORT_INFANTRY:
        building_fort::ghost_preview(ctx, end, pixel, 0);
        return;

    case BUILDING_WELL:
        building_well::ghost_preview(ctx, end, pixel, 0);
        break;
    }

    // go through the tiles DIAGONALLY to render footprint and top correctly
    for (int dg_y = 0; dg_y < size.y + size.x - 1; dg_y++) {
        for (int dg_x = fmax(0, dg_y - size.y + 1); dg_x < size.x && dg_x < dg_y + 1; dg_x++) {
            // extract proper row and column index from the mess above
            int row = dg_y - dg_x;
            int column = dg_x;

            int image_id = tile_graphics_array[row][column];
            if (image_id > 0) {
                vec2i current_coord = pixel_coords_cache[row][column];
                ImageDraw::isometric_from_drawtile(ctx, image_id, current_coord, COLOR_MASK_GREEN);
                ImageDraw::isometric_from_drawtile_top(ctx, image_id, current_coord, COLOR_MASK_GREEN);
            }
        }
    }
}


bool build_planner::place() {
    if (end == tile2i(-1, -1)) {
        return false;
    }

    int x = end.x();
    int y = end.y();

    // for debugging...
    logs::info("Attempting to place at: %03i %03i %06i", x, y, MAP_OFFSET(x, y));

    // Check warnings for placement and create building/update tiles accordingly.
    // Some of the buildings below have specific warning messages (e.g. roadblocks)
    // that can't be easily put in `building_construction_can_place_on_terrain()`!
    int placement_cost = model_get_building(build_type)->cost;
    switch (build_type) {
    case BUILDING_CLEAR_LAND: {
            // BUG in original (keep this behaviour): if confirmation has to be asked (bridge/fort),
            // the previous cost is deducted from treasury and if user chooses 'no', they still pay for removal.
            // If we don't do it this way, the user doesn't pay for the removal at all since we don't come back
            // here when the user says yes.
            int items_placed = building_construction_clear_land(false, start, end);
            if (items_placed < 0)
                items_placed = last_items_cleared;
            placement_cost *= items_placed;
            map_property_clear_constructing_and_deleted();
        }
        break;

    case BUILDING_MUD_WALL:
        placement_cost *= building_construction_place_wall(false, start, end);
        break;

    case BUILDING_ROAD:
        placement_cost *= building_road::place(false, start, end);
        break;

    case BUILDING_PLAZA:
        placement_cost *= building_plaza::place(start, end);
        break;

    case BUILDING_GARDENS:
        placement_cost *= building_garden::place(start, end);
        map_routing_update_land();
        break;

    case BUILDING_LOW_BRIDGE:
    case BUILDING_UNUSED_SHIP_BRIDGE_83: {
        placement_cost *= map_bridge_add(x, y, build_type == BUILDING_UNUSED_SHIP_BRIDGE_83);
        break;
    }
    case BUILDING_IRRIGATION_DITCH: {
        placement_cost *= building_construction_place_canal(false, start, end);
        if (!placement_cost) {
            map_tiles_update_all_canals(0);
            map_routing_update_land();
        }
        break;
    }
    case BUILDING_HOUSE_VACANT_LOT:
        placement_cost *= place_houses(false, start.x(), start.y(), end.x(), end.y());
        if (placement_cost == 0) {
            city_warning_show(WARNING_CLEAR_LAND_NEEDED);
            return false;
        }
        break;
    case BUILDING_TEMPLE_COMPLEX_ALTAR:
    case BUILDING_TEMPLE_COMPLEX_ORACLE:
        if (!attach_temple_upgrade(additional_req_param1, end.grid_offset())) {
            return false;
        }
        break;

    default:
        if (!place_building(build_type, end, absolute_orientation, building_variant)) {
            return false;
        }
        break;
    }

    // TODO
    //    if (building_is_fort(building_type)) {
    //        const int offsets_x[] = {3, -1, -4, 0};
    //        const int offsets_y[] = {-1, -4, 0, 3};
    //        int orient_index = building_rotation_get_rotation();
    //        int x_offset = offsets_x[orient_index];
    //        int y_offset = offsets_y[orient_index];
    //        if (!map_tiles_are_clear(x + x_offset, y + y_offset, 4, terrain_mask)) {
    //            city_warning_show(WARNING_CLEAR_LAND_NEEDED);
    //            return 0;
    //        }
    //        if (formation_get_num_legions_cached() >= formation_get_max_legions()) {
    //            city_warning_show(WARNING_MAX_LEGIONS_REACHED);
    //            return 0;
    //        }
    //    }

    total_cost = placement_cost;
    if (total_cost == 0) {
        return false;
    }
    return true;
}