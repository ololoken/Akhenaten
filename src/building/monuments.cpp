#include "monuments.h"

#include "building/building.h"
#include "building/properties.h"
#include "building/model.h"
#include "building/count.h"
#include "graphics/image.h"
#include "graphics/image_groups.h"
#include "graphics/view/view.h"
#include "empire/empire_city.h"
#include "core/svector.h"
#include "city/resource.h"
#include "city/message.h"
#include "game/resource.h"
#include "grid/building_tiles.h"
#include "grid/terrain.h"
#include "core/calc.h"
#include "core/log.h"
#include "io/io_buffer.h"

#include "js/js_game.h"

#include <algorithm>

#define DELIVERY_ARRAY_SIZE_STEP 200
#define ORIGINAL_DELIVERY_BUFFER_SIZE 16
#define MODULES_PER_TEMPLE 2
#define ARCHITECTS RESOURCE_NONE

#define MAX_PHASES 6

#define BUILDING_MONUMENT_FIRST_ID BUILDING_SMALL_MASTABA

#define NOTHING 0
#define INFINITE 10000

grid_xx g_monuments_progress_grid = {0, {FS_UINT32, FS_UINT32}};

io_buffer* iob_monuments_progress_grid = new io_buffer([](io_buffer* iob, size_t version) {
    iob->bind(BIND_SIGNATURE_GRID, &g_monuments_progress_grid);
});

struct monument_phase_resource {
    int resource;
    uint16_t count;
};

struct monument_phase {
    std::array<monument_phase_resource, 6> resources;
};

struct monument {
    e_building_type btype;
    svector<monument_phase, MAX_PHASES> phases;
};

struct monument_mastaba : public monument {
    monument_mastaba() {
        btype = BUILDING_SMALL_MASTABA;
        phases.push_back({monument_phase_resource{ARCHITECTS, 1}, {RESOURCE_BRICKS, 3200}});
        phases.push_back({monument_phase_resource{ARCHITECTS, 1}, {RESOURCE_TIMBER, 1600}, {RESOURCE_BRICKS, 3200}});
        phases.push_back({monument_phase_resource{ARCHITECTS, 1}, {RESOURCE_TIMBER, 1600}, {RESOURCE_BRICKS, 3200}});
        phases.push_back({monument_phase_resource{ARCHITECTS, 4}, {RESOURCE_TIMBER, 3200}, {RESOURCE_BRICKS, 3200}, {RESOURCE_MARBLE, 48}});
        phases.push_back({monument_phase_resource{NOTHING, 0}});
    }
} g_monument_mastaba;

const monument *g_monument_types[] = {
    &g_monument_mastaba,
};

struct monument_delivery {
    int walker_id;
    int destination_id;
    int resource;
    int cartloads;
};

svector<monument_delivery, 32> g_monument_deliveries;

uint8_t map_monuments_get_progress(int grid_offset) {
    return map_grid_get(&g_monuments_progress_grid, grid_offset);
}
void map_monuments_set_progress(int grid_offset, int growth) {
    map_grid_set(&g_monuments_progress_grid, grid_offset, growth);
}

int building_monument_deliver_resource(building *b, e_resource resource) {
    if (b->id <= 0 || !building_monument_is_monument(b) ||
        b->data.monuments.resources[resource] <= 0) {
        return 0;
    }

    while (b->prev_part_building_id) {
        b = building_get(b->prev_part_building_id);
    }

    b->data.monuments.resources[resource]--;

    while (b->next_part_building_id) {
        b = building_get(b->next_part_building_id);
        b->data.monuments.resources[resource]--;
    }
    return 1;
}

int building_monument_access_point(building *b, tile2i *dst) {
    if (b->size < 3 || b->type == BUILDING_SMALL_MASTABA) {
        *dst = b->tile;
        return true;
    }

    int half_size = b->size / 2;

    *dst = b->tile.shifted(half_size, b->size - 1);
    return true;
}

