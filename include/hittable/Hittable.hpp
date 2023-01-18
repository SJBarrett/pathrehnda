//
// Created by sjbar on 10/01/2023.
//

#pragma once

#include <memory>
#include <optional>
#include "Vec3.hpp"
#include "Ray.hpp"
#include "Aabb.hpp"

namespace PathRehnda {
    class Material;
    /*
     * OPTIMISATION could improve performance by finding the closest hit first, and only then calculate the normal
     */
    struct HitResult {
        Point3 hit_location;
        Vec3 normal;

        std::shared_ptr<Material> material;

        double t; // From P(t) = A + t * b formula
        bool front_face;
    };

    class Hittable {
    public:
        [[nodiscard]]
        virtual std::optional<HitResult> hit(const Ray &ray, double t_min, double t_max) const = 0;

        [[nodiscard]]
        virtual std::optional<AABB> bounding_box(double time_0, double time_1) const = 0;
    };
}