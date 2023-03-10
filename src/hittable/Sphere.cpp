//
// Created by sjbar on 10/01/2023.
//

#include "hittable/Sphere.hpp"

namespace PathRehnda {
    /**
     * We can solve for if a ray hits a sphere by finding a solution to the following equation:
     *      (P(t) - C) . (P(t) - C) = r^2
     * Which expands to:
     *      t^2*b . b + 2tb . (A - C) + (A - C) . (A - C) - r^2 = 0
     * If we find 0 solutions to the quadratic then there is no intersection with the surface of a sphere. 1 solution means it just skims
     * the surface. 2 solutions means the ray goes through the sphere, entering and exiting it.
     *
     * We can use the quadratic formula to find solutions when ax^2 + bx + c = 0:
     *     x = (-b +/- sqrt(b^2 - 4ac)) / (2 * a)
     *
     * If we let b = 2h the equation simplifies to the below, and can be further simplified by refactoring b*b to b.length_squared():
     *     x = (-h +/- sqrt(h*h - ac)) / a
     * This function will return the closest solution (smallest t value) if there is a hit, otherwise it is empty.
     */
    std::optional<HitResult> Sphere::hit(const Ray &ray, double t_min, double t_max) const {
        Vec3 oc = ray.origin - centre; // (A - C)
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
        Vec3 outward_normal = (hit_location - centre) / radius;
        Vec3 normal = outward_normal;
        bool front_face = true;
        // if the dot product is positive, then the normal is not against the ray, so the hit is inside
        if (dot(ray.direction, normal) > 0.0) {
            normal = -normal;
            front_face = false;
        }

        return HitResult{
                .hit_location = hit_location,
                .normal = normal,
                .t = root,
                .front_face = front_face,
                .material = material,
                .uv = get_unit_sphere_uv(outward_normal),
        };
    }

    std::optional<AABB> Sphere::bounding_box(double time_0, double time_1) const {
        return AABB{
                centre - Vec3(radius, radius, radius),
                centre + Vec3(radius, radius, radius)
        };
    }

    Uv Sphere::get_unit_sphere_uv(const Point3 &surface_point) {
        auto theta = acos(-surface_point.y());
        auto phi = atan2(-surface_point.z(), surface_point.x()) + pi;
        return {
            .u = phi / (2 * pi),
            .v = theta / pi,
        };
    }
} // PathRehnda