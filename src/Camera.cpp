//
// Created by sjbar on 11/01/2023.
//

#include "Camera.hpp"

namespace PathRehnda {

    Camera::Camera(double aspect_ratio) : aspect_ratio(aspect_ratio) {
        // camera properties
        auto viewport_height = 2.0;
        auto viewport_width = aspect_ratio * viewport_height;
        auto focal_length = 1.0;

        origin = Point3(0, 0, 0);
        horizontal = Vec3(viewport_width, 0, 0);
        vertical = Vec3(0, viewport_height, 0);
        lower_left_corner = origin - horizontal / 2 - vertical / 2 - Vec3(0, 0, focal_length);
    }

    Ray Camera::get_ray(double u, double v) const {
        return {
            origin,
            lower_left_corner + u * horizontal + v * vertical - origin
        };
    }

    double Camera::get_aspect_ratio() const {
        return aspect_ratio;
    }
}