int get_temple_complex_part_image(int type, int part, int orientation, int level) {
    if (level == 0) {
        switch (part) {
        case 0:
        return image_id_from_group(GROUP_BUILDING_TEMPLE_COMPLEX_MAIN, type) + 3 * orientation;
        case 1:
        return image_id_from_group(GROUP_BUILDING_TEMPLE_COMPLEX_ORACLE, type) + 3 * orientation;
        case 2:
        return image_id_from_group(GROUP_BUILDING_TEMPLE_COMPLEX_ALTAR, type) + 3 * orientation;
        }
    } else if (level == 1) {
        switch (part) {
        case 0:
        return image_id_from_group(GROUP_BUILDING_TEMPLE_COMPLEX_MAIN, type) + orientation;
        case 1:
        return image_id_from_group(GROUP_BUILDING_TEMPLE_COMPLEX_UPGRADES, type) + orientation;
        case 2:
        return image_id_from_group(GROUP_BUILDING_TEMPLE_COMPLEX_UPGRADES, type) + 2 + orientation;
        }
    }
    return 0;
}

int get_monument_part_image(int part, int orientation, int level) {
    level = std::clamp(level, 0, 11);

    int base_image_id = image_id_from_group(GROUP_MONUMENT_BLOCKS);
    return base_image_id;

    //switch (part) {
    //case MONUMENT_PART_CORNERS:
    //    return base_image_id + orientation + level * 8;
    //case MONUMENT_PART_SIDES:
    //    return base_image_id + orientation + level * 8 + 4;
    //case MONUMENT_PART_CENTER:
    //    if (level > 5)
    //        level = 5;
    //    return base_image_id + 96 + level + orientation * 6;
    //case MONUMENT_PART_EXTRA: // ramps, mastaba entrance, etc.
    //    return base_image_id + 108 + orientation;
    //case MONUMENT_PART_EXTERIORS: // TODO
    //    return image_id_from_group(GROUP_MONUMENT_EXTERIORS_END_DRY);
    //    break;
    //case MONUMENT_PART_CORNERS_2: // for bent pyramids
    //    if (level > 5)
    //        level = 5;
    //    return image_id_from_group(GROUP_MONUMENT_EXTRA_BLOCKS) + orientation + level * 8;
    //case MONUMENT_PART_SIDES_2: // for bent pyramids
    //    if (level > 5)
    //        level = 5;
    //    return image_id_from_group(GROUP_MONUMENT_EXTRA_BLOCKS) + orientation + level * 8 + 4;
    //}
}

int building_monument_add_module(building *b, int module) {
    //if (!building_monument_is_monument(b) ||
    //    b->data.monuments.phase != MONUMENT_FINISHED ||
    //    (b->data.monuments.upgrades && b->type != BUILDING_A && b->type != BUILDING_B)) {
    //    return 0;
    //}
    //b->data.monuments.upgrades = module;
    //map_building_tiles_add(b->id, b->tile, b->size, building_image_get(b), TERRAIN_BUILDING);
    return 1;
}

int building_monument_get_monument(tile2i tile, e_resource resource, int road_network_id, tile2i &dst) {
    if (city_resource_is_stockpiled(resource)) {
        return 0;
    }

    //int min_dist = INFINITE;
    //building *min_building = 0;
    //for (e_building_type type = BUILDING_MONUMENT_FIRST_ID; type < BUILDING_TYPE_MAX; type++) {
    //    if (!g_monument_types[type]) {
    //        continue;
    //    }
    //
    //    for (building *b = building_first_of_type(type); b; b = b->next_of_type) {
    //        if (b->data.monuments.phase == MONUMENT_FINISHED ||
    //            b->data.monuments.phase < MONUMENT_START ||
    //            building_monument_is_construction_halted(b) ||
    //            (!resource && building_monument_needs_resources(b))) {
    //            continue;
    //        }
    //        short needed = b->resources[resource];
    //        if ((needed - building_monument_resource_in_delivery(b, resource)) <= 0) {
    //            continue;
    //        }
    //        if (!map_has_road_access(b->tile, b->size) ||
    //            b->distance_from_entry <= 0 || b->road_network_id != road_network_id) {
    //            continue;
    //        }
    //        int dist = calc_maximum_distance(b->tile, tile);
    //        if (dist < min_dist) {
    //            min_dist = dist;
    //            min_building = b;
    //        }
    //    }
    //}
    //
    //if (min_building && min_dist < INFINITE) {
    //    map_point_store_result(min_building->road_access, dst);
    //    return min_building->id;
    //}
    return 0;
}

