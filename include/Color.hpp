//
// Created by sjbar on 9/01/2023.
//

#pragma once

#include "RehndaMath.hpp"
#include "Vec3.hpp"

#include <iostream>

namespace PathRehnda {
    struct Color24BitRgb {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };

    Color24BitRgb unit_interval_to_24_bit_rgb(const ColorRgb& color_rgb) {
        return {
                static_cast<uint8_t>(256 * clamp(color_rgb.x(), 0.0, 0.999)),
                static_cast<uint8_t>(256 * clamp(color_rgb.y(), 0.0, 0.999)),
                static_cast<uint8_t>(256 * clamp(color_rgb.z(), 0.0, 0.999))
        };
    }
}