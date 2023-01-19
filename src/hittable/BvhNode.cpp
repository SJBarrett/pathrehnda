//
// Created by sjbar on 17/01/2023.
//

#include <algorithm>
#include <spdlog/spdlog.h>
#include "hittable/BvhNode.hpp"

namespace PathRehnda {
    BvhNode::BvhNode(const HittableList &list, double time_0, double time_1, size_t depth) : BvhNode(list.objects, 0, list.objects.size(), time_0, time_1, depth) {

    }

    BvhNode::BvhNode(const std::vector<std::shared_ptr<Hittable>> &src_objects, size_t start, size_t end, double time_0, double time_1, size_t depth) : depth(depth) {
        auto objects = src_objects;

        int axis = random_int(0, 2);
        auto comparator = (axis == 0) ? box_x_compare
                        : (axis == 1) ? box_y_compare
                        : box_z_compare;

        size_t object_span = end - start;
        if (object_span == 1) {
            left = right = objects[start];
        } else if (object_span == 2) {
            if (comparator(objects[start], objects[start+1])) {
                left = objects[start];
                right = objects[start+1];
            } else {
                left = objects[start+1];
                right = objects[start];
            }
        } else {
            std::sort(objects.begin() + start, objects.begin() + end, comparator);
            auto mid = start + object_span / 2;
            left = std::make_shared<BvhNode>(objects, start, mid, time_0, time_1, depth + 1);
            right = std::make_shared<BvhNode>(objects, mid, end, time_0, time_1, depth + 1);
        }

        if (auto left_box = left->bounding_box(time_0, time_1)) {
            if (auto right_box = right->bounding_box(time_0, time_1)) {
                box = AABB::surrounding_box(left_box.value(), right_box.value());
                return;
            }
        }
        throw std::runtime_error("Failed to build BvhNode as a child node had no bounding box");
    }

    std::optional<HitResult> BvhNode::hit(const Ray &ray, double t_min, double t_max) const {
        if (!box.does_hit(ray, t_min, t_max)) {
            return {};
        }

        const auto hit_left = left->hit(ray, t_min, t_max);
        const auto hit_right = right->hit(ray, t_min, hit_left ? hit_left->t : t_max);

        // if right hit, that means it was closer as the max search distance was how far left is
        if (hit_right) {
            return hit_right.value();
        } else if (hit_left) {
            return hit_left.value();
        } else {
            return {};
        }
    }

    std::optional<AABB> BvhNode::bounding_box(double time_0, double time_1) const {
        return box;
    }


    bool box_x_compare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b) {
        return box_compare(a, b, 0);
    }

    bool box_y_compare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b) {
        return box_compare(a, b, 1);
    }

    bool box_z_compare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b) {
        return box_compare(a, b, 2);
    }

    bool box_compare(const std::shared_ptr<Hittable> &a, const std::shared_ptr<Hittable> &b, int axis) {
        if (auto box_a = a->bounding_box(0, 0)) {
            if (auto box_b = b->bounding_box(0, 0)) {
                return box_a->minimum.e[axis] < box_b->minimum.e[axis];
            }
        }

        throw std::runtime_error("Can't compare boxes that don't produce bounding boxes");
    }
} // PathRehnda