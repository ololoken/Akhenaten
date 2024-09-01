#pragma once

#include "window/advisors.h"
#include "graphics/elements/ui.h"

namespace ui {
struct advisor_health_window : public advisor_window_t<advisor_health_window> {
    virtual int handle_mouse(const mouse *m) override { return 0; }
    virtual int get_tooltip_text() override { return 0; }
    virtual void ui_draw_foreground() override;
    virtual void draw_foreground() override {}
    virtual int draw_background() override;
    virtual int ui_handle_mouse(const mouse *m) override;
    virtual void init() override {}

    static advisor_window *instance();
};
}
