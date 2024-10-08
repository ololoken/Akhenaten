#include "window_building_info.h"

#include "building/building.h"
#include "city/object_info.h"
#include "window/building/common.h"
#include "js/js_game.h"

struct info_window_entertainment : public building_info_window_t<info_window_entertainment>{
    virtual void init(object_info &c) override;
    virtual bool check(object_info &c) override {
        building *b = c.building_get();
        return building_type_any_of(b->type, BUILDING_PAVILLION, BUILDING_JUGGLER_SCHOOL, BUILDING_CONSERVATORY);
    }
};

struct info_window_bandstand : public building_info_window_t<info_window_bandstand>{
    virtual void init(object_info &c) override;
    virtual bool check(object_info &c) override {
        return c.building_get()->dcast_bandstand();
    }
};

struct info_window_booth : public building_info_window_t<info_window_booth> {
    virtual void init(object_info &c) override;
    virtual bool check(object_info &c) override {
        building *b = c.building_get();
        return c.building_get()->dcast_booth();
    }
};

info_window_entertainment entertainment_infow;
info_window_bandstand bandstand_infow;
info_window_booth booth_infow;

void info_window_entertainment::init(object_info &c) {
    c.go_to_advisor.first = ADVISOR_ENTERTAINMENT;

    building_info_window::init(c);

    building *b = c.building_get();

    std::pair<int, int> reason = {c.group_id, 6};
    if (!c.has_road_access) { reason = {69, 25}; }
    else if (b->num_workers <= 0) { reason.second = 7; }
    else { reason.second = approximate_value(c.worker_percentage / 100.f, make_array(5, 4, 3, 2)); }
 
    ui["warning_text"] = ui::str(reason);
    fill_employment_details(c);
}

void info_window_bandstand::init(object_info &c) {
    building_info_window::init(c);

    building *b = c.building_get();

    textid reason{ c.group_id, 1 };
    if (!c.has_road_access) { reason = { 69, 25 }; } 
    else if (b->num_workers <= 0) { reason.id = 6; } 
    else if (!b->data.entertainment.num_shows) { reason.id = 2; }
    else if (b->data.entertainment.num_shows == 2) { reason.id = 3; } 
    else if (b->data.entertainment.days1) { reason.id = 5; } 
    else if (b->data.entertainment.days2) { reason.id = 4; }

    fill_employment_details(c);
    ui["warning_text"] = ui::str(reason);

    if (b->data.entertainment.days1 > 0) {
        ui["play_text"].text_var("%s %s %d", ui::str(c.group_id, 10), ui::str(8, 44), 2 * b->data.entertainment.days1);
    } else {
        ui["play_text"] = ui::str(c.group_id, 9);
    }
  
    if (b->data.entertainment.days2 > 0) {
        ui["play2_text"].text_var("%s %s %d\n%s", ui::str(c.group_id, 8), ui::str(8, 44), 2 * b->data.entertainment.days2, ui::str(72, 7 + b->data.entertainment.days3_or_play));
    } else {
        ui["play2_text"] = ui::str(c.group_id, 7);
    }
}

void info_window_booth::init(object_info &c) {
    building_info_window::init(c);

    building *b = c.building_get();

    textid reason{ c.group_id, 1 };
    if (!c.has_road_access) { reason = {69, 25}; } 
    else if (b->num_workers <= 0) { reason.id = 4; }
    else if (!b->data.entertainment.num_shows) { reason.id = 2; }
    else if (b->data.entertainment.days1) { reason.id = 3; }

    fill_employment_details(c);
    ui["warning_text"] = ui::str(reason);

    if (b->data.entertainment.days1 > 0) {
        ui["play_text"].text_var("%s %s %d", ui::str(c.group_id, 6), ui::str(8, 44), 2 * b->data.entertainment.days1);
    } else {
        ui["play_text"] = ui::str(c.group_id, 5);
    }

    ui["play2_text"] = "";
}