int building_monument_has_unfinished_monuments() {
    bool found = false;
    for (const auto &m: g_monument_types) {
        buildings_valid_do([&] (building &b) {
            if (b.data.monuments.phase != MONUMENT_FINISHED) {
                found |= true;
            }
        }, m->btype);
    }
    return found;
}

int building_monument_phases(e_building_type type) {
    auto m_it = std::find_if(std::begin(g_monument_types), std::end(g_monument_types), [&] (auto m) { return m->btype == type; });

    if (m_it == std::end(g_monument_types)) {
        return 0;
    }

    return (int)(*m_it)->phases.size();
}

bool building_monument_resources_needed_for_monument_type(e_building_type type, e_resource resource, int phase) {
    auto m_it = std::find_if(std::begin(g_monument_types), std::end(g_monument_types), [&] (auto m) { return m->btype == type; });

    if (m_it == std::end(g_monument_types)) {
        return false;
    }
    
    const monument_phase &ph = (*m_it)->phases[phase - 1];

    auto r_it = std::find_if(ph.resources.begin(), ph.resources.end(), [&] (auto &p) { return p.resource == resource; });
    return m_it != std::end(g_monument_types);
}

int building_image_get(building *b) {
    switch (b->type) {
    case BUILDING_SMALL_MASTABA:
        switch (b->data.monuments.phase) {
        case MONUMENT_START:
            return image_group(IMG_SMALL_MASTABA);
        default:
            return image_group(IMG_SMALL_MASTABA) + 1;
        }
    }

    return 0;
}

void building_monument_set_phase(building *b, int phase) {
    if (phase == building_monument_phases(b->type)) {
        phase = MONUMENT_FINISHED;
    }

    if (phase == b->data.monuments.phase) {
        return;
    }

    b->data.monuments.phase = phase;
    map_building_tiles_add(b->id, b->tile, b->size, building_image_get(b), TERRAIN_BUILDING);
    if (b->data.monuments.phase != MONUMENT_FINISHED) {
        for (e_resource resource = RESOURCE_NONE; resource < RESOURCES_MAX; ++resource) {
            b->data.monuments.resources[resource] = building_monument_resources_needed_for_monument_type(b->type, resource, b->data.monuments.phase);
        }
    }
}

bool building_monument_is_monument(const building *b) {
    return building_monument_type_is_monument(b->type);
}

bool building_monument_type_is_monument(e_building_type type) {
    return type > BUILDING_NONE && type < BUILDING_MAX
                && type == BUILDING_SMALL_MASTABA;
}

bool building_monument_type_is_mini_monument(e_building_type type) {
    return building_monument_type_is_monument(type) && building_properties_for_type(type)->size < 5;
}

bool building_monument_is_temple_complex(e_building_type type) {
    switch (type) {
    case BUILDING_TEMPLE_COMPLEX_OSIRIS:
    case BUILDING_TEMPLE_COMPLEX_RA:
    case BUILDING_TEMPLE_COMPLEX_PTAH:
    case BUILDING_TEMPLE_COMPLEX_SETH:
    case BUILDING_TEMPLE_COMPLEX_BAST:
        return 1;
    default:
        return 0;
    }
}

int building_monument_needs_resource(building *b, e_resource resource) {
    if (b->data.monuments.phase == MONUMENT_FINISHED) {
        return 0;
    }
    return (b->data.monuments.resources[resource]);
}

void building_monuments_set_construction_phase(int phase) {
    //for (building_type type = BUILDING_MONUMENT_FIRST_ID; type < BUILDING_TYPE_MAX; type++) {
    //    if (!MONUMENT_TYPES[type]) {
    //        continue;
    //    }
    //    for (building *b = building_first_of_type(type); b; b = b->next_of_type) {
    //        building_monument_set_phase(b, phase);
    //    }
    //}
}

void building_monument_finish_monuments() {
    bool found = false;
    for (const auto &m: g_monument_types) {
        buildings_valid_do([&] (building &b) {
            if (b.data.monuments.phase != MONUMENT_FINISHED) {
                return;
            }

            building_monument_set_phase(&b, MONUMENT_FINISHED);
            for (auto &r: b.data.monuments.resources) {
                r = 0;
            }
        }, m->btype);
    }
}

