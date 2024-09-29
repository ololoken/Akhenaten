#include "ui.h"

#include "button.h"
#include "core/string.h"
#include "generic_button.h"
#include "arrow_button.h"
#include "image_button.h"
#include "rich_text.h"
#include "lang_text.h"
#include "panel.h"
#include "graphics/text.h"
#include "core/svector.h"
#include "core/log.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "graphics/elements/tooltip.h"
#include "graphics/image.h"
#include "core/span.hpp"
#include "resource/icons.h"
#include "io/gamefiles/lang.h"
#include "js/js_game.h"

#include <stack>

namespace ui {
    tooltip_context tooltipctx;

    const tooltip_context &get_tooltip() {
        return tooltipctx;
    }

    void set_tooltip(const xstring &text) {
        tooltipctx.text = text;
    }

    struct universal_button {
        enum e_btn_type {
            unknown = -1,
            generic = 0,
            image,
            arrow
        };
        e_btn_type type = unknown;
        generic_button g_button;
        image_button i_button;
        arrow_button a_button;
    
        bool handle_mouse(const mouse *m, vec2i offset) {
            int tmp_btn;
            switch (type) {
            case generic: return !!generic_buttons_handle_mouse(m, offset, &g_button, 1, &tmp_btn);
            case image: return !!image_buttons_handle_mouse(m, offset, &i_button, 1, &tmp_btn);
            case arrow: return !!arrow_buttons_handle_mouse(m, offset, &a_button, 1, &tmp_btn);
            }

            return false;
        }

        pcstr tooltip() const {
            switch (type) {
            case generic: return g_button._tooltip.c_str();
            case image: return i_button._tooltip.c_str(); 
            }

            return "";
        }

        vec2i pos() const {
            switch (type) {
            case generic: return { g_button.x, g_button.y };
            case image: return { i_button.x, i_button.y };
            case arrow: return { a_button.x, a_button.y };
            }

            return {0, 0};
        }

        vec2i size() const {
            switch (type) {
            case generic: return g_button.size();
            case image: return { i_button.width, i_button.height };
            case arrow: return a_button.size();
            }

            return {0, 0};
        }

        template<class Func> void onclick(Func f) { 
            switch (type) {
            case generic: g_button.onclick(f); break;
            case image: i_button.onclick(f); break;
            case arrow: a_button.onclick(f); break;
            }
        }

        universal_button() {}
        ~universal_button() {}

        universal_button(const universal_button &o) {
            type = o.type;
            switch (type) {
            case generic: g_button = o.g_button; break;
            case image: i_button = o.i_button; break;
            case arrow: a_button = o.a_button; break;
            }
        }

        universal_button(const generic_button &b) {
            type = generic;
            g_button = b;
        }

        universal_button(const image_button &b) {
            type = image;
            i_button = b;
        }

        universal_button(const arrow_button &b) {
            type = arrow;
            a_button = b;
        }
    };

    struct state {
        std::stack<vec2i> _offset;
        std::vector<universal_button> buttons;
        std::vector<scrollbar_t*> scrollbars;

        inline const vec2i offset() { return _offset.empty() ? vec2i{0, 0} : _offset.top(); }
    };

    state g_state;
    generic_button dummy;
    element dummy_element;

    image_desc resource_icons;
}

ANK_REGISTER_CONFIG_ITERATOR(config_load_ui_options);
void config_load_ui_options() {
    g_config_arch.r_section("uioptions", [] (archive arch) {
        arch.r_desc("resource_icons", ui::resource_icons);
    });
}

static ui::element::ptr create_element(pcstr type) {
    ui::element::ptr elm;
    if (!strcmp(type, "outer_panel")) { elm = std::make_shared<ui::eouter_panel>(); }
    else if (!strcmp(type, "scrollbar")) { elm = std::make_shared<ui::escrollbar>(); }
    else if (!strcmp(type, "menu_header")) { elm = std::make_shared<ui::emenu_header>(); }
    else if (!strcmp(type, "inner_panel")) { elm = std::make_shared<ui::einner_panel>(); }
    else if (!strcmp(type, "background")) { elm = std::make_shared<ui::ebackground>(); }
    else if (!strcmp(type, "image")) { elm = std::make_shared<ui::eimg>(); }
    else if (!strcmp(type, "label")) { elm = std::make_shared<ui::elabel>(); }
    else if (!strcmp(type, "text")) { elm = std::make_shared<ui::etext>(); }
    else if (!strcmp(type, "generic_button")) { elm = std::make_shared<ui::egeneric_button>(); }
    else if (!strcmp(type, "image_button")) { elm = std::make_shared<ui::eimage_button>(); }
    else if (!strcmp(type, "resource_icon")) { elm = std::make_shared<ui::eresource_icon>(); }
    else if (!strcmp(type, "arrow_button")) { elm = std::make_shared<ui::earrow_button>(); }
    else if (!strcmp(type, "border")) { elm = std::make_shared<ui::eborder>(); }
    else if (!strcmp(type, "large_button")) {
        auto btn = std::make_shared<ui::egeneric_button>();
        btn->mode = 1;
        elm = btn;
    }

    return elm;
}

