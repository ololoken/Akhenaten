#include "advisor_imperial.h"

#include "city/city.h"
#include "city/finance.h"
#include "city/military.h"
#include "city/ratings.h"
#include "city/city_resource.h"
#include "empire/empire.h"
#include "figure/formation_legion.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "graphics/elements/generic_button.h"
#include "graphics/elements/lang_text.h"
#include "graphics/elements/panel.h"
#include "graphics/text.h"
#include "graphics/window.h"
#include "scenario/scenario.h"
#include "scenario/request.h"
#include "window/donate_to_city.h"
#include "window/window_empire.h"
#include "window/window_gift_to_kingdome.h"
#include "window/popup_dialog.h"
#include "window/set_salary.h"
#include "graphics/screen.h"
#include "game/game.h"

ui::advisor_imperial_window g_advisor_imperial_window;

ANK_REGISTER_CONFIG_ITERATOR(config_load_advisor_imperial);
void config_load_advisor_imperial() {
    g_config_arch.r_section("advisor_imperial_window", [] (archive arch) {
        g_advisor_imperial_window.load(arch);
    });
}

enum E_STATUS {
    STATUS_NOT_ENOUGH_RESOURCES = -1,
    STATUS_CONFIRM_SEND_LEGIONS = -2,
    STATUS_NO_LEGIONS_SELECTED = -3,
    STATUS_NO_LEGIONS_AVAILABLE = -4,
};

static void button_request(int index, int param2);

static generic_button imperial_buttons[] = {
  {38, 96, 560, 40, button_request, button_none, 0, 0},
  {38, 138, 560, 40, button_request, button_none, 1, 0},
  {38, 180, 560, 40, button_request, button_none, 2, 0},
  {38, 222, 560, 40, button_request, button_none, 3, 0},
  {38, 264, 560, 40, button_request, button_none, 4, 0},
};

static int focus_button_id;

static int get_request_status(int index) {
    const scenario_request* request = scenario_request_get_visible(index);
    if (request) {
        if (request->resource == RESOURCE_DEBEN) {
            if (city_finance_treasury() <= request->amount) {
                return STATUS_NOT_ENOUGH_RESOURCES;
            }
        } else if (request->resource == RESOURCE_TROOPS) {
            if (city_military_months_until_distant_battle() > 0 && !city_military_distant_battle_kingdome_army_is_traveling_forth()) {
                if (g_city.military.total_batalions <= 0) {
                    return STATUS_NO_LEGIONS_AVAILABLE;
                } else if (g_city.military.kingdome_service_batalions <= 0) {
                    return STATUS_NO_LEGIONS_SELECTED;
                } else {
                    return STATUS_CONFIRM_SEND_LEGIONS;
                }
            }
        } else {
            if (city_resource_count((e_resource)request->resource) < request->resource_amount()) {
                return STATUS_NOT_ENOUGH_RESOURCES;
            }
        }
        return request->event_id;
    }
    return 0;
}

void ui::advisor_imperial_window::draw_foreground() {
}

int ui::advisor_imperial_window::draw_background() {
    g_city.kingdome.calculate_gift_costs();

    auto &ui = g_advisor_imperial_window;
    ui["header_label"].text((pcstr)city_player_name());
    ui["rating_label"].text_var("%s %u", ui::str(52, 0), g_city.ratings.kingdom);
    ui["rating_advice"].text_var(ui::str(52, g_city.ratings.kingdom / 5 + 22));
    ui["player_rank"].text(ui::str(32, g_city.kingdome.player_rank));
    ui["personal_savings"].text_var("%s %u %s", ui::str(52, 1), g_city.kingdome.personal_savings, ui::str(6, 0));
    ui["salary_rank"].text_var("%s %u %s", ui::str(52, g_city.kingdome.salary_rank + 4), g_city.kingdome.salary_amount, ui::str(52, 3));

    ui["donate_to_city"].onclick([] { window_donate_to_city_show(); });
    ui["salary_rank"].onclick([] { window_set_salary_show(); });
    ui["send_gift"].onclick([] { window_gift_to_kingdome_show(); });

    return 0;
}

