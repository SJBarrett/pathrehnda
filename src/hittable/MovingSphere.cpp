//
// Created by sjbar on 16/01/2023.
//

#include "hittable/MovingSphere.hpp"

#include <utility>

namespace PathRehnda {
    MovingSphere::MovingSphere(Point3 start_point, Point3 end_point, double start_time, double end_time, double radius,
                               std::shared_ptr<Material> material) : start_point(start_point), end_point(end_point), start_time(start_time),
                                                                     end_time(end_time), radius(radius), material(std::move(material)) {

    }

    Point3 MovingSphere::centre(double time) const {
        const auto time_since_start = time - start_time;
        const auto movement_vector = end_point - start_point;
        return start_point + ((time_since_start) / (end_time - start_time)) * movement_vector;
    }

    // similar to sphere hit
    std::optional<HitResult> MovingSphere::hit(const Ray &ray, double t_min, double t_max) const {
        Vec3 oc = ray.origin - centre(ray.time); // (A - C)
        auto a = ray.direction.length_squared(); // b . b from the above (which is equal to the vector length squared)
        auto half_b = dot(oc, ray.direction); // b . (A - C)
        auto c = oc.length_squared() - radius * radius; // (A - C) . (A - C) - r^2
        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) {
            return {};
        }
        // find the nearest solution in the acceptable range
        auto sqrt_dis = sqrt(discriminant);
        auto root = (-half_b - sqrt_dis) / a;
        if (root < t_min || t_max < root) {
            root = (-half_b + sqrt_dis) / a;
            if (root < t_min || t_max < root)
                return {};
        }

        auto hit_location = ray.at(root);
        Vec3 normal = (hit_location - centre(ray.time)) / radius;
        bool front_face = true;
        // if the dot product is positive, then the normal is not against the ray, so the hit is inside
        if (dot(ray.direction, normal) > 0.0) {
            normal = -normal;
            front_face = false;
        }

        return HitResult{
                .hit_location = hit_location,
                .normal = normal,
                .material = material,
                .t = root,
                .front_face = front_face,
        };
    }

    std::optional<AABB> MovingSphere::bounding_box(double time_0, double time_1) const {
        AABB box_at_0 {
                centre(time_0) - Vec3(radius, radius, radius),
                centre(time_0) + Vec3(radius, radius, radius),
        };
        AABB box_at_1 {
                centre(time_1) - Vec3(radius, radius, radius),
                centre(time_1) + Vec3(radius, radius, radius),
        };
        return AABB::surrounding_box(box_at_0, box_at_1);
    }
} // PathRehnda