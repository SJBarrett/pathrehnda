//
// Created by sjbar on 16/01/2023.
//

#pragma once

#include <memory>
#include "Hittable.hpp"

namespace PathRehnda {

    class MovingSphere : public Hittable {
    public:
        MovingSphere() = default;
        MovingSphere(Point3 start_point, Point3 end_point, double start_time, double end_time, double radius, std::shared_ptr<Material> material);

        [[nodiscard]]
        std::optional<HitResult> hit(const Ray &ray, double t_min, double t_max) const override;

        [[nodiscard]]
        std::optional<AABB> bounding_box(double time_0, double time_1) const override;

        [[nodiscard]]
        Point3 centre(double time) const;
    public:
        Point3 start_point, end_point;
        double start_time, end_time;
        double radius;
        std::shared_ptr<Material> material;
    };

} // PathRehnda