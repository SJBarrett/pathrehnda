//
// Created by sjbar on 19/01/2023.
//

#pragma once

#include "Vec3.hpp"

namespace PathRehnda {
    struct Uv {
        double u;
        double v;
    };
    class Texture {
    public:
        [[nodiscard]]
        virtual ColorRgb sample(Uv uv, const Point3& point) const = 0;
    };
}
