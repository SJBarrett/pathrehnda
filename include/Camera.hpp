//
// Created by sjbar on 11/01/2023.
//

#pragma once

#include "Vec3.hpp"
#include "Ray.hpp"

namespace PathRehnda {
    class Camera {
    public:
        Camera(Point3 look_from, Point3 look_at, Vec3 up, double vertical_fov_degrees, double aspect_ratio, double aperture, double focus_dist);

        [[nodiscard]]
        Ray get_ray(double s, double t) const;

        [[nodiscard]]
        double get_aspect_ratio() const;

    private:
        double aspect_ratio;
        Point3 origin;
        Point3 lower_left_corner;
        Vec3 horizontal;
        Vec3 vertical;
        Vec3 look_direction;
        Vec3 u; // camera x axis
        Vec3 v; // camera y axis
        double lens_radius;
    };
}