bool building_monument_needs_resources(building *b) {
    if (b->data.monuments.phase == MONUMENT_FINISHED) {
        return 0;
    }

    for (auto &r: b->data.monuments.resources) {
        if (r > 0) {
            return true;
        }
    }
    return false;
}

constexpr int MESSAGE_MONUMENT_COMPLETE = 171;
int building_monument_progress(building *b)
{
    if (building_monument_needs_resources(b)) {
        return 0;
    }
    if (b->data.monuments.phase == MONUMENT_FINISHED) {
        return 0;
    }

    while (b->prev_part_building_id) {
        b = building_get(b->prev_part_building_id);
    }

    building_monument_set_phase(b, b->data.monuments.phase + 1);

    while (b->next_part_building_id) {
        b = building_get(b->next_part_building_id);
        building_monument_set_phase(b, b->data.monuments.phase + 1);
    }
    if (b->data.monuments.phase == MONUMENT_FINISHED) {
        if (building_monument_is_temple_complex(b->type)) {
            city_message_post(1, MESSAGE_MONUMENT_COMPLETE, 0, b->tile.grid_offset());
        } else if (b->type == BUILDING_SMALL_MASTABA) {
            city_message_post(1, MESSAGE_MONUMENT_COMPLETE, 0, b->tile.grid_offset());
        }
    }
    return 1;
}

static int delivery_in_use(const monument_delivery *delivery) {
    return delivery->destination_id != 0;
}

void building_monument_initialize_deliveries(void) {
    //if (!array_init(monument_deliveries, DELIVERY_ARRAY_SIZE_STEP, 0, delivery_in_use)) {
    //    logs::error("Failed to create monument array. The game will likely crash.");
    //}
}

void building_monument_add_delivery(int monument_id, int figure_id, int resource_id, int num_loads) {
    g_monument_deliveries.push_back({0});
    monument_delivery &delivery = g_monument_deliveries.back();

    delivery.destination_id = monument_id;
    delivery.walker_id = figure_id;
    delivery.resource = resource_id;
    delivery.cartloads = num_loads;
}

bool building_monument_has_delivery_for_worker(int figure_id) {
    for(auto &delivery: g_monument_deliveries) {
        if (delivery.walker_id == figure_id && delivery.destination_id > 0) {
            return 1;
        }
    }
    return 0;
}

void building_monument_remove_delivery(int figure_id) {
    for(auto &delivery: g_monument_deliveries) {
        if (delivery.walker_id == figure_id) {
            delivery.destination_id = 0;
        }
    }

    auto &delv = g_monument_deliveries;
    delv.erase(std::remove_if(delv.begin(), delv.end(), [] (auto &d) { return !d.destination_id; }), delv.end());
}

void building_monument_remove_all_deliveries(int monument_id) {
    for(auto &delivery: g_monument_deliveries) {
        if (delivery.destination_id == monument_id) {
            delivery.destination_id = 0;
        }
    }

    auto &delv = g_monument_deliveries;
    delv.erase(std::remove_if(delv.begin(), delv.end(), [] (auto &d) { return !d.destination_id; }), delv.end());
}

int building_monument_resource_in_delivery(int monument_id, int resource_id) {
    int resources = 0;
    for(auto &delivery: g_monument_deliveries) {
        if (delivery.destination_id == monument_id &&
            delivery.resource == resource_id) {
            resources += delivery.cartloads;
        }
    }

    return resources;
}

static int resource_in_delivery_multipart(building *b, int resource_id) {
    int resources = 0;

    while (b->prev_part_building_id) {
        b = building_get(b->prev_part_building_id);
    }

    while (b->id) {
        for(auto &delivery: g_monument_deliveries) {
            if (delivery.destination_id == b->id &&
                delivery.resource == resource_id) {
                resources += delivery.cartloads;
            }
        }
        b = building_get(b->next_part_building_id);
    }

    return resources;
}

