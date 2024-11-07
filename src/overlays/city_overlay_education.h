#pragma once

#include "city_overlay.h"

struct city_overlay_education : public city_overlay {
    city_overlay_education();

    virtual int get_column_height(const building *b) const override;
    virtual xstring get_tooltip_for_building(tooltip_context *c, const building *b) const override;
};

struct city_overlay_academy : public city_overlay {
    city_overlay_academy();

    virtual int get_column_height(const building *b) const override;
    virtual xstring get_tooltip_for_building(tooltip_context *c, const building *b) const override;
};

struct city_overlay_libraries : public city_overlay {
    city_overlay_libraries();

    virtual int get_column_height(const building *b) const override;
    virtual xstring get_tooltip_for_building(tooltip_context *c, const building *b) const override;
};

city_overlay* city_overlay_for_education();
city_overlay* city_overlay_for_scribal_school();
city_overlay* city_overlay_for_library();
city_overlay* city_overlay_for_academy();