static void load_elements(archive arch, pcstr section, ui::element *parent, ui::element::items &elements) {
    arch.r_objects(section, [&elements, parent] (pcstr key, archive elem) {
        pcstr type = elem.r_string("type");
        ui::element::ptr elm = create_element(type);

        if (elm) {
            elm->id = key;
            elements.push_back(elm);
            elm->load(elem, parent, elements);
        }
    });
}

void ui::begin_widget(vec2i offset, bool relative) {
    if (relative) {
        vec2i top = g_state._offset.empty() ? vec2i{0, 0} : g_state._offset.top();
        offset += top;
    }
    g_state._offset.push(offset);
}

void ui::begin_frame() {
    assert(g_state.buttons.size() < 1000);
    //assert(g_state._offset.size() == 0);
    g_state._offset = {};
    g_state.buttons.clear();
    g_state.scrollbars.clear();
    tooltipctx.set(0, "");
}

void ui::end_frame() {

}

void ui::end_widget() {
    if (!g_state._offset.empty()) {
        g_state._offset.pop();
    }
}

bool ui::handle_mouse(const mouse *m) {
    bool handle = false;
    for (int i = g_state.buttons.size() - 1; i >= 0 && !handle; --i) {
        handle |= !!g_state.buttons[i].handle_mouse(m, g_state.offset());
    }

    for (int i = g_state.scrollbars.size() - 1; i >= 0 && !handle; --i) {
        handle |= !!scrollbar_handle_mouse(g_state.offset(), g_state.scrollbars[i], m);
    }

    return handle;
}

pcstr ui::str(int group, int id) {
    return (pcstr)lang_get_string(group, id);
}

int ui::button_hover(const mouse *m) {
    for (auto &btn : g_state.buttons) {
        if (is_button_hover(btn, g_state.offset())) {
            return (std::distance(&g_state.buttons.front(), &btn) + 1);
        }
    }

    return 0;
}

generic_button &ui::link(pcstr label, vec2i pos, vec2i size, e_font font, UiFlags flags, std::function<void(int, int)> cb) {
    const vec2i offset = g_state.offset();

    g_state.buttons.push_back(generic_button{pos.x, pos.y, size.x + 4, size.y + 4, button_none, button_none, 0, 0});
    auto &gbutton = g_state.buttons.back().g_button;
    int focused = is_button_hover(gbutton, offset);

    text_draw_centered((uint8_t *)label, offset.x + pos.x + 1, offset.y + pos.y, size.x, focused ? FONT_NORMAL_YELLOW : font, 0);

    if (!!cb) {
        gbutton.onclick(cb);
    }
    return gbutton;
}

template<typename T>
void splitStringByNewline(const std::string &str, T &result) {
    size_t start = 0;
    size_t end = str.find('\n');

    while (end != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find('\n', start);
    }

    result.push_back(str.substr(start));
}

