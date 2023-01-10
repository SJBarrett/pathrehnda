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
        bool did_hit = false;
        Point3 hit_location;
        Vec3 normal;
        double t; // From P(t) = A + t * b formula
        bool front_face;

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