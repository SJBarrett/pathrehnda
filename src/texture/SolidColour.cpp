//
// Created by sjbar on 19/01/2023.
//

#include "texture/SolidColor.hpp"

namespace PathRehnda {
    SolidColor::SolidColor(ColorRgb color) : colour_value(color) {
    }

    ColorRgb SolidColor::sample(Uv uv, const Point3 &point) const {
        return colour_value;
    }
}