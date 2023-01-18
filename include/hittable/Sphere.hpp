//
// Created by sjbar on 10/01/2023.
//

#pragma once

#include <memory>
#include <utility>
#include "Vec3.hpp"
#include "Hittable.hpp"

namespace PathRehnda {

    class Sphere : public Hittable {
    public:
        Sphere() {}
        Sphere(Point3 centre, double radius, std::shared_ptr<Material> mat) : centre(centre), radius(radius), material(std::move(mat)) {};

        [[nodiscard]]
        std::optional<HitResult> hit(const Ray &ray, double t_min, double t_max) const override;

        [[nodiscard]]
        std::optional<AABB> bounding_box(double time_0, double time_1) const override;

    public:
        Point3 centre;
        double radius;
        std::shared_ptr<Material> material;
    };

} // PathRehnda