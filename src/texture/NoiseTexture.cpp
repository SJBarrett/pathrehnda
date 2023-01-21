//
// Created by sjbar on 21/01/2023.
//

#include "texture/NoiseTexture.hpp"

namespace PathRehnda {
    NoiseTexture::NoiseTexture(double scale) : scale(scale) {
    }

    ColorRgb NoiseTexture::sample(Uv uv, const Point3 &point) const {
        return ColorRgb(1,1,1) * 0.5 * (1 + sin(scale * point.z() + 10 * perlin.turbulence(point)));
    }
} // PathRehnda