generic_button &ui::button(pcstr label, vec2i pos, vec2i size, fonts_vec fonts, UiFlags flags, std::function<void(int, int)> cb) {
    const vec2i offset = g_state.offset();

    g_state.buttons.push_back(generic_button{pos.x, pos.y, size.x + 4, size.y + 4, button_none, button_none, 0, 0});
    generic_button &gbutton = g_state.buttons.back().g_button;
    gbutton.hovered = is_button_hover(gbutton, offset);
    gbutton.clip = graphics_clip_rectangle();
    const bool grayed = !!(flags & UiFlags_Grayed);
    const bool hasbody = !(flags & UiFlags_NoBody);
    const bool hasborder = !(flags & UiFlags_NoBorder);

    if (hasbody) {
        button_border_draw(offset + pos, size, gbutton.hovered && !grayed);
    } else if (hasborder) {
        if (gbutton.hovered && !grayed) {
            button_border_draw(offset + pos, size, true);
        }
    }

    e_font font = fonts[gbutton.hovered ? 1 : 0];
    if (font == FONT_INVALID) {
        font = fonts[0];
    }

    int symbolh = get_letter_height((uint8_t *)"H", font);
    if (label) {
        const bool alingycenter = !!(flags & UiFlags_AlignYCentered);
        const bool alingxcenter = !!(flags & UiFlags_AlignXCentered);
        const bool alignleft = !!(flags & UiFlags_AlignLeft);
        const bool rich = !!(flags & UiFlags_Rich);
        if (rich) {
            int symbolw = text_get_width((uint8_t *)"H", font);
            int lines_num = std::max<int>(1, (int)strlen(label) * symbolw / size.x);
            int centering_y_offset = (size.y - lines_num * symbolh) / 2;
            rich_text_set_fonts(font, FONT_NORMAL_YELLOW);
            rich_text_draw((uint8_t *)label, offset.x + pos.x, offset.y + pos.y + centering_y_offset, size.x, lines_num, false, true);
        } else if (alingycenter) {
            text_draw((uint8_t *)label, offset.x + pos.x + 8, offset.y + pos.y + (size.y - symbolh) / 2 + 2, font, 0);
        } else if (alignleft) {
            text_draw((uint8_t *)label, offset.x + pos.x + 8, offset.y + pos.y + 8, font, 0);
        } else if (alingxcenter) {
            text_draw_centered((uint8_t *)label, offset.x + pos.x + 1, offset.y + pos.y + 4, size.x, font, 0);
        } else {
            text_draw_centered((uint8_t *)label, offset.x + pos.x + 1, offset.y + pos.y + (size.y - symbolh) / 2 + 4, size.x, font, 0);
        }
    }

    if (grayed) {
        graphics_shade_rect(offset + pos, size, 0x80);
    }

    if (!grayed && !!cb) {
        gbutton.onclick(cb);
    }
    return gbutton;
}

generic_button &ui::button(const svector<pcstr,4> &labels, vec2i pos, vec2i size, fonts_vec fonts, UiFlags flags, std::function<void(int, int)> cb) {
    const vec2i offset = g_state.offset();

    g_state.buttons.push_back(generic_button{pos.x, pos.y, size.x + 4, size.y + 4, button_none, button_none, 0, 0});
    auto &gbutton = g_state.buttons.back().g_button;
    gbutton.hovered = is_button_hover(gbutton, offset);

    e_font font = fonts[gbutton.hovered ? 1 : 0];
    if (font == FONT_INVALID) {
        font = fonts[0];
    }

    button_border_draw(offset + pos, size, gbutton.hovered ? 1 : 0);
    int symbolh = get_letter_height((uint8_t *)"H", font);
    int labels_num = labels.size();
    int starty = offset.y + pos.y + (size.y - (symbolh + 2) * labels_num) / 2 + 4;
    if (!!(flags & UiFlags_AlignYCentered)) {
        for (const auto &str : labels) {
            text_draw((uint8_t *)str, offset.x + pos.x + 8, starty, font, 0);
            starty += symbolh + 2;
        }
    } else {
        for (const auto &str : labels) {
            text_draw_centered((uint8_t *)str, offset.x + pos.x + 1, starty, size.x, font, 0);
            starty += symbolh + 2;
        }
    }

    if (!!cb) {
        gbutton.onclick(cb);
    }
    return gbutton;
}

generic_button &ui::large_button(pcstr label, vec2i pos, vec2i size, e_font font) {
    const vec2i offset = g_state.offset();

    g_state.buttons.push_back(generic_button{pos.x, pos.y, size.x + 4, size.y + 4, button_none, button_none, 0, 0});
    auto &gbutton = g_state.buttons.back().g_button;
    int focused = is_button_hover(gbutton, offset);

    large_label_draw(offset.x + pos.x, offset.y + pos.y, size.x / 16, focused ? 1 : 0);
    int letter_height = get_letter_height((uint8_t *)"A", font);
    text_draw_centered((uint8_t *)label, offset.x + pos.x + 1, offset.y + pos.y + 2 + (size.y - letter_height) / 2, size.x, font, 0);

    return gbutton;
}

generic_button &ui::button(uint32_t id) {
    return (id < g_state.buttons.size()) ? g_state.buttons[id].g_button : dummy;
}

pcstr ui::button_tooltip(uint32_t id) {
    return (id < g_state.buttons.size()) ? g_state.buttons[id].tooltip() : "";
}

