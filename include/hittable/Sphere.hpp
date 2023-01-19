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
        Sphere() = default;
        Sphere(Point3 centre, double radius, std::shared_ptr<Material> mat) : centre(centre), radius(radius), material(std::move(mat)) {};

        [[nodiscard]]
        std::optional<HitResult> hit(const Ray &ray, double t_min, double t_max) const override;

        [[nodiscard]]
        std::optional<AABB> bounding_box(double time_0, double time_1) const override;

    public:
        Point3 centre;
        double radius;
        std::shared_ptr<Material> material;

    private:
        // p: a given point on the sphere of radius one, centered at the origin.
        // u: returned value [0,1] of angle around the Y axis from X=-1.
        // v: returned value [0,1] of angle from Y=-1 to Y=+1.
        //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
        //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
        //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>
        static Uv get_unit_sphere_uv(const Point3& surface_point);
    };

} // PathRehnda