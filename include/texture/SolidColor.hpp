//
// Created by sjbar on 19/01/2023.
//

#pragma once

#include "Texture.hpp"

namespace PathRehnda {
    class SolidColor : public Texture {
    public:
        SolidColor() = default;
        explicit SolidColor(ColorRgb color);

        [[nodiscard]]
        ColorRgb sample(Uv uv, const Point3 &point) const override;

    private:
        ColorRgb colour_value;
    };
}