image_button &ui::img_button(image_desc desc, vec2i pos, vec2i size, const img_button_offsets offsets, UiFlags flags) {
    const vec2i state_offset = g_state.offset();
    const mouse *m = mouse_get();

    g_state.buttons.push_back(image_button{pos.x, pos.y, size.x + 4, size.y + 4, IB_NORMAL, (uint32_t)desc.pack, (uint32_t)desc.id, offsets.data[0], button_none, button_none, 0, 0, true});
    auto &ibutton = g_state.buttons.back().i_button;

    ibutton.hovered = !(flags & UiFlags_Grayed) && (is_button_hover(ibutton, state_offset) || !!(flags & UiFlags_Selected));
    ibutton.pressed = ibutton.hovered && m->left.is_down;
    ibutton.enabled = !(flags & UiFlags_Readonly);

    time_millis current_time = time_get_millis();
    if (ibutton.pressed) {
        if (current_time - ibutton.pressed_since > 100) {
            if (ibutton.button_type != IB_BUILD && ibutton.button_type != IB_OVERSEER && !mouse_get()->left.is_down)
                ibutton.pressed = false;
        }
    }

    int image_id = image_id_from_group(ibutton.image_collection, ibutton.image_group) + ibutton.image_offset;
    if (image_id > 0) {
        if (ibutton.enabled) {
            if (ibutton.pressed) {
                image_id += offsets.data[2];
            } else if (ibutton.hovered) {
                image_id += offsets.data[1];
            }
        } else {
            image_id += offsets.data[3];
        }

        painter ctx = game.painter();
        ImageDraw::img_generic(ctx, image_id, state_offset + pos);
    }

    return ibutton;
}

image_button &ui::imgok_button(vec2i pos, std::function<void(int, int)> cb) {
    auto &btn = img_button({ PACK_GENERAL, 96 }, pos, { 39, 26 });
    btn.onclick(cb);
    return btn;
}

image_button &ui::imgcancel_button(vec2i pos, std::function<void(int, int)> cb) {
    auto &btn = img_button({ PACK_GENERAL, 96 }, pos, { 39, 26 }, { 4, 1, 2, 3 });
    btn.onclick(cb);
    return btn;
}

image_button &ui::img_button(e_image_id img, vec2i pos, vec2i size, int offset) {
    image_desc desc = get_image_desc(img);
    return img_button(desc, pos, size, {desc.offset + offset, 1, 2, 3});
}

int ui::label(int group, int number, vec2i pos, e_font font, UiFlags flags, int box_width) {
    pcstr str = (pcstr)lang_get_string(group, number);
    return label(str, pos, font, flags, box_width);
}

int ui::label(pcstr label, vec2i pos, e_font font, UiFlags flags, int box_width) {
    const vec2i offset = g_state.offset();
    if (!!(flags & UiFlags_AlignCentered)) {
        text_draw_centered((uint8_t*)label, offset.x + pos.x, offset.y + pos.y, box_width, font, 0);
        return box_width;
    } else if (!!(flags & UiFlags_LabelMultiline)) {
        return text_draw_multiline((uint8_t *)label, offset.x + pos.x, offset.y + pos.y, box_width, font, 0);
    } else if (!!(flags & UiFlags_Rich)) {
        rich_text_set_fonts(font, FONT_NORMAL_YELLOW);
        return rich_text_draw((uint8_t *)label, offset.x, offset.y, box_width, 10, false);
    } else {
        return lang_text_draw(label, offset + pos, font, box_width);
    }
}

int ui::label_amount(int group, int number, int amount, vec2i pos, e_font font, pcstr postfix) {
    const vec2i offset = g_state.offset();
    return lang_text_draw_amount(group, number, amount, offset.x + pos.x, offset.y + pos.y, font, postfix);
}

int ui::label_percent(int amount, vec2i pos, e_font font) {
    const vec2i offset = g_state.offset();
    return text_draw_percentage(amount, offset.x + pos.x, offset.y + pos.y, font);
}

int ui::label_colored(textid tx, vec2i pos, e_font font, color color) {
    return lang_text_draw_colored(tx.group, tx.id, pos.x, pos.y, font, color);
}

int ui::label_colored(pcstr tx, vec2i pos, e_font font, color color) {
    return lang_text_draw_colored(tx, pos.x, pos.y, font, color);
}

void ui::eimage(e_image_id group, vec2i pos, int img) {
    painter ctx = game.painter();
    const vec2i offset = g_state.offset();
    ImageDraw::img_generic(ctx, image_group(group) + img, pos + offset);
}

void ui::eimage(image_desc imgd, vec2i pos) {
    painter ctx = game.painter();
    const vec2i offset = g_state.offset();
    ImageDraw::img_generic(ctx, image_group(imgd), pos + offset);
}

void ui::panel(vec2i pos, vec2i size, UiFlags flags) {
    const vec2i offset = g_state.offset();
    if (!!(flags & UiFlags_PanelOuter)) {
        outer_panel_draw(offset + pos, size.x, size.y);
    } else if (!!(flags & UiFlags_PanelInner)) {
        inner_panel_draw(offset + pos, size);
    }
}

void ui::icon(vec2i pos, e_resource img) {
    const vec2i offset = g_state.offset();
    painter ctx = game.painter();
    ImageDraw::img_generic(ctx, image_id_resource_icon(img), offset.x + pos.x, offset.y + pos.y);
}