void ui::advisor_imperial_window::ui_draw_foreground() {
    auto &ui = g_advisor_imperial_window;

    ui.begin_widget(screen_dialog_offset());
    ui.draw();

    int num_requests = 0;
    if (city_military_months_until_distant_battle() > 0
        && !city_military_distant_battle_kingdome_army_is_traveling_forth()) {
        
        // can send to distant battle
        ui.button("", vec2i{38, 96}, vec2i{560, 40}, FONT_NORMAL_WHITE_ON_DARK);
        ui.icon(vec2i{50, 106}, RESOURCE_WEAPONS);

        bstring128 distant_battle_text(ui::str(52, 72), ui::str(21, g_empire.city(city_military_distant_battle_city())->name_id));
        ui.label(distant_battle_text, vec2i{80, 102}, FONT_NORMAL_WHITE_ON_DARK);

        int strength_text_id;
        int enemy_strength = city_military_distant_battle_enemy_strength();
        if (enemy_strength < 46) {
            strength_text_id = 73;
        } else if (enemy_strength < 89) {
            strength_text_id = 74;
        } else {
            strength_text_id = 75;
        }

        bstring128 distant_strenght_text;
        distant_strenght_text.printf("%s %s %d", ui::str(52, strength_text_id), ui::str(8, 4), city_military_months_until_distant_battle());
        ui.label(distant_strenght_text, vec2i{80, 120}, FONT_NORMAL_WHITE_ON_DARK);
        num_requests = 1;
    }

    num_requests = scenario_request_foreach_visible(num_requests, [&ui] (int index, const scenario_request* request) {
        if (index >= 5) {
            return;
        }

        ui.button("", vec2i{38, 96 + 42 * index}, vec2i{560, 42});
        ui.icon(vec2i{45, 103 + 42 * index}, request->resource);

        int request_amount = request->resource_amount();

        int quat = stack_proper_quantity(request_amount, request->resource);
        bstring256 amount_text;
        amount_text.printf("%u %s", quat, ui::str(23, request->resource));
        ui.label(amount_text, vec2i{65, 102 + 42 * index}, FONT_NORMAL_WHITE_ON_DARK);

        bstring256 month_to_comply;
        month_to_comply.printf("%s %u %s", ui::str(8, 4), request->months_to_comply, ui::str(12, 2));
        ui.label(month_to_comply, vec2i{310, 102 + 42 * index}, FONT_NORMAL_WHITE_ON_DARK);

        if (request->resource == RESOURCE_DEBEN) {
            // request for money
            int treasury = city_finance_treasury();
            bstring256 saved_deben;
            pcstr allow_str = (treasury < request->amount) ? ui::str(52, 48) : ui::str(52, 47);
            saved_deben.printf("%u %s %s", treasury, ui::str(52, 44), allow_str);
            ui.label(saved_deben, vec2i{40, 120 + 42 * index}, FONT_NORMAL_WHITE_ON_DARK);
        } else {
            // normal goods request
            int amount_stored = city_resource_count(request->resource);
            int request_amount = request->resource_amount();
            bstring256 saved_deben;
            pcstr allow_str = (amount_stored < request_amount) ? ui::str(52, 48) : ui::str(52, 47);
            saved_deben.printf("%u %s %s", amount_stored, ui::str(52, 43), allow_str);
            ui.label(saved_deben, vec2i{40, 120 + 42 * index}, FONT_NORMAL_WHITE_ON_DARK);
        }
    });

    if (!num_requests) {
        ui.label(ui::str(52, 21), vec2i{64, 160}, FONT_NORMAL_WHITE_ON_DARK, UiFlags_LabelMultiline, 512);
    }
}

int ui::advisor_imperial_window::handle_mouse(const mouse* m) {
    return 0;
}

static void confirm_nothing(bool accepted) {}

static void confirm_send_troops(bool accepted) {
    if (accepted) {
        formation_legions_dispatch_to_distant_battle();
        window_empire_show();
    }
}

static void button_request(int index, int param2) {
    int status = get_request_status(index);
    // in C3, the enums are offset by two! (I have not fixed this)
    if (status) {
        g_city.military.clear_kingdome_service_batalions();
        switch (status) {
        case STATUS_NO_LEGIONS_AVAILABLE:
            window_ok_dialog_show("#popup_dialog_no_legions_available");
            break;
        case STATUS_NO_LEGIONS_SELECTED:
            window_ok_dialog_show("#popup_dialog_no_legions_selected");
            break;
        case STATUS_CONFIRM_SEND_LEGIONS:
            window_ok_dialog_show("#popup_dialog_send_troops");
            break;
        case STATUS_NOT_ENOUGH_RESOURCES:
            window_ok_dialog_show("#popup_dialog_not_enough_goods");
            break;
        default:
            window_yes_dialog_show("#popup_dialog_send_goods", [selected_request_id = index] {
                scenario_request_dispatch(selected_request_id);
            });
            break;
        }
    }
}

int ui::advisor_imperial_window::get_tooltip_text(void) {
    if (focus_button_id && focus_button_id <= 2)
        return 93 + focus_button_id;
    else if (focus_button_id == 3)
        return 131;
    else {
        return 0;
    }
}

advisor_window* ui::advisor_imperial_window::instance() {
    return &g_advisor_imperial_window;
}
