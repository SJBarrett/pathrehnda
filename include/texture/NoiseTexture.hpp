//
// Created by sjbar on 21/01/2023.
//

#pragma once

#include "Texture.hpp"
#include "Perlin.hpp"

namespace PathRehnda {

    class NoiseTexture : public Texture {
    public:
        NoiseTexture() = default;
        explicit NoiseTexture(double scale);

        [[nodiscard]]
        ColorRgb sample(Uv uv, const Point3 &point) const override;

    public:
        Perlin perlin;
        double scale;
    };

} // PathRehnda