void ui::icon(vec2i pos, e_advisor adv) {
    painter ctx = game.painter();
    const vec2i offset = g_state.offset();
    ImageDraw::img_generic(ctx, image_group(IMG_ADVISOR_ICONS) + (adv - 1), offset.x + pos.x, offset.y + pos.y);
}

arrow_button &ui::arw_button(vec2i pos, bool down, bool tiny, UiFlags_ flags) {
    const vec2i offset = g_state.offset();
    const mouse *m = mouse_get();

    int img_index = tiny ? (down ? 0 : 3) : down ? 17 : 15;
    int size = tiny ? 17 : 24;
    g_state.buttons.push_back(arrow_button{pos.x, pos.y, img_index, size, button_none, 0, 0});
    auto &abutton = g_state.buttons.back().a_button;

    const bool hovered = !(flags & UiFlags_Grayed) && (is_button_hover(abutton, offset) || !!(flags & UiFlags_Selected));
    const bool pressed = hovered && m->left.is_down;
    abutton.state = (hovered ? (pressed ? 2 : 1) : 0);

    arrow_buttons_draw(offset, abutton, tiny);
    const bool grayed = !!(flags & UiFlags_Grayed);

    if (grayed) {
        graphics_shade_rect(offset + pos, vec2i{ size, size }, 0x80);
    }

    return abutton;
}

scrollbar_t &ui::scrollbar(scrollbar_t &scr, vec2i pos, int &value, vec2i size) {
    const vec2i offset = g_state.offset();

    g_state.scrollbars.push_back(&scr);
    scrollbar_draw(offset, &scr);

    return scr;
}

void ui::element::load(archive arch, element *parent, element::items &items) {
    vec2i parent_offset = parent ? parent->pos : vec2i{0, 0};
    pos = arch.r_vec2i("pos") + parent_offset;
    size = arch.r_size2i("size");
    enabled = arch.r_bool("enabled", true);
    arch.r_section("margin", [this] (archive m) {
        margin.bottom = m.r_int("bottom", recti::nomargin);
        margin.left = m.r_int("left", recti::nomargin);
        margin.right = m.r_int("right", recti::nomargin);
        margin.top = m.r_int("top", recti::nomargin);
    });

    load_elements(arch, "ui", this, items);
}

pcstr ui::element::text_from_key(pcstr key) {
    return lang_text_from_key(key);
}

void ui::element::update_pos(const recti &r) {
    if (margin.left > recti::nomargin) { pos.x = r.left + margin.left; }
    if (margin.bottom > recti::nomargin) { pos.y = r.bottom + margin.bottom; }
    if (margin.right > recti::nomargin) { pos.x = r.right + margin.right; }
    if (margin.top > recti::nomargin) { pos.y = r.top + margin.top; }
}

vec2i ui::element::screen_pos() const {
    const vec2i offset = g_state.offset();
    return offset + pos;
}

void ui::eouter_panel::draw() {
    ui::panel(pos, size, UiFlags_PanelOuter);
}

void ui::eouter_panel::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    pcstr type = arch.r_string("type");
    assert(!strcmp(type, "outer_panel"));
}

void ui::einner_panel::draw() {
    ui::panel(pos, size, UiFlags_PanelInner);
}

void ui::einner_panel::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    pcstr type = arch.r_string("type");
    assert(!strcmp(type, "inner_panel"));
}

void ui::widget::draw() {
    vec2i bsize = ui["background"].pxsize();
    recti rect{ 0, 0, bsize.x, bsize.y };
    for (auto &e : elements) {
        e->update_pos(rect);
        if (e->enabled) {
            e->draw();
        }
    }
}

void ui::widget::load(archive arch, pcstr section) {
    elements.clear();
    pos = arch.r_vec2i("pos");
    
    load_elements(arch, section, nullptr, elements);
}

void ui::widget::load(pcstr section) {
    g_config_arch.r_section(section, [this] (archive arch) {
        this->load(arch);
    });
}

ui::element& ui::widget::operator[](pcstr id) {
    auto it = std::find_if(elements.begin(), elements.end(), [id] (const auto &e) { return e->id == id; });
    if (it == elements.end()) {
        logs::error("No element with id:%s", id);
    }
    return (it != elements.end() ? **it : ui::dummy_element);
}

void ui::widget::set_clip_rectangle(vec2i pos, vec2i size) {
    const vec2i offset = g_state.offset();
    graphics_set_clip_rectangle(pos + offset, size);
}

void ui::widget::set_clip_rectangle(const element &e) {
    const vec2i offset = g_state.offset();
    graphics_set_clip_rectangle(e.pos + offset, e.pxsize());
}

