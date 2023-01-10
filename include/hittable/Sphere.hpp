//
// Created by sjbar on 10/01/2023.
//

#pragma once

#include "Vec3.hpp"
#include "Hittable.hpp"

namespace PathRehnda {

    class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point3 centre, double radius) : centre(centre), radius(radius) {};

        [[nodiscard]]
        HitResult hit(const Ray &ray, double t_min, double t_max) const override;

    public:
        Point3 centre;
        double radius;
    };

} // PathRehnda