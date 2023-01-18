//
// Created by sjbar on 10/01/2023.
//
#include "hittable/HittableList.hpp"

namespace PathRehnda {

    HittableList::HittableList(const std::shared_ptr<Hittable>& initial_hittable) {
        add(initial_hittable);
    }

    void HittableList::clear() {
        objects.clear();
    }

    void HittableList::add(const std::shared_ptr<Hittable>& object) {
        objects.push_back(object);
    }

    std::optional<HitResult> HittableList::hit(const Ray &ray, double t_min, double t_max) const {
        HitResult closest_hit;
        double closest_t = t_max;
        bool did_hit = false;
        for (const auto& object : objects) {
            if (auto next_hit = object->hit(ray, t_min, closest_t)) {
                closest_hit = next_hit.value();
                closest_t = closest_hit.t;
                did_hit = true;
            }
        }
        if (did_hit) {
            return closest_hit;
        } else {
            return {};
        }
    }

    std::optional<AABB> HittableList::bounding_box(double time_0, double time_1) const {
        if (objects.empty()) return {};

        AABB surrounding_box;
        bool first_box = true;
        for (const auto &object : objects) {
            if (auto aabb = object->bounding_box(time_0, time_0)) {
                surrounding_box = first_box ? aabb.value() : AABB::surrounding_box(aabb.value(), surrounding_box);
            } else {
                throw std::runtime_error("No nested hittable lists supported yet");
            }
            first_box = false;
        }
        return surrounding_box;
    }
}