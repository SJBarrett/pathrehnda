//
// Created by sjbar on 9/01/2023.
//

#pragma once

#include "RehndaMath.hpp"
#include "Vec3.hpp"

#include <iostream>

namespace PathRehnda {
    double perform_gamma_correction(double color_value) {
        return sqrt(color_value);
    }

    void write_color(std::ostream &out, ColorRgb pixel_color, int samples_per_pixel) {
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();

        auto scale = 1.0 / samples_per_pixel;
        // perform
        r = perform_gamma_correction(scale * r);
        g = perform_gamma_correction(scale * g);
        b = perform_gamma_correction(scale * b);

        // Write the translated [0,255] value of each color component.
        out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
            << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
    }
}