int building_monument_resource_in_delivery(building *b, int resource_id)
{
    if (b->next_part_building_id || b->prev_part_building_id) {
        return resource_in_delivery_multipart(b, resource_id);
    } else {
        return building_monument_resource_in_delivery(b->id, resource_id);
    }
}

int building_monument_get_id(e_building_type type) {
    building *b = building_first_of_type(type);
    if (!building_monument_type_is_monument(type) || !b) {
        return 0;
    }
    return b->id;
}

int building_monument_count_temple_complex(void) {
    int count = 0;
    static const e_building_type temple_complex[] = {
        BUILDING_TEMPLE_COMPLEX_OSIRIS,
        BUILDING_TEMPLE_COMPLEX_RA,
        BUILDING_TEMPLE_COMPLEX_PTAH,
        BUILDING_TEMPLE_COMPLEX_SETH,
        BUILDING_TEMPLE_COMPLEX_BAST,
    };
    for (auto &type: temple_complex) {
        count += building_count_active(type);
    }
    return count;
}

int building_monument_has_labour_problems(building *b)
{
    const model_building *model = model_get_building(b->type);

    if (b->num_workers < model->laborers) {
        return 1;
    } else {
        return 0;
    }
}

int building_monument_working(e_building_type type) {
    int monument_id = building_monument_get_id(type);
    building *b = building_get(monument_id);
    if (!monument_id) {
        return 0;
    }
    if (b->data.monuments.phase != MONUMENT_FINISHED || b->state != BUILDING_STATE_VALID) {
        return 0;
    }

    if (building_monument_has_labour_problems(b)) {
        return 0;
    }

    return monument_id;
}

bool building_monument_requires_resource(e_building_type type, e_resource resource) {
    int phases = building_monument_phases(type);
    for (int phase = 1; phase < phases; phase++) {
        if (building_monument_resources_needed_for_monument_type(type, resource, phase)) {
            return true;
        }
    }
    return false;
}

int building_monument_has_required_resources_to_build(e_building_type type) {
    int phases = building_monument_phases(type);
    for (int phase = 1; phase < phases; phase++) {
        for (e_resource r = RESOURCE_MIN; r < RESOURCES_MAX; ++r) {
            if (building_monument_resources_needed_for_monument_type(type, r, phase) &&
                !empire_can_produce_resource(r, false) && !empire_can_produce_resource(r, false)) {
                return 0;
            }
        }
    }
    return 1;
}

int building_monument_upgraded(e_building_type type) {
    int monument_id = building_monument_working(type);
    building *b = building_get(monument_id);
    if (!monument_id) {
        return 0;
    }
    if (!b->data.monuments.upgrades) {
        return 0;
    }
    return monument_id;
}

int building_monument_module_type(e_building_type type) {
    int monument_id = building_monument_working(type);

    if (!monument_id) {
        return 0;
    }

    building *b = building_get(monument_id);
    return b->data.monuments.upgrades;
}

io_buffer *iob_city_building_monuments = new io_buffer([] (io_buffer *iob, size_t version) {
    int delivers_size = (int)g_monument_deliveries.size();
    iob->bind(BIND_SIGNATURE_INT32, &delivers_size);
    for (int i = 0; i < g_monument_deliveries.capacity(); ++i) {
        monument_delivery &delivery = g_monument_deliveries[i];
        iob->bind(BIND_SIGNATURE_INT32, &delivery.walker_id);
        iob->bind(BIND_SIGNATURE_INT32, &delivery.destination_id);
        iob->bind(BIND_SIGNATURE_INT32, &delivery.resource);
        iob->bind(BIND_SIGNATURE_INT32, &delivery.cartloads);
    }
});

int building_monument_is_construction_halted(building *b) {
    return b->main()->state == BUILDING_STATE_MOTHBALLED;
}

int building_monument_toggle_construction_halted(building *b) {
    if (b->state == BUILDING_STATE_MOTHBALLED) {
        b->state = BUILDING_STATE_VALID;
        return 0;
    } else {
        b->state = BUILDING_STATE_MOTHBALLED;
        return 1;
    }
}

int building_monument_is_unfinished_monument(const building *b) {
    return building_monument_is_monument(b) && b->data.monuments.phase != MONUMENT_FINISHED;
}