//
// Created by sjbar on 9/01/2023.
//

#pragma once

#include "Vec3.hpp"

#include <iostream>

void writeColor(std::ostream &out, Color pixel_color) {
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}