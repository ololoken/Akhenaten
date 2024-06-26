#include "generic_button.h"

#include "input/mouse.h"

static int get_button(const mouse* m, vec2i pos, const generic_button* buttons, int num_buttons) {
    for (int i = 0; i < num_buttons; i++) {
        if (pos.x + buttons[i].x <= m->x && pos.x + buttons[i].x + buttons[i].width > m->x && pos.y + buttons[i].y <= m->y
            && pos.y + buttons[i].y + buttons[i].height > m->y) {
            return i + 1;
        }
    }
    return 0;
}

static int get_button_min(const mouse* m, vec2i pos, const generic_button* buttons, int num_buttons, int minimum_button) {
    for (int i = minimum_button; i < num_buttons; i++) {
        if (pos.x + buttons[i].x <= m->x && pos.x + buttons[i].x + buttons[i].width > m->x && pos.y + buttons[i].y <= m->y
            && pos.y + buttons[i].y + buttons[i].height > m->y) {
            return i + 1;
        }
    }
    return 0;
}

int generic_buttons_handle_mouse(const mouse* m, vec2i pos, const generic_button* buttons, int num_buttons, int* focus_button_id) {
    int button_id = get_button(m, pos, buttons, num_buttons);
    if (focus_button_id) {
        *focus_button_id = button_id;
    }

    if (!button_id) {
        return 0;
    }

    const generic_button* button = &buttons[button_id - 1];
    if (m->left.went_up) {
        button->left_click_handler(button->parameter1, button->parameter2);
        if (button->_onclick) {
            button->_onclick(button->parameter1, button->parameter2);
        }
    } else if (m->right.went_up) {
        button->right_click_handler(button->parameter1, button->parameter2);
    } else {
        return 0;
    }

    return button_id;
}

int generic_buttons_min_handle_mouse(const mouse* m, vec2i pos, const generic_button* buttons, int num_buttons, int* focus_button_id, int minimum_button) {
    int button_id = get_button_min(m, pos, buttons, num_buttons, minimum_button);
    if (focus_button_id)
        *focus_button_id = button_id;

    if (!button_id)
        return 0;

    const generic_button* button = &buttons[button_id - 1];
    if (m->left.went_up)
        button->left_click_handler(button->parameter1, button->parameter2);

    else if (m->right.went_up)
        button->right_click_handler(button->parameter1, button->parameter2);
    else
        return 0;

    return button_id;
}