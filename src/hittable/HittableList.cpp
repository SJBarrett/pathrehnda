//
// Created by sjbar on 10/01/2023.
//
#include <tracy/Tracy.hpp>
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

    HitResult HittableList::hit(const Ray &ray, double t_min, double t_max) const {
        ZoneScopedN("HittableList::Hit");
        HitResult closest_hit {.did_hit = false, .t = t_max};

        for (const auto& object : objects) {
            if (auto next_hit = object->hit(ray, t_min, closest_hit.t)) {
                closest_hit = next_hit;
            }
        }
        return closest_hit;
    }
}