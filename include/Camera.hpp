//
// Created by sjbar on 11/01/2023.
//

#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"

namespace PathRehnda {
    class Camera {
    public:
        explicit Camera(double aspect_ratio);

        [[nodiscard]]
        Ray get_ray(double u, double v) const;

        [[nodiscard]]
        double get_aspect_ratio() const;

    private:
        double aspect_ratio;
        Point3 origin;
        Point3 lower_left_corner;
        Vec3 horizontal;
        Vec3 vertical;
    };
}
