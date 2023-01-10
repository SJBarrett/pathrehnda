//
// Created by sjbar on 10/01/2023.
//

#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"

namespace PathRehnda {

    /*
     * OPTIMISATION could improve performance by finding the closest hit first, and only then calculate the normal
     */
    struct HitResult {
        Point3 hit_location = Point3::zero();
        Vec3 normal = Vec3::zero();
        double t = -1; // From P(t) = A + t * b formula
        bool did_hit;

        explicit operator bool() const {
            return did_hit;
        }
    };

    class Hittable {
    public:
        [[nodiscard]]
        virtual HitResult hit(const Ray &ray, double t_min, double t_max) const = 0;
    };
}