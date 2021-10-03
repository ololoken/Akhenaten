#include "water_supply.h"

#include "building/building.h"
#include "building/list.h"
#include "core/image.h"
#include "core/game_environment.h"
#include "map/aqueduct.h"
#include "map/building_tiles.h"
#include "map/data.h"
#include "map/desirability.h"
#include "map/building.h"
#include "map/grid.h"
#include "map/image.h"
#include "map/property.h"
#include "map/terrain.h"
#include "scenario/property.h"
#include "tiles.h"

#include <string.h>

//#define OFFSET(x,y) (x + grid_size[GAME_ENV] * y)

#define MAX_QUEUE 1000

//static const int ADJACENT_OFFSETS[] = {-GRID_SIZE, 1, GRID_SIZE, -1};

static struct {
    int items[MAX_QUEUE];
    int head;
    int tail;
} queue;

static void mark_well_access(int well_id, int radius) {
    building *well = building_get(well_id);
    int x_min, y_min, x_max, y_max;
    map_grid_get_area(well->x, well->y, 1, radius, &x_min, &y_min, &x_max, &y_max);

    for (int yy = y_min; yy <= y_max; yy++) {
        for (int xx = x_min; xx <= x_max; xx++) {
            int building_id = map_building_at(map_grid_offset(xx, yy));
            if (building_id)
                building_get(building_id)->has_well_access = 1;

        }
    }
}

void map_water_supply_update_houses(void) {
    building_list_small_clear();
    for (int i = 1; i < MAX_BUILDINGS[GAME_ENV]; i++) {
        building *b = building_get(i);
        if (b->state != BUILDING_STATE_VALID)
            continue;

        if (b->type == BUILDING_WELL)
            building_list_small_add(i);
        else if (b->house_size) {
            b->has_water_access = false;
            b->has_well_access = 0;
            if (b->data.house.bathhouse || map_terrain_exists_tile_in_area_with_type(
                    b->x, b->y, b->size, TERRAIN_FOUNTAIN_RANGE)) {
                b->has_water_access = true;
            }
        }
    }
    int total_wells = building_list_small_size();
    const int *wells = building_list_small_items();
    for (int i = 0; i < total_wells; i++) {
        if (GAME_ENV == ENGINE_ENV_C3)
            mark_well_access(wells[i], 2);
        else if (GAME_ENV == ENGINE_ENV_PHARAOH)
            mark_well_access(wells[i], 1);
    }
}

const static int aqueduct_image_water_diff = 48;
static void aqueducts_empty_all(void) {
    int image_without_water = image_id_from_group(GROUP_BUILDING_AQUEDUCT) + aqueduct_image_water_diff;
    int grid_offset = map_data.start_offset;
    for (int y = 0; y < map_data.height; y++, grid_offset += map_data.border_size) {
        for (int x = 0; x < map_data.width; x++, grid_offset++) {
            if (map_terrain_is(grid_offset, TERRAIN_AQUEDUCT)) {
//                int water_level = map_aqueduct_at(grid_offset);
//                water_level--;
//                if (water_level < 0)
//                    water_level = 0;
                map_aqueduct_set(grid_offset, 0);
                int image_id = map_image_at(grid_offset);
//                if (image_id < image_without_water && water_level > 0)
//                    map_image_set(grid_offset, image_id + aqueduct_image_water_diff);
                if (image_id < image_without_water)
                    map_image_set(grid_offset, image_id + aqueduct_image_water_diff);
            }
        }
    }
}

static void fill_aqueducts_from_offset(int grid_offset) {
    if (!map_terrain_is(grid_offset, TERRAIN_AQUEDUCT))
        return;
    memset(&queue, 0, sizeof(queue));
    int guard = 0;
    int next_offset;
    int image_without_water = image_id_from_group(GROUP_BUILDING_AQUEDUCT) + aqueduct_image_water_diff;
    do {
        if (++guard >= grid_total_size[GAME_ENV])
            break;

        map_aqueduct_set(grid_offset, 1);
        int image_id = map_image_at(grid_offset);
        if (image_id >= image_without_water)
            map_image_set(grid_offset, image_id - aqueduct_image_water_diff);
        map_terrain_add_with_radius(map_grid_offset_to_x(grid_offset), map_grid_offset_to_y(grid_offset), 1, 2, TERRAIN_IRRIGATION_RANGE);

        next_offset = -1;
        for (int i = 0; i < 4; i++) {
            const int ADJACENT_OFFSETS[] = {-grid_size[GAME_ENV], 1, grid_size[GAME_ENV], -1};
            int new_offset = grid_offset + ADJACENT_OFFSETS[i];
            building *b = building_at(new_offset);
            if (b->id && b->type == BUILDING_WATER_LIFT) {
                // check if aqueduct connects to reservoir --> doesn't connect to corner
                int xy = map_property_multi_tile_xy(new_offset);
                if (xy != EDGE_X0Y0 && xy != EDGE_X2Y0 && xy != EDGE_X0Y2 && xy != EDGE_X2Y2) {
                    if (!b->has_water_access)
                        b->has_water_access = 2;
                }
            } else if (map_terrain_is(new_offset, TERRAIN_AQUEDUCT)) {
                if (!map_aqueduct_at(new_offset)) {
                    if (next_offset == -1)
                        next_offset = new_offset;
                    else {
                        queue.items[queue.tail++] = new_offset;
                        if (queue.tail >= MAX_QUEUE)
                            queue.tail = 0;
                    }
                }
            }
        }
        if (next_offset == -1) {
            if (queue.head == queue.tail)
                return;
            next_offset = queue.items[queue.head++];
            if (queue.head >= MAX_QUEUE)
                queue.head = 0;
        }
        grid_offset = next_offset;
    } while (next_offset > -1);
}