void ui::widget::reset_clip_rectangle() {
    graphics_reset_clip_rectangle();
}

void ui::widget::line(bool hline, vec2i npos, int size) {
    const vec2i offset = g_state.offset();
    if (hline) {
        graphics_draw_horizontal_line(npos + offset, size, COLOR_BLACK);
    } else {
        graphics_draw_vertical_line(npos + offset, size, COLOR_BLACK);
    }
}

void ui::eimg::draw() {
    if (img > 0) {
        ui::eimage(img, pos);
    } else if (isometric) {
        painter ctx = game.painter();
        const vec2i offset = g_state.offset();
        ImageDraw::isometric_from_drawtile(ctx, image_group(img_desc), offset + pos);
        ImageDraw::isometric_from_drawtile_top(ctx, image_group(img_desc), offset + pos);
    } else {
        ui::eimage(img_desc, pos);
    }
}

void ui::eimg::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    pcstr type = arch.r_string("type");
    assert(!strcmp(type, "image"));
    img = arch.r_image("image");
    img_desc.pack = arch.r_int("pack");
    img_desc.id = arch.r_int("id");
    img_desc.offset = arch.r_int("offset");
    isometric = arch.r_bool("isometric");
}

void ui::eimg::image(image_desc image) {
    img_desc = image;
}

void ui::eimg::image(int image) {
    img_desc.offset = image;
}

void ui::ebackground::draw() {
    painter ctx = game.painter();
    ImageDraw::img_background(ctx, image_group(img_desc), 1.f, pos);
}

void ui::ebackground::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    pcstr type = arch.r_string("type");
    assert(!strcmp(type, "background"));
    scale = arch.r_float("scale", 1.f);
    img_desc.pack = arch.r_int("pack");
    img_desc.id = arch.r_int("id");
    img_desc.offset = arch.r_int("offset");
}

void ui::eborder::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    border = arch.r_int("border");
}

void ui::eborder::draw() {
    const vec2i offset = g_state.offset();
    switch (border) {
    default: //fallthrought
    case 0:
        button_border_draw(offset + pos, size, false);
        break;
    }
}

void ui::eresource_icon::draw() {
    ui::eimage(resource_icons + res, pos);
}

int image_id_resource_icon(int resource) {
    return image_group(ui::resource_icons) + resource;
}

void ui::eresource_icon::image(int image) {
    res = (e_resource)image;
}

void ui::eresource_icon::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    pcstr type = arch.r_string("type");
    assert(!strcmp(type, "resource_icon"));
    res = arch.r_type<e_resource>("resource");
}

void ui::elabel::draw() {
    const vec2i offset = g_state.offset();
    if (_body.x > 0) {
        label_draw(pos.x + offset.x, pos.y + offset.y, _body.x, _body.y);
    }
    ui::label(_text.c_str(), pos + ((_body.x > 0) ? vec2i{8, 4} : vec2i{0, 0}), _font, _flags, size.x);
}

void ui::elabel::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    _text = arch.r_string("text");
    if (_text[0] == '#') {
        _text = lang_text_from_key(_text.c_str());
    }
    _font = arch.r_type<e_font>("font", FONT_NORMAL_BLACK_ON_LIGHT);
    _font_link = arch.r_type<e_font>("font_link", FONT_NORMAL_YELLOW);
    _font_hover = arch.r_type< e_font>("font_hover", FONT_INVALID);
    _body = arch.r_size2i("body");
    _color = arch.r_uint("color");
    _wrap = arch.r_int("wrap");
    _clip_area = arch.r_bool("clip_area");
    _shadow_color = arch.r_uint("shadow");
    _tooltip = arch.r_string("tooltip");
    pcstr talign = arch.r_string("align");
    bool multiline = arch.r_bool("multiline");
    bool rich = arch.r_bool("rich");
    bool scroll = arch.r_bool("scroll", true);
    bool aligncenter = strcmp("center", talign) == 0;
    bool alignleft = strcmp("left", talign) == 0;
    bool alignycenter = strcmp("ycenter", talign) == 0;
    bool alignxcenter = strcmp("xcenter", talign) == 0;
    _flags = (aligncenter ? UiFlags_AlignCentered : UiFlags_None)
               | (alignycenter ? UiFlags_AlignYCentered : UiFlags_None)
               | (alignxcenter ? UiFlags_AlignXCentered : UiFlags_None)
               | (alignleft ? UiFlags_AlignLeft : UiFlags_None)
               | (multiline ? UiFlags_LabelMultiline : UiFlags_None)
               | (rich ? UiFlags_Rich : UiFlags_None)
               | (scroll ? UiFlags_None : UiFlags_NoScroll);
}

void ui::elabel::text(pcstr v) {
    _text = lang_text_from_key(v);
}

