#include "figures.h"
#include <graphics/view/lookup.h>

#include "building/building.h"
#include "empire/empire_city.h"
#include "figure/figure.h"
#include "figure/formation.h"
#include "figure/figure_phrase.h"
#include "figure/trader.h"
#include "graphics/image.h"
#include "graphics/graphics.h"
#include "graphics/elements/generic_button.h"
#include "graphics/elements/lang_text.h"
#include "graphics/elements/panel.h"
#include "graphics/text.h"
#include "graphics/view/view.h"
#include "graphics/window.h"
#include "config/config.h"
#include "scenario/scenario.h"
#include "widget/widget_city.h"
#include "dev/debug.h"
#include "game/game.h"

const int FIGURE_TYPE_TO_BIG_FIGURE_IMAGE[] = {
     8,  13, 13, 9,  4,  13, 8,  16, 7,  4, 18, 42, 26, 41, 8,  1,  33, 10, 11, 25, 8,  25, 15, 15, 15,
     60, 12, 14, 5,  52, 52, 2,  3,  6,  6, 13, 8,  8,  17, 12, 58, 21, 50, 8,  8,  8,  28, 30, 23, 8,
     8,  8,  34, 39, 33, 43, 27, 48, 63, 8, 8,  8,  8,  8,  53, 8,  38, 62, 54, 55, 56, 8,  8
};

int big_people_image(e_figure_type type) {
    int result = 0;
    int index = type;
    result = image_id_from_group(GROUP_PORTRAITS) + type;
    return result;
}

int inventory_to_resource_id(int value) {
    switch (value) {
    case 0:
        return RESOURCE_GRAIN;
    case 1:
        return RESOURCE_MEAT;
    case 2:
        return RESOURCE_LETTUCE;
    case 3:
        return RESOURCE_FIGS;
    case INVENTORY_GOOD4:
        return RESOURCE_BEER;
    case INVENTORY_GOOD3:
        return RESOURCE_MEAT;
    case INVENTORY_GOOD2:
        return RESOURCE_LUXURY_GOODS;
    case INVENTORY_GOOD1:
        return RESOURCE_POTTERY;
    default:
        return RESOURCE_NONE;
    }
}

int name_group_id() { // TODO
    return 254;
}

void figure::draw_animal(object_info* c) {
    painter ctx = game.painter();
    ImageDraw::img_generic(ctx, big_people_image(type), c->offset + vec2i{28, 112});
    lang_text_draw(64, type, c->offset.x + 92, c->offset.y + 139, FONT_NORMAL_BLACK_ON_DARK);
}

void figure::draw_normal_figure(object_info* c) {
    painter ctx = game.painter();
    int image_id = big_people_image(type);
    if (action_state == FIGURE_ACTION_74_FIREMAN_GOING_TO_FIRE || action_state == FIGURE_ACTION_75_FIREMAN_AT_FIRE) {
        image_id = image_id_from_group(GROUP_PORTRAITS) + 18;
    }

    ImageDraw::img_generic(ctx, image_id, c->offset + vec2i{28, 112});

    lang_text_draw(name_group_id(), name, c->offset.x + 90, c->offset.y + 108, FONT_LARGE_BLACK_ON_DARK);
    lang_text_draw(64, type, c->offset.x + 92, c->offset.y + 139, FONT_NORMAL_BLACK_ON_DARK);

    if (c->figure.phrase_group > 0 && c->figure.phrase_id >= 0) {
        lang_text_draw_multiline(c->figure.phrase_group, c->figure.phrase_id, c->offset + vec2i{90, 160}, 16 * (c->bgsize.x - 8), FONT_NORMAL_BLACK_ON_DARK);
    }
}