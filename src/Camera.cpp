//
// Created by sjbar on 11/01/2023.
//

#include "Camera.hpp"

namespace PathRehnda {

    Camera::Camera(Point3 look_from, Point3 look_at, Vec3 up, double vertical_fov_degrees, double aspect_ratio) {
        auto theta = degrees_to_radians(vertical_fov_degrees);
        auto h = tan(theta/2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = aspect_ratio * viewport_height;

        auto look_direction = unit_vector(look_from - look_at);
        auto u = unit_vector(cross(up, look_direction));
        auto v = cross(look_direction, u);

        origin = look_from;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - look_direction;
    }

    Ray Camera::get_ray(double s, double t) const {
        return {
            origin,
            lower_left_corner + s * horizontal + t * vertical - origin
        };
    }

    double Camera::get_aspect_ratio() const {
        return aspect_ratio;
    }
}