void ui::elabel::text_color(color v) {
    _color = v;
}

void ui::elabel::font(int v) {
    _font = (e_font)v;
}

void ui::elabel::width(int v) {
    size.x = v;
    _wrap = v;
}

void ui::eimage_button::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    pcstr type = arch.r_string("type");
    assert(!strcmp(type, "image_button"));

    img = arch.r_image("image");
    scale = arch.r_float("scale", 1.f);
    param1 = arch.r_int("param1");
    param2 = arch.r_int("param2");
    img_desc.pack = arch.r_int("pack");
    img_desc.id = arch.r_int("id");
    img_desc.offset = arch.r_int("offset");
    border = arch.r_bool("border");
    offsets.data[0] = img_desc.offset;
    offsets.data[1] = arch.r_int("offset_focused", 1);
    offsets.data[2] = arch.r_int("offset_pressed", 2);
    offsets.data[3] = arch.r_int("offset_disabled", 3);
    _tooltip = arch.r_vec2i("tooltip");

    pcstr name_icon_texture = arch.r_string("icon_texture");
    if (name_icon_texture && *name_icon_texture) {
        vec2i tmp_size;
        icon_texture = load_icon_texture(name_icon_texture, tmp_size);
    }
}

void ui::eimage_button::draw() {
    const vec2i doffset = g_state.offset();
    UiFlags flags = (selected ? UiFlags_Selected : UiFlags_None);
    flags |= (readonly ? UiFlags_Readonly : UiFlags_None);

    image_button *btn = nullptr;
    pcstr pid = id.c_str();

    vec2i tsize;
    if (img) {
        btn = &ui::img_button(img, pos, size, img_desc.offset);
    } else if (img_desc.id || img_desc.offset) {
        int img_id = image_id_from_group(img_desc.pack, img_desc.id);
        const image_t *img_ptr = image_get(img_id + img_desc.offset);

        tsize.x = size.x > 0 ? size.x : img_ptr->width;
        tsize.y = size.y > 0 ? size.y : img_ptr->height;

        btn = &ui::img_button(img_desc, pos, tsize, offsets, flags);
    } else if (texture_id > 0) {
        graphics_draw_from_texture(texture_id, doffset + pos, size);
        tsize = size;
        btn = &ui::img_button({ 0, 0 }, pos, size, offsets, flags);
    } else if (icon_texture) {
        painter ctx = game.painter();
        ctx.draw((SDL_Texture*)icon_texture, pos, {0, 0}, size, 0xffffffff, scale, false, true);

        btn = &ui::img_button({ 0, 0 }, pos, size, offsets, UiFlags_None);
    } 

    if (!btn) {
        return;
    }

    if (grayed) {
        graphics_shade_rect(doffset + pos, tsize, 0x80);
        return;
    }

    if (readonly) {
        return;
    }

    if (border && selected) {
        button_border_draw(doffset + pos - vec2i{ 4, 4 }, tsize + vec2i{ 8, 8 }, true);
    }

    btn->onclick(_func);
    btn->tooltip(_tooltip);

    if (_tooltip.id && btn->hovered) {
        tooltipctx.set(0, _tooltip);
    }
}

void ui::etext::load(archive arch, element* parent, items &elems) {
    elabel::load(arch, parent, elems);

    pcstr type = arch.r_string("type");
    assert(!strcmp(type, "text"));
}

void ui::escrollbar::draw() {
    ui::scrollbar(this->scrollbar, pos, this->scrollbar.scroll_position);
}

void ui::escrollbar::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    pcstr type = arch.r_string("type");
    assert(!strcmp(type, "scrollbar"));

    scrollbar.pos = pos;
    scrollbar.height = size.y;
}

