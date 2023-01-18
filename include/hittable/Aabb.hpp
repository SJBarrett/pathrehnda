//
// Created by sjbar on 17/01/2023.
//

#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"

namespace PathRehnda {

    class AABB {
    public:
        AABB() = default;
        AABB(const Point3& a, const Point3& b);

        bool does_hit(const Ray& ray, double t_min, double t_max) const;
    public:
        static AABB surrounding_box(AABB box_0, AABB box_1);
    public:
        Point3 minimum;
        Point3 maximum;

    };

} // PathRehnda