static int OFFSET(int x, int y) {
    switch (GAME_ENV) {
        case ENGINE_ENV_C3:
            return OFFSET_C3(x, y);
            break;
        case ENGINE_ENV_PHARAOH:
            return OFFSET_PH(x, y);
            break;
    }
}

void map_water_supply_update_aqueducts(void) {
    // first, reset all aqueducts (set to empty)
    map_terrain_remove_all(TERRAIN_IRRIGATION_RANGE);
    aqueducts_empty_all();
    building_list_large_clear(1);

    // cached grid offsets for water lift outputs
    const int CONNECTOR_OFFSETS[4][2] = {
            { OFFSET(0, 2), OFFSET(1, 2) },
            { OFFSET(-1, 0), OFFSET(-1, 1) },
            { OFFSET(0, -1), OFFSET(1, -1) },
            { OFFSET(2, 0), OFFSET(2, 1) }
    };

    // mark reservoirs next to water
    for (int i = 1; i < MAX_BUILDINGS[GAME_ENV]; i++) {
        building *b = building_get(i);
        if (b->state == BUILDING_STATE_VALID && b->type == BUILDING_WATER_LIFT) {
            building_list_large_add(i);
            if (map_terrain_exists_tile_in_radius_with_type(b->x, b->y, 2, 1, TERRAIN_WATER)) {
                b->has_water_access = true;
                fill_aqueducts_from_offset(b->grid_offset + CONNECTOR_OFFSETS[b->data.industry.orientation][0]);
                fill_aqueducts_from_offset(b->grid_offset + CONNECTOR_OFFSETS[b->data.industry.orientation][1]);
                map_terrain_add_with_radius(b->x, b->y, 2, 4, TERRAIN_IRRIGATION_RANGE);
            } else
                b->has_water_access = false;
        }
    }

//    int total_reservoirs = building_list_large_size();
//    const int *reservoirs = building_list_large_items();
//    // fill reservoirs from full ones
//    int changed = 1;
//    const int CONNECTOR_OFFSETS[] = {OFFSET(1, -1), OFFSET(3, 1), OFFSET(1, 3), OFFSET(-1, 1)};
//    while (changed == 1) {
//        changed = 0;
//        for (int i = 0; i < total_reservoirs; i++) {
//            building *b = building_get(reservoirs[i]);
//            if (b->has_water_access == 2) {
//                b->has_water_access = true;
//                changed = 1;
//                for (int d = 0; d < 4; d++)
//                    fill_aqueducts_from_offset(b->grid_offset + CONNECTOR_OFFSETS[d]);
//            }
//        }
//    }
    // mark reservoir ranges
//    for (int i = 0; i < total_reservoirs; i++) {
//        building *b = building_get(reservoirs[i]);
//        if (b->has_water_access)
//            map_terrain_add_with_radius(b->x, b->y, 3, 10, TERRAIN_IRRIGATION_RANGE);
//    }
}
void map_water_supply_update_wells_PH(void) {
    map_terrain_remove_all(TERRAIN_FOUNTAIN_RANGE);
    int total_wells = building_list_small_size();
    const int *wells = building_list_small_items();
    for (int i = 0; i < total_wells; i++) {
        building *b = building_get(wells[i]);
        if (b->type == BUILDING_WELL)
            map_terrain_add_with_radius(b->x, b->y, 1, 3, TERRAIN_FOUNTAIN_RANGE);
    }
}

int map_water_supply_is_well_unnecessary(int well_id, int radius) {
    building *well = building_get(well_id);
    int num_houses = 0;
    int x_min, y_min, x_max, y_max;
    map_grid_get_area(well->x, well->y, 1, radius, &x_min, &y_min, &x_max, &y_max);

    for (int yy = y_min; yy <= y_max; yy++) {
        for (int xx = x_min; xx <= x_max; xx++) {
            int grid_offset = map_grid_offset(xx, yy);
            int building_id = map_building_at(grid_offset);
            if (building_id && building_get(building_id)->house_size && !building_get(building_id)->data.house.bathhouse) {
                num_houses++;
//                if (!building_get(building_id)->has_water_access) //todo: water carrier access
                    return WELL_NECESSARY;
            }
        }
    }
    return num_houses ? WELL_UNNECESSARY_FOUNTAIN : WELL_UNNECESSARY_NO_HOUSES;
}