void ui::etext::draw() {
    const vec2i offset = g_state.offset();
    if (!!(_flags & UiFlags_AlignCentered)) {
        if (_shadow_color) {
            text_draw_centered((uint8_t *)_text.c_str(), offset.x + pos.x + 1, offset.y + pos.y, size.x, _font, _shadow_color);
        }
        text_draw_centered((uint8_t *)_text.c_str(), offset.x + pos.x, offset.y + pos.y, size.x, _font, _color);
    } else if (!!(_flags & UiFlags_LabelMultiline)) {
        text_draw_multiline((uint8_t *)_text.c_str(), offset.x + pos.x, offset.y + pos.y, _wrap, _font, _color);
    } else if (!!(_flags & UiFlags_AlignYCentered)) {
        int symbolh = get_letter_height((uint8_t *)"H", _font);
        if (_shadow_color) {
            text_draw((uint8_t *)_text.c_str(), offset.x + pos.x + 1, offset.y + pos.y + (size.y - symbolh) / 2, _font, _shadow_color);
        }
        text_draw((uint8_t *)_text.c_str(), offset.x + pos.x, offset.y + pos.y + (size.y - symbolh) / 2, _font, _color);
    } else if (!!(_flags & UiFlags_Rich)) {
        if (_clip_area) {
            graphics_set_clip_rectangle(offset + pos, pxsize());
        }

        int symbolh = get_letter_height((uint8_t *)"H", _font);
        int maxlines = pxsize().y > 0 
                        ? std::max(1, pxsize().y / symbolh)
                        : 0xff;

        int rwrap = _wrap <= 0 ? size.x : _wrap;
        rwrap = rwrap <= 0 ? 9999 : rwrap;

        rich_text_set_fonts(_font, _font_link);
        rich_text_draw((const uint8_t *)_text.c_str(), offset.x + pos.x, offset.y + pos.y, rwrap, maxlines, false);

        if (!(_flags & UiFlags_NoScroll)) {
            rich_text_draw_scrollbar();
        }
        
        if (_clip_area) {
            graphics_reset_clip_rectangle();
        }
    } else {
        if (_shadow_color) {
            text_draw((uint8_t *)_text.c_str(), offset.x + pos.x + 1, offset.y + pos.y, _font, _shadow_color);
        }
        text_draw((uint8_t *)_text.c_str(), offset.x + pos.x, offset.y + pos.y, _font, _color);
    }
}

void ui::emenu_header::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    pcstr type = arch.r_string("type");
    assert(!strcmp(type, "menu_header"));

    _font = (e_font)arch.r_int("font", FONT_NORMAL_BLACK_ON_LIGHT);
    _tooltip = arch.r_string("tooltip");

    impl.text = arch.r_string("text");
    if (!!impl.text && impl.text[0u] == '#') {
        impl.text = lang_text_from_key(impl.text.c_str());
    }
}

void ui::emenu_header::load_items(archive arch, pcstr section) {
    impl.items.clear();

    arch.r_objects(section, [this] (pcstr key, archive elem) {
        pcstr type = elem.r_string("type");
        assert(!strcmp(type, "menu_item"));

        menu_item item;
        item.id = key;
        item.text = elem.r_string("text");
        if (!!item.text && item.text[0u] == '#') {
            item.text = lang_text_from_key(item.text.c_str());
        }
        item.parameter = elem.r_int("parameter");
        item.hidden = elem.r_bool("hidden");
        impl.items.push_back(item);
    });
}

void ui::emenu_header::draw() {
    lang_text_draw(impl.text, pos, _font);
}

int ui::emenu_header::text_width() {
    return lang_text_get_width(impl.text, _font);
}

menu_item &ui::emenu_header::item(pcstr key) {
    static menu_item dummy;
    auto it = std::find_if(impl.items.begin(), impl.items.end(), [key] (auto &it) { return it.id == key; });
    return it != impl.items.end() ? *it : dummy;
}

void ui::earrow_button::load(archive arch, element *parent, items &elems) {
    element::load(arch, parent, elems);

    tiny = arch.r_bool("tiny");
    down = arch.r_bool("down");
}

void ui::earrow_button::draw() {
    ui::arw_button(pos, down, tiny)
        .onclick(_func);
}

void ui::egeneric_button::draw() {
    UiFlags flags = _flags 
                      | (grayed ? UiFlags_Grayed : UiFlags_None)
                      | (!_border ? UiFlags_NoBorder : UiFlags_None)
                      | (!_hbody ? UiFlags_NoBody : UiFlags_None);

    generic_button *btn = nullptr;
    switch (mode) {
    case 0:
        btn = &ui::button(_text.c_str(), pos, size, { _font, _font_hover }, flags);
        break;

    case 1:
        btn = &ui::large_button(_text.c_str(), pos, size, _font);
        break;
    }

    if (_func) btn->onclick(_func);
    if (_rfunc) btn->onrclick(_rfunc);

    btn->tooltip(_tooltip);

    if (_tooltip.id && btn->hovered) {
        tooltipctx.set(0, _tooltip);
    }
}

void ui::egeneric_button::load(archive arch, element *parent, items &elems) {
    elabel::load(arch, parent, elems);

    pcstr mode_str = arch.r_string("mode");
    if (mode_str && !strcmp(mode_str, "large")) {
        mode = 1;
    }
    _tooltip = arch.r_vec2i("tooltip");
    _border = arch.r_bool("border", true);
    _hbody = arch.r_bool("hbody", true);
}

void ui::info_window::load(archive arch, pcstr section) {
    widget::load(arch, section);
    resource_text_group = arch.r_int("resource_text_group");
}

void ui::info_window::load() {
    widget::load(section);
}