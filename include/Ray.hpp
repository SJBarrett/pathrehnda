//
// Created by sjbar on 9/01/2023.
//

#pragma once

#include "Vec3.hpp"

namespace PathRehnda {

    /**
     * A ray represents the mathematical function
     *     P(t) = A + t * b
     */
    class Ray {
    public:
        Ray() {}

        Ray(const Point3 &origin, const Vec3 &direction, double time = 0.0) : origin(origin), direction(direction), time(time) {};

        [[nodiscard]]
        Point3 at(double t) const {
            return origin + t * direction;
        }

    public:
        Point3 origin;
        Vec3 direction;
        double time;
    };
}