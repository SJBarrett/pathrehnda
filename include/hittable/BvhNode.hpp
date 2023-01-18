//
// Created by sjbar on 17/01/2023.
//

#pragma once

#include "HittableList.hpp"

namespace PathRehnda {

    class BvhNode : public Hittable {
    public:
        BvhNode() = default;

        BvhNode(const HittableList &list, double time_0, double time_1, size_t depth);

        BvhNode(const std::vector<std::shared_ptr<Hittable>> &src_objects, size_t start, size_t end, double time_0, double time_1, size_t depth);

        [[nodiscard]]
        std::optional<HitResult> hit(const Ray &ray, double t_min, double t_max) const override;

        [[nodiscard]]
        std::optional<AABB> bounding_box(double time_0, double time_1) const override;

    public:
        std::shared_ptr<Hittable> left;
        std::shared_ptr<Hittable> right;
        AABB box;
        size_t depth;
    };


    bool box_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b, int axis);

    bool box_x_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b);

    bool box_y_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b);

    bool box_z_compare(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b);
} // PathRehnda