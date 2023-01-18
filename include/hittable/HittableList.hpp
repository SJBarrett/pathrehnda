//
// Created by sjbar on 10/01/2023.
//

#pragma once

#include <memory>
#include <vector>


#include "Hittable.hpp"

namespace PathRehnda {
    class HittableList : public Hittable {
    public:
        HittableList() = default;
        explicit HittableList(const std::shared_ptr<Hittable>& initial_hittable);

        void clear();
        void add(const std::shared_ptr<Hittable>& object);

        [[nodiscard]]
        std::optional<HitResult> hit(const Ray &ray, double t_min, double t_max) const override;

        [[nodiscard]]
        std::optional<AABB> bounding_box(double time_0, double time_1) const override;

    public:
        std::vector<std::shared_ptr<Hittable>> objects;